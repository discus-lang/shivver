
# Shivver (WIP)

The Reflective Lambda Machine with Cold Storage

This project is just getting started, so not much works yet.

Shivver is a minimal Scheme-like language intended as an intermediate language for
the reflective bootstrapping of higher level languages.

Shivver is an evolution of the older [Shimmer](https://github.com/discus-lang/shimmer) language,

The implementation is plain, portable C99 code.


## Grammar

```
Decl  ::= '@' Name '=' Term ';'

Term  ::= mvar   Name Nat            (like 'name^5')
       |  msym   Name                (like '%name)
       |  mprm   Name                (like '#name)
       |  mmac   Name                (like '@mac)

       |  mmmm n Term^n              '[' Term,* ']'

       |  mabs n Name^n Term         '{' Name* '}' Term
       |  mapp   Term Term           Term Term

       |  mlet   Name  Term Term     '!let' Name '=' Term '!in' Term
       |  mrec   Binds Term          '!rec' Binds '!in' Term

Binds ::= bind1  bind                Bind
       |  binds  Binds               Bind '!and' Binds

Bind  ::= bind   Name '=' Term       Name '=' Term
```




