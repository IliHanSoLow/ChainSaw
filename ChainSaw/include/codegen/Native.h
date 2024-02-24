#pragma once

#include <codegen/Context.h>
#include <codegen/Instruction.h>
#include <codegen/Value.h>

#include <functional>
#include <memory>

namespace csaw::codegen
{
	typedef std::function<ConstPtr(ContextPtr context, ConstPtr callee, const std::vector<ConstPtr>& args)> NativeFunction;

	struct NativeInst : Instruction
	{
		NativeInst(NativeFunction function);

		NativeFunction Function;
	};

	typedef std::shared_ptr<NativeInst> NativeInstPtr;
}