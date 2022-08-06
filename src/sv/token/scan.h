#pragma once
#include "sv/token.h"
#include "sv/token/state.h"

bool    sv_token_scan
        ( sv_token_state_t*        state
        , sv_token_t*              out_token);

size_t  sv_token_scan_sigil_name
        (char* str, size_t strLen);

