
#include "shivver/codec/ascii.h"


// ------------------------------------------------------------------------------------------------
// Get the name of a token tag.
const char*
shivver_token_name
        (size_t tag)
{
        switch(tag)
        { case TOKEN_NONE:      return "none";
          case TOKEN_END:       return "end";
          case TOKEN_RBRA:      return "rbra";
          case TOKEN_RKET:      return "rket";
          case TOKEN_CBRA:      return "cbra";
          case TOKEN_CKET:      return "cket";
          case TOKEN_COMMA:     return "comma";

          case TOKEN_VAR:       return "var";
          case TOKEN_SYM:       return "sym";
          case TOKEN_PRM:       return "prm";
          default:              abort();
        }
}


// ------------------------------------------------------------------------------------------------
// Scan the input string to get the next token.
//
//  On lex success:
//   'outTag' is set to the token tag.
//   'outStr' is set to the pointer to the start of the token.
//   'outLen' is set to the length of the token.
//   function returns true.
//
//  On lex failure,
//   'outTag' is set to TOKEN_NONE.
//   'outStr' is set to 0.
//   'outLen' is set to 0.
//   function returns false.
//
bool    shivver_lexer_scan
        ( char*   str           // pointer to next character of input string.
        , size_t  strLen        // length of input prefix to consider.
        , size_t* outTag        // output for token tag.
        , char**  outStr        // output for pointer to start of next token.
        , size_t* outLen)       // output for token length.
{

  // Try to lex a token from this point.
  again:
        if (strLen == 0)
        {       *outTag = TOKEN_END;
                *outLen = 0;
        }

        // The first character determines what sort of token this is.
        switch (*str)
        { // whitespace
          case ' ':
          case '\t':
          case '\n':
                str++; strLen--;
                goto again;

          // punctuation
          case '(': *outTag = TOKEN_RBRA;  goto single;
          case ')': *outTag = TOKEN_RKET;  goto single;
          case '{': *outTag = TOKEN_CBRA;  goto single;
          case '}': *outTag = TOKEN_CKET;  goto single;
          case ',': *outTag = TOKEN_COMMA; goto single;

          // symbol
          case '%':
          {     *outTag = TOKEN_SYM;
                *outStr = str;
                *outLen = shivver_lexer_scan_symprm(str, strLen);
                return true;
          }

          // primitive
          case '#':
          {     *outTag = TOKEN_PRM;
                *outStr = str;
                *outLen = shivver_lexer_scan_symprm(str, strLen);
                return true;
          }

          default:
                // variables
                if (*str >= 'a' && *str <= 'z')
                {       *outTag = TOKEN_VAR;
                        *outStr = str;
                        *outLen = shivver_lexer_scan_var(str, strLen);
                        return true;
                }

                // lex failure.
                *outTag = TOKEN_NONE;
                *outStr = 0;
                *outLen = 0;
                return false;
        }

  single:
        *outLen = 1;
        *outStr = str;
        return true;

}


// ------------------------------------------------------------------------------------------------
// Scan a variable name, returning the raw length in the lex buffer.
size_t  shivver_lexer_scan_var
        (char* str, size_t strLen)
{
        size_t len = 0;
        while(  strLen > 0
         &&     *str >= 'a' && *str <= 'z')
        {
                str++; strLen--; len++;
        }

        return len;
}


// Load a variable name from the lex buffer into the given string buffer.
//   The state should be as it was just after scanning the token.
void    shivver_lexer_load_var
        (char* str, size_t strLen, char* out)
{
        for(size_t i = 0; i < strLen; i++)
                out[i] = str[i];
}


// ------------------------------------------------------------------------------------------------
// Scan a symbol name, returning the raw length in the lex buffer.
size_t  shivver_lexer_scan_symprm
        (char* str, size_t strLen)
{
        size_t len = 0;

        // skip across sigil.
        str++; strLen--; len++;

        while ( strLen > 0
         &&     *str >= 'a' && *str <= 'z')
        {
                str++; strLen--; len++;
        }

        return len;
}


// Load a symbol name from the lex buffer into the given string buffer.
//   The state should be as it was just after scanning the token.
void    shivver_lexer_load_symprm
        (char* str, size_t strLen, char* out)
{
        for (size_t i = 0; i < strLen; i++)
                out[i] = str[i];
}
