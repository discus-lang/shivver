
#include "sv/store.h"
#include "sv/source/term.h"


// Check that the given object tag is in-range.
static inline
sv_store_object_check_tag(
        sv_store_object_tag_t   tag)
{
        assert(tag >= sv_store_object_tag_top);
        assert(tag <= sv_store_object_tag_bot);
}


// Dereference an object.
//  @param slab   For cold objects the slab that contains it.
//  @param object Machine pointer to the object that contains the reference.
//  @param ref    Reference within the specified object.
static inline
sv_store_object_t*
sv_store_object_deref(
        sv_store_slab_t*    slab,
        sv_store_object_t*  object,
        sv_store_ref_t      ref)
{
        // hot object, reference is a machine pointer.
        if (object->super.header.hot_cold) {
                assert(slab == 0);
                assert(ref.ptr & SV_STORE_MASK_REF_LOW == 0);
                sv_store_object_check_tag(object->super.header.tag);
                return ref.ptr;
        }

        // cold object, reference is offset from the start of the slab.
        // slab must be specified.
        assert(slab != 0);
        assert(ref.off < slab->size);
        sv_store_object_check_tag(object->super.header.tag);

        // pointer to start of object.
        return slab->space + ref.off;
}


// Lookup a name in the specified name chain.
bool
sv_source_name_lookup(
        sv_store_slab_t*    slab,
        sv_store_object_t*  object,
        size_t              length,
        char*               name,
        sv_store_object_name_def_t* out_def)
{
 again:
        switch(object->super.header.tag) {
        case sv_store_object_tag_name_def:
                sv_store_object_name_def_t* object_def = &(object->name_def);

                // Current object defines the name.
                if (object_def->length == length
                 && strcmp(name, object_def->name) == 0) {
                        out_def = object_def;
                        return true;
                }

                // This is the end of the name def chain,
                //  and the name has not been found.
                else if (object_def->link.raw == 0) {
                        out_def = 0;
                        return false;
                }

                // Move to the next link in the chain.
                else {
                        sv_store_object_t* link
                         = sv_store_object_deref(slab, object_def, object_def->link);
                        object = link;
                        goto again;
                }

        default:
                assert(false);
        }
}


// sv_store_offset_t
// sv_source_create_name(
//         sv_store_region_t*      region,
//         sv_source_space_t       space,
//         size_t                  length,
//         char*                   name)
// {
//
// }