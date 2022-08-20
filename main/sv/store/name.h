#pragma once
#include "sv/store/region.h"

/* --------------------------------------------------------------------------------------------- */
// A name table provides a bidirectional map between names and their codes.
// Terms in the store use these codes to identify names to allow equality
// comparison to be constant time.
typedef struct {
        // Index of the next name to allocate into this table.
        size_t index;

        // Region containing name records.
        sv_store_region_t* records;
} sv_store_name_table_t;


// Record representing a single name.
typedef struct {
        // Length of the name in bytes.
        size_t  length;

        // Null terminated name text.
        uint8_t name[];
} sv_store_name_record_t;


