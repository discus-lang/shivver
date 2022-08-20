#pragma once
#include <stdint.h>

/* --------------------------------------------------------------------------------------------- */
typedef enum {
        sv_store_object_tag_cold_name_def,
        sv_store_object_tag_cold_name_ref,
        sv_store_object_tag_cold_mmm,
        sv_store_object_tag_cold_app,
        sv_store_object_tag_cold_abs,
        sv_store_object_tag_cold_let,
        sv_store_object_tag_cold_rec,
        sv_store_object_tag_cold_box,
        sv_store_object_tag_cold_run,

        sv_store_object_tag_hot_cold_ref,
        sv_store_object_tag_hot_mmm,
        sv_store_object_tag_hot_app,
        sv_store_object_tag_hot_abs,
        sv_store_object_tag_hot_let,
        sv_store_object_tag_hot_rec,
        sv_store_object_tag_hot_box,
        sv_store_object_tag_hot_run
} sv_store_object_tag_t;


/* --------------------------------------------------------------------------------------------- */
// Objects that can appear in stores.
union sv_store_object_t_;


// Name definition.
// Name definitions are stored in a chain of cold objects.
typedef struct {
        unsigned int tag        : SV_STORE_BITS_TAG;

        // Space of the name.
        unsigned int space      : SV_STORE_BITS_NAME_SPACE;

        // Length of null terminated string.
        unsigned int length     : SV_STORE_BITS_NAME_LENGTH;

        // Offset of previous name definition record in the chain.
        unsigned int offset     : SV_STORE_BITS_OFFSET;

        // Null terminated string defining the name,
        uint8_t name[];
} sv_store_object_cold_name_def_t;


// Name reference to a name definition object.
typedef struct {
        unsigned int tag        : SV_STORE_BITS_TAG;

        // Offset of name definition object in the cold space of the current zone.
        unsigned int offset     : SV_STORE_BITS_OFFSET;
} sv_store_object_cold_name_ref_t;


// Cold argument list.
typedef struct {
        unsigned int tag        : SV_STORE_BITS_TAG;

        // Number of arguments.
        unsigned int count      : SV_STORE_BITS_MMM_COUNT;

        // Offset to argument within the same region.
        uint64_t arg[];
} sv_store_object_cold_mmm_t;


// Hot reference to a cold object.
typedef struct {
        unsigned int tag        : SV_STORE_BITS_TAG;

        // Index of the cold region in the current zone.
        unsigned int region     : SV_STORE_BITS_COLD_REGION;

        // Offset to the object in the specified cold region.
        unsigned int offset     : SV_STORE_BITS_OFFSET;
} sv_store_object_hot_cold_ref_t;


