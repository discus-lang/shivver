
#include "shivver/prim.h"
#include "shivver/eval/error.h"

// Check that this is a NatA object, and take its value.
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

// Check that this is a NatA object, and take its value.
size_t
shivver_eval_takeBool
        ( obj_t*        obj)
{
        switch(xObj_tag(obj))
        { case TAG_BLLA:
                return xBllA(obj);
        }
        shivver_fail("not a bool");
}


static inline size_t
takeBool (obj_t* obj)
{       return shivver_eval_takeBool(obj);
}


static inline size_t
takeNat (obj_t* obj)
{       return shivver_eval_takeNat(obj);
}


// Evaluate a primitive operators applied to its arguments.
// TODO: inline this into main evaluator to avoid stack buildup.
void
shivver_eval_prim
        ( eval_t*       state
        , obj_t*        oEnv
        , size_t        nResExpect
        , obj_t**       osRes
        , size_t        tag
        , size_t        nArgActual
        , obj_t**       osArg)
{
        // Check the primitive produces the expected number of results.
        size_t nResActual = shivver_prim_tag_results(tag);
        reqeval ( state, (nResActual == 0xff) || (nResExpect == nResActual)
                , "Arity mismatch in prim result -- need '%d', have '%d'"
                , nResExpect, nResActual);

        // Check the primitive has the expected number of parameters.
        size_t nArgExpect  = shivver_prim_tag_args(tag);
        reqeval ( state, (nArgExpect == 0xff) || (nArgExpect == nArgActual)
                , "Arity mismatch in prim application -- need '%d', have '%d'"
                , nArgActual, nArgExpect);

        switch(tag) {
         case PRIM_NAT_ADD:
                osRes[0] = aNatA(takeNat(osArg[0]) + takeNat(osArg[1]));
                return;

         case PRIM_NAT_SUB:
                osRes[0] = aNatA(takeNat(osArg[0]) - takeNat(osArg[1]));
                return;

         case PRIM_NAT_MUL:
                osRes[0] = aNatA(takeNat(osArg[0]) * takeNat(osArg[1]));
                return;

         case PRIM_NAT_DIV:
                osRes[0] = aNatA(takeNat(osArg[0]) / takeNat(osArg[1]));
                return;

         case PRIM_NAT_EQ:
                osRes[0] = aBllA(takeNat(osArg[0]) == takeNat(osArg[1]));
                return;

         case PRIM_NAT_LT:
                osRes[0] = aBllA(takeNat(osArg[0]) <  takeNat(osArg[1]));
                return;

         case PRIM_NAT_LE:
                osRes[0] = aBllA(takeNat(osArg[0]) <= takeNat(osArg[1]));
                return;

         case PRIM_NAT_GT:
                osRes[0] = aBllA(takeNat(osArg[0]) >  takeNat(osArg[1]));
                return;

         case PRIM_NAT_GE:
                osRes[0] = aBllA(takeNat(osArg[0]) >= takeNat(osArg[1]));
                return;

         case PRIM_IF:
         {      obj_t* oClo = 0;
                if (takeBool(osArg[0])) oClo = osArg[1];
                else                    oClo = osArg[2];

                reqeval ( state, xObj_tag(oClo) == TAG_CLOH
                        , "branch of '#if' is not a closure.");

                reqeval ( state, xCloH_len(oClo) == 0
                        , "closure in branch of '#if' has wrong shape.");

                // TODO: calling this again will build a stack frame,
                // need to do a tail-call.
                shivver_eval_termN
                        ( state, nResExpect, osRes
                        , oEnv, xCloH_body(oClo));
                return;
         }

         default:
                reqeval ( state, false
                        , "No evaluator for primitive #%s = 0x%08x"
                        , shivver_prim_nameOfTag(tag), tag);
        }
}

