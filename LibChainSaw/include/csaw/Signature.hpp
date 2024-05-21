#pragma once

#include <csaw/Def.hpp>

namespace csaw
{
    struct Signature
    {
        [[nodiscard]] std::string Mangle() const;
        [[nodiscard]] bool IsConstructor() const;

        [[nodiscard]] FunctionTypePtr GetFunctionType() const;

        std::string Name;
        TypePtr Parent;
        TypePtr Result;
        std::vector<TypePtr> Args;
        bool IsVarargs = false;
        bool IsC = false;
    };
}
