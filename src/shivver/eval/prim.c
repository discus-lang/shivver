
#include "shivver/prim.h"


size_t
shivver_eval_takeNat
        ( obj_t*        obj)
{
        switch(xObj_tag(obj))
        { case TAG_NATA:
                return xNatA(obj);
        }
        shivver_fail("not a nat");
}


static inline size_t
takeNat (obj_t* obj)
{       return shivver_eval_takeNat(obj);
}


void
shivver_eval_prim
        ( size_t        nArity
        , obj_t**       osRes
        , size_t        tag
        , obj_t**       osArg)
{
        switch(tag) {
         case PRIM_NAT_ADD:
         {      osRes[0] = aNatA(takeNat(osArg[0]) + takeNat(osArg[1]));
                return;
         }

         case PRIM_NAT_SUB:
         {      osRes[0] = aNatA(takeNat(osArg[0]) - takeNat(osArg[1]));
                return;
         }

         case PRIM_NAT_MUL:
         {      osRes[0] = aNatA(takeNat(osArg[0]) * takeNat(osArg[1]));
                return;
         }

         case PRIM_NAT_DIV:
         {      osRes[0] = aNatA(takeNat(osArg[0]) / takeNat(osArg[1]));
                return;
         }

         default:
                shivver_fail("invalid prim");
        }
}

