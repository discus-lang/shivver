#pragma once
#include "shivver/heap/object/atomic.h"


// Destruct an application to a vector of terms with an expected length.
//  If this is indeed an application then set the output values and return true,
//  otherwise return false.
static inline bool
dAps    ( obj_t*        obj         // object to destruct.
        , uint32_t      nArgs       // expect number of arguments.
        , obj_t**       outFun      // out pointer for functional expression.
        , obj_t**       outArgs)    // out pointer for argument array with 'nArgs' elements.
{
        switch (xObj_tag(obj))
        { case TAG_APSH:
          {     uint32_t nArgsHas = xApsH_len(obj);
                if (nArgsHas != nArgs) return false;
                *outFun = xApsH_fun(obj);
                for (uint32_t i = 0; i < nArgs; i++)
                        outArgs[i] = xApsH_arg(obj, i);
                return true;
          }
        }
        return false;
}


// Destruct an application of a symbol to a vector of terms with an expected length.
//  If this is indeed such an application then set the output values and return true,
//  otherwise return false.
static inline bool
dSymAps ( obj_t*        obj         // object to destruct
        , char*         name        // expected symbol name
        , uint32_t      nArgs       // expect number of arguments
        , obj_t**       outArgs)    // out pointer for argument array with 'nArgs' elements.
{
        switch (xObj_tag(obj))
        { case TAG_APSH:
          {     uint32_t nArgsHas = xApsH_len(obj);
                if (nArgsHas != nArgs) return false;

                obj_t* oFun = xApsH_fun(obj);
                switch (xObj_tag(oFun))
                { case TAG_SYMA:
                  {     uint32_t nLen   = xSymA_len(oFun);
                        if (strlen(name) != nLen) return false;
                        if (strncmp(xSymA_name(oFun), name, nLen) != 0) return false;
                        break;
                  }
                }

                for (uint32_t i = 0; i < nArgs; i++)
                        outArgs[i] = xApsH_arg(obj, i);

                return true;
          }
        }
        return false;
}