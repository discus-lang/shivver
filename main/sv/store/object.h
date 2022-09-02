#pragma once
#include <stdint.h>

/* --------------------------------------------------------------------------------------------- */
// Object tags.
//   We use non-contiguous codes to help debugging,
//   as it is less likely arbitrary memory will contain a valid code.
typedef enum {
        sv_store_object_tag_bot         = 0x10,
        sv_store_object_tag_name_def    = 0x11,
        sv_store_object_tag_name_ref    = 0x22,
        sv_store_object_tag_mmm         = 0x33,
        sv_store_object_tag_app         = 0x44,
        sv_store_object_tag_abs         = 0x55,
        sv_store_object_tag_let         = 0x66,
        sv_store_object_tag_rec         = 0x77,
        sv_store_object_tag_box         = 0x88,
        sv_store_object_tag_run         = 0x99,
        sv_store_object_tag_top         = 0xa0
} sv_store_object_tag_t;


/* --------------------------------------------------------------------------------------------- */
// Objects that can appear in stores.
union sv_store_object_t_;

typedef struct {
        // Bit indicating hot or cold obhect.
        unsigned int hot_cold   : 1;

        // Unused flag bits in header.
        unsigned int unused     : 7;

        // Tag identifying the sort of object.
        unsigned int tag        : 8;
} sv_store_object_header_t;


// Super type of all store objects.
typedef struct {
        sv_store_object_header_t header;
} sv_store_object_super_t;


// Name definition.
// Name definitions are stored in a chain of cold objects.
typedef struct {
        sv_store_object_header_t header;

        // Length of null terminated string.
        unsigned int length     : SV_STORE_BITS_NAME_LENGTH;

        // Offset of previous name definition record in the chain,
        // or 0 if this is the first one.
        sv_store_ref_t link;

        // Null terminated string defining the name,
        uint8_t name[];
} sv_store_object_name_def_t;


// Name reference to a name definition object.
typedef struct {
        sv_store_object_header_t header;

        // Reference to the name definition object.
        sv_store_ref_t ref;
} sv_store_object_name_ref_t;


// Cold argument list.
typedef struct {
        sv_store_object_header_t header;

        // Number of arguments.
        unsigned int count      : SV_STORE_BITS_MMM_COUNT;

        // Offset to argument within the same region.
        uint64_t arg[];
} sv_store_object_mmm_t;


// Hot reference to a cold object.
typedef struct {
        sv_store_object_header_t header;

        // Index of the cold region in the current zone.
        unsigned int region     : SV_STORE_BITS_COLD_REGION;

        // Offset to the object in the specified cold region.
        sv_store_ref_t ref;
} sv_store_object_cold_ref_t;


typedef union {
        sv_store_object_super_t         super;
        sv_store_object_name_def_t      name_def;
        sv_store_object_name_ref_t      name_ref;
        sv_store_object_mmm_t           mmm;
        sv_store_object_cold_ref_t      cold_ref;
} sv_store_object_t;