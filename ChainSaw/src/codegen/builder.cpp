#include <csaw/CSaw.hpp>
#include <csaw/codegen/Builder.hpp>
#include <csaw/codegen/FunctionRef.hpp>
#include <llvm/Passes/PassBuilder.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Transforms/InstCombine/InstCombine.h>
#include <llvm/Transforms/Scalar/GVN.h>
#include <llvm/Transforms/Scalar/Reassociate.h>
#include <llvm/Transforms/Scalar/SimplifyCFG.h>

csaw::Builder::Builder(const std::string& moduleName)
{
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();

    llvm::orc::LLJITBuilder builder;
    builder.setLinkProcessSymbolsByDefault(true);
    m_JIT = m_Error(builder.create());

    llvm::orc::SymbolMap symbols;
    symbols[m_JIT->mangleAndIntern("fprintf")] = {
        llvm::orc::ExecutorAddr(llvm::pointerToJITTargetAddress(&fprintf)),
        llvm::JITSymbolFlags()
    };
    m_Error(m_JIT->getMainJITDylib().define(absoluteSymbols(symbols)));

    m_Context = std::make_unique<llvm::LLVMContext>();
    m_Builder = std::make_unique<llvm::IRBuilder<>>(*m_Context);
    m_Module = std::make_unique<llvm::Module>(moduleName, *m_Context);
    m_Module->setDataLayout(m_JIT->getDataLayout());
    m_Module->setTargetTriple(m_JIT->getTargetTriple().str());

    m_FPM = std::make_unique<llvm::FunctionPassManager>();
    m_LAM = std::make_unique<llvm::LoopAnalysisManager>();
    m_FAM = std::make_unique<llvm::FunctionAnalysisManager>();
    m_CGAM = std::make_unique<llvm::CGSCCAnalysisManager>();
    m_MAM = std::make_unique<llvm::ModuleAnalysisManager>();
    m_PIC = std::make_unique<llvm::PassInstrumentationCallbacks>();
    m_SI = std::make_unique<llvm::StandardInstrumentations>(*m_Context, true);
    m_SI->registerCallbacks(*m_PIC, m_MAM.get());

    m_FPM->addPass(llvm::InstCombinePass());
    m_FPM->addPass(llvm::ReassociatePass());
    m_FPM->addPass(llvm::GVNPass());
    m_FPM->addPass(llvm::SimplifyCFGPass());

    llvm::PassBuilder pb;
    pb.registerModuleAnalyses(*m_MAM);
    pb.registerFunctionAnalyses(*m_FAM);
    pb.crossRegisterProxies(*m_LAM, *m_FAM, *m_CGAM, *m_MAM);
}

llvm::LLVMContext& csaw::Builder::GetContext() const
{
    return *m_Context;
}

llvm::IRBuilder<>& csaw::Builder::GetBuilder() const
{
    return *m_Builder;
}

llvm::Module& csaw::Builder::GetModule() const
{
    return *m_Module;
}

int csaw::Builder::Main(const int argc, const char** argv)
{
    m_Error(m_JIT->addIRModule(llvm::orc::ThreadSafeModule(std::move(m_Module), std::move(m_Context))));

    const auto main_fn = m_Error(m_JIT->lookup("main")).toPtr<int(*)(int, const char**)>();
    return main_fn(argc, argv);
}

const csaw::FunctionRef& csaw::Builder::GetFunction(const std::string& name, const TypePtr& callee, const std::vector<TypePtr>& args)
{
    for (const auto& ref : m_Functions[name])
    {
        if (name != ref.Name) continue;
        if (callee != ref.Callee) continue;
        if (args.size() < ref.Args.size() || (args.size() > ref.Args.size() && !ref.IsVarArg)) continue;

        size_t i = 0;
        for (; i < ref.Args.size(); ++i)
            if (ref.Args[i] != args[i]) break;
        if (i < ref.Args.size()) continue;

        return ref;
    }

    return {};
}

csaw::FunctionRef& csaw::Builder::GetOrCreateFunction(const std::string& name, const bool constructor, const TypePtr& callee, const std::vector<TypePtr>& args, const bool vararg, const TypePtr& result)
{
    for (auto& ref : m_Functions[name])
    {
        if (name != ref.Name) continue;
        if (constructor != ref.IsConstructor) continue;
        if (callee != ref.Callee) continue;
        if (args.size() != ref.Args.size()) continue;
        if (vararg != ref.IsVarArg) continue;
        if (result != ref.Result) continue;

        size_t i = 0;
        for (; i < ref.Args.size(); ++i)
            if (ref.Args[i] != args[i]) break;
        if (i < ref.Args.size()) continue;

        return ref;
    }

    return m_Functions[name].emplace_back(nullptr, name, constructor, callee, args, vararg, result);
}

std::pair<int, csaw::TypePtr> csaw::Builder::ElementInStruct(const TypePtr& rawType, const std::string& element)
{
    const auto struct_type = std::dynamic_pointer_cast<StructType>(rawType);
    if (!struct_type)
        throw std::runtime_error("type is not a struct");

    int i = 0;
    for (const auto& [name, type] : struct_type->Elements)
    {
        if (name == element)
            return {i, type};
        ++i;
    }

    throw std::runtime_error("type has no element with the given name");
}

bool csaw::Builder::IsGlobal() const
{
    return !m_Builder->GetInsertBlock();
}

std::pair<csaw::ValueRef, csaw::ValueRef> csaw::Builder::CastToBestOf(const ValueRef& left, const ValueRef& right)
{
    if (left.GetType() == right.GetType())
        return {left, right};

    if (left.GetType()->isIntegerTy())
    {
        if (right.GetType()->isIntegerTy())
        {
            if (left.GetType()->getIntegerBitWidth() > right.GetType()->getIntegerBitWidth())
            {
                const auto value = m_Builder->CreateIntCast(right.GetValue(), left.GetType(), true);
                return {left, ValueRef::Constant(this, value, left.GetRawType())};
            }

            const auto value = m_Builder->CreateIntCast(left.GetValue(), right.GetType(), true);
            return {ValueRef::Constant(this, value, right.GetRawType()), right};
        }

        if (right.GetType()->isFloatingPointTy())
        {
            const auto value = m_Builder->CreateSIToFP(left.GetValue(), right.GetType());
            return {ValueRef::Constant(this, value, right.GetRawType()), right};
        }
    }

    if (left.GetType()->isFloatingPointTy())
    {
        if (right.GetType()->isIntegerTy())
        {
            const auto value = m_Builder->CreateSIToFP(right.GetValue(), left.GetType());
            return {left, ValueRef::Constant(this, value, left.GetRawType())};
        }
    }

    CSAW_WIP;
}
