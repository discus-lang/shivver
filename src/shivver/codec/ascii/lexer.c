
#include "shivver/codec/ascii.h"


// ------------------------------------------------------------------------------------------------
// Get the name of a token tag.
//   The name is statically allocated and does not need to be freed.
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
          case TOKEN_SBRA:      return "sbra";
          case TOKEN_SKET:      return "sket";

          case TOKEN_COMMA:     return "comma";
          case TOKEN_EQ:        return "eq";

          case TOKEN_VAR:       return "var";
          case TOKEN_SYM:       return "sym";
          case TOKEN_PRM:       return "prm";
          case TOKEN_MAC:       return "mac";
          case TOKEN_KEY:       return "key";
          case TOKEN_NAT:       return "nat";

          case TOKEN_KEY_DEF:   return "key'def";
          case TOKEN_KEY_LET:   return "key'let";
          case TOKEN_KEY_REC:   return "key'rec";
          case TOKEN_KEY_IN:    return "key'in";

          case TOKEN_KEY_SHIVVER: return "key'shivver";

          default: shivver_fail("shivver_token_name: no match");
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
        // We are already at the end of the input.
        if (strLen == 0)
        {       *outTag = TOKEN_END; *outStr = str;
                *outLen = 0;
                return true;
        }

        // The first character determines what sort of token this is.
        switch (*str)
        { // end of string
          case '\0':
                *outTag = TOKEN_END; *outStr = str;
                *outLen = 0;
                return true;

          // whitespace
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
          case '[': *outTag = TOKEN_SBRA;  goto single;
          case ']': *outTag = TOKEN_SKET;  goto single;
          case ',': *outTag = TOKEN_COMMA; goto single;
          case ';': *outTag = TOKEN_SEMI;  goto single;
          case '=': *outTag = TOKEN_EQ;    goto single;

          // symbol name
          case '%':
          {     *outTag = TOKEN_SYM; *outStr = str;
                *outLen = shivver_lexer_scan_signame(str, strLen);
                return true;
          }

          // primitive name
          case '#':
          {     *outTag = TOKEN_PRM; *outStr = str;
                *outLen = shivver_lexer_scan_signame(str, strLen);
                return true;
          }

          // macro name
          case '@':
          {     *outTag = TOKEN_MAC; *outStr = str;
                *outLen = shivver_lexer_scan_signame(str, strLen);
                return true;
          }

          // keyword name
          case '!':
          {     size_t nStr = shivver_lexer_scan_signame(str, strLen);
                char*  buf  = (char*)alloca(nStr + 1);
                memcpy(buf, str, nStr);
                buf[nStr] = 0;

                *outTag = TOKEN_NONE; *outStr = str;
                *outLen = 0;

                // Check if this is one of the recognised keywords.
                if (strcmp(buf, "!shivver") == 0)
                {       *outTag = TOKEN_KEY_SHIVVER;
                        *outLen = nStr;
                        return true;
                }

                if (strcmp(buf, "!def") == 0)
                {       *outTag = TOKEN_KEY_DEF;  *outLen = nStr; return true; }

                if (strcmp(buf, "!let") == 0)
                {       *outTag = TOKEN_KEY_LET;  *outLen = nStr; return true; }

                if (strcmp(buf, "!rec") == 0)
                {       *outTag = TOKEN_KEY_REC;  *outLen = nStr; return true; }

                if (strcmp(buf, "!in") == 0)
                {       *outTag = TOKEN_KEY_IN;   *outLen = nStr; return true; }

                return false;
          }

          default:
                // variables
                if (*str >= 'a' && *str <= 'z')
                {       *outTag = TOKEN_VAR; *outStr = str;
                        *outLen = shivver_lexer_scan_var(str, strLen);
                        return true;
                }

                // primitive nats without the #nat' prefix.
                if (*str >= '0' && *str <= '9')
                {       *outTag = TOKEN_NAT; *outStr = str;
                        *outLen = shivver_lexer_scan_nat(str, strLen);
                        return true;
                }

                // lex failure.
                *outTag = TOKEN_NONE; *outStr = 0;
                *outLen = 0;
                return false;
        }

  // Return a single character token.
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
         && (   (*str >= 'a' && *str <= 'z')
             || (*str >= 'A' && *str <= 'Z')
             || (*str >= '0' && *str <= '9')
             || (*str == '\'')
             || (*str == '_')))
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
size_t  shivver_lexer_scan_signame
        (char* str, size_t strLen)
{
        size_t len = 0;

        // skip across sigil.
        str++; strLen--; len++;

        while(  strLen > 0
         && (   (*str >= 'a' && *str <= 'z')
             || (*str >= 'A' && *str <= 'Z')
             || (*str >= '0' && *str <= '9')
             || (*str == '\'')
             || (*str == '_')))
        {
                str++; strLen--; len++;
        }

        return len;
}


// Load a symbol name from the lex buffer into the given string buffer.
//   The state should be as it was just after scanning the token.
void    shivver_lexer_load_signame
        (char* str, size_t strLen, char* out)
{
        for (size_t i = 0; i < strLen; i++)
                out[i] = str[i];
}


// ------------------------------------------------------------------------------------------------
// Scan a natural number, returning the raw length in the lex buffer.
size_t  shivver_lexer_scan_nat
        (char* str, size_t strLen)
{
        size_t len = 0;

        while(  strLen > 0
         &&    (*str >= '0' && *str <= '9'))
        {
                str++; strLen--; len++;
        }

        return len;
}


