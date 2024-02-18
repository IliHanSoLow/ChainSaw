#include <codegen/Context.h>

csaw::codegen::ValuePtr csaw::codegen::Context::CreateCmpLT(ValuePtr left, ValuePtr right)
{
	auto result = std::make_shared<Value>(GetNumType());
	auto inst = std::make_shared<CmpInst>(CMP_LT, left, right, result);
	m_InsertPoint->Insert(inst);
	return result;
}

csaw::codegen::ValuePtr csaw::codegen::Context::CreateCmpLE(ValuePtr left, ValuePtr right)
{
	auto result = std::make_shared<Value>(GetNumType());
	auto inst = std::make_shared<CmpInst>(CMP_LE, left, right, result);
	m_InsertPoint->Insert(inst);
	return result;
}

csaw::codegen::ValuePtr csaw::codegen::Context::CreateCmpEQ(ValuePtr left, ValuePtr right)
{
	auto result = std::make_shared<Value>(GetNumType());
	auto inst = std::make_shared<CmpInst>(CMP_EQ, left, right, result);
	m_InsertPoint->Insert(inst);
	return result;
}

csaw::codegen::ValuePtr csaw::codegen::Context::CreateMul(ValuePtr left, ValuePtr right)
{
	auto result = std::make_shared<Value>(GetNumType());
	auto inst = std::make_shared<MulInst>(left, right, result);
	m_InsertPoint->Insert(inst);
	return result;
}

csaw::codegen::ValuePtr csaw::codegen::Context::CreateDiv(ValuePtr left, ValuePtr right)
{
	auto result = std::make_shared<Value>(GetNumType());
	auto inst = std::make_shared<DivInst>(left, right, result);
	m_InsertPoint->Insert(inst);
	return result;
}

csaw::codegen::ValuePtr csaw::codegen::Context::CreateRem(ValuePtr left, ValuePtr right)
{
	throw;
}

csaw::codegen::ValuePtr csaw::codegen::Context::CreateAnd(ValuePtr left, ValuePtr right)
{
	throw;
}

csaw::codegen::ValuePtr csaw::codegen::Context::CreateOr(ValuePtr left, ValuePtr right)
{
	throw;
}

csaw::codegen::ValuePtr csaw::codegen::Context::CreateXOr(ValuePtr left, ValuePtr right)
{
	throw;
}

csaw::codegen::ValuePtr csaw::codegen::Context::CreateLAnd(ValuePtr left, ValuePtr right)
{
	throw;
}

csaw::codegen::ValuePtr csaw::codegen::Context::CreateLOr(ValuePtr left, ValuePtr right)
{
	throw;
}

csaw::codegen::ValuePtr csaw::codegen::Context::CreateShL(ValuePtr left, ValuePtr right)
{
	throw;
}

csaw::codegen::ValuePtr csaw::codegen::Context::CreateShR(ValuePtr left, ValuePtr right)
{
	throw;
}

csaw::codegen::ValuePtr csaw::codegen::Context::CreateNeg(ValuePtr value)
{
	auto result = std::make_shared<Value>(GetNumType());
	auto inst = std::make_shared<NegInst>(value, result);
	m_InsertPoint->Insert(inst);
	return result;
}

csaw::codegen::ValuePtr csaw::codegen::Context::CreateNot(ValuePtr value)
{
	throw;
}

csaw::codegen::ValuePtr csaw::codegen::Context::CreateInv(ValuePtr value)
{
	throw;
}