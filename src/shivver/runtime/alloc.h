

// ----------------------------------------------------------------------------
// A symbol with a statically allocated name string.
//
//     7 6 5 4 3 2 1 0
// 0   0 . . . . . . F
// 1   ptr to c-string
static inline obj_t*
aSymT (const char* name)
{
        uint64_t buf = halloc(2);
        buf[0] = nBump << 8 | TAG_SYMT;
        buf[1] = (uint64_t)name;
        return buf;
}

// A variable with a statically allocated name string.
//
//     7 6 5 4 3 2 1 0
// 0   0 . . . bump  F
// 1   ptr to c-string
//
static inline obj_t*
aVarT (const char* name, uint24_t nBump)
{
        uint64_t* buf = halloc(2);
        buf[0] = nBump << 8 | TAG_VART;
        buf[1] = (uint64_t)name;
        return buf;
}
