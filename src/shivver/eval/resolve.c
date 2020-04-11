
#include "shivver/heap.h"
#include "shivver/prim.h"
#include "shivver/eval.h"

// TODO: handle bump counter.
//
// Lookup a variable name from the given environment.
//  If we find it then return the associated object,
//  otherwise return 0.
//  We treat an environment pointer of 0 as an empty environment,
//  and will always return false if given one.
//
obj_t*
shivver_eval_resolve_var
        ( obj_t*        oEnv
        , char*         name
        , size_t        bump)
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


// Lookup a macro name from the given declaration list.
//  If we find it then return the associated object,
//  otherwise return 0.
//  We treat an declaration list pointer of 0 as an empty list,
//  and will always return false if given one.
obj_t*
shivver_eval_resolve_mac
        ( eval_decl_t*  decl
        , char*         name)
{ again:
        if (decl == 0)
                return 0;

        if (shivver_eval_eqMac(decl->name, name))
                return decl->body;

        decl = decl->next;
        goto again;
}


// Check if a variable has the given name.
bool
shivver_eval_eqSym
        ( obj_t*        oExp
        , char*         name)
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


// Check if a macro has the given name.
bool
shivver_eval_eqMac
        ( obj_t*        oExp
        , char*         name)
{
        switch(xObj_tag(oExp))
        { case TAG_MACA:
                return strcmp(xMacA_name(oExp), name) == 0;

          default:
                printf("* eqMac: object is not a macro name");
                shivver_prim_console_printp(oExp);
                shivver_fail("evaluation failed");
        }
}


