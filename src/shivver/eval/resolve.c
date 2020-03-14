
#include "shivver/heap.h"
#include "shivver/prim.h"
#include "shivver/eval.h"

// TODO: handle bump counter.
// Lookup a variable from the given environment.
//  If we find it set the outResult to the value and return true,
//  otherwise return false.
//
//  We treat an environemnt pointer of NULL as an empty environment,
//  and will always return false if given one.
//
obj_t*
shivver_eval_resolveT
        (obj_t* oEnv, char* name, size_t bump)
{ again:
        if (oEnv == 0)
                return 0;

        size_t nCount = xEnvH_len(oEnv);
        for (size_t i = 0; i < nCount; i++)
        {       obj_t* oParm = xEnvH_var(oEnv, i);
                if (shivver_eval_eqSym (oParm, name))
                {       return xEnvH_val(oEnv, i);
                }
        }

        oEnv = xEnvH_parent(oEnv);
        goto again;
}


// Check if a symbol has the given name.
bool
shivver_eval_eqSym
        (obj_t* oExp, char* name)
{
        switch(xObj_tag(oExp))
        { case TAG_VART:
                return strcmp(xVarT_name(oExp), name) == 0;

          case TAG_VARA:
                return strcmp(xVarA_name(oExp), name) == 0;

          default:
                printf("* eqSym: object is not a variable");
                shivver_prim_console_printp(oExp);
                shivver_fail("evaluation failed");
        }
}

