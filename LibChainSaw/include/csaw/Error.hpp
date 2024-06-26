#pragma once

namespace csaw
{
    static int CSawError;

    struct SourceLoc;
    void ThrowError(const SourceLoc& loc, bool is_severe, const char* format, ...);
}

#ifdef __GNUG__
#define ThrowErrorStmt(_Statement, _IsSevere, _Format, ...) csaw::ThrowError((_Statement).Loc, _IsSevere, _Format __VA_OPT__(,) __VA_ARGS__)
#else
#define ThrowErrorStmt(_Statement, _IsSevere, _Format, ...) csaw::ThrowError((_Statement).Loc, _IsSevere, _Format, __VA_ARGS__)
#endif
