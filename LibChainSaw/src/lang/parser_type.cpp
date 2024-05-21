#include <csaw/Parser.hpp>
#include <csaw/Type.hpp>

csaw::Arg csaw::Parser::ParseArg()
{
    std::string name = Expect(TK_IDENTIFIER).Value;

    TypePtr type;
    if (!NextIfAt(":"))
    {
        type = ParseType(Type::Get(name));
        name = "";
    }
    else type = ParseType();

    return {name, type};
}

csaw::TypePtr csaw::Parser::ParseType()
{
    if (NextIfAt("("))
    {
        std::vector<TypePtr> args;
        bool is_vararg = false;

        while (!At(")") && !AtEOF())
        {
            if (NextIfAt("?"))
            {
                is_vararg = true;
                break;
            }

            args.push_back(ParseArg().Type);

            if (!At(")"))
                Expect(",");
        }
        Expect(")");

        Expect("(");
        const auto result = ParseType();
        Expect(")");

        return ParseType(FunctionType::Get(args, is_vararg, result));
    }

    const auto name = Expect(TK_IDENTIFIER).Value;
    const auto type = Type::Get(name);

    return ParseType(type);
}

csaw::TypePtr csaw::Parser::ParseType(const TypePtr& base)
{
    if (NextIfAt("*"))
        return ParseType(PointerType::Get(base));

    if (NextIfAt("["))
    {
        const auto size = Expect(TK_INT_BIN | TK_INT_OCT | TK_INT_DEC | TK_INT_HEX).IntValue();
        Expect("]");
        return ParseType(ArrayType::Get(base, size));
    }

    return base;
}
