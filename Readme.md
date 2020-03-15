
# Shivver (WIP)

The Reflective Lambda Machine with Cold Storage

Shivver is a minimal Scheme-like language intended for reflective bootstrapping
of higher level languages. Shivver is an evolution of the older
[Shimmer](https://github.com/discus-lang/shimmer) language.

The implementation is plain, portable C99 code.

This project is just getting started, so not much works yet.


## Grammar

```
Var     → (like 'name')
Sym     → (like '%name')
Prm     → (like '#name')
Mac     → (like '@name')

Module  ::= module Var Decl*            '!shivver' Sym Decl*

Decl    ::= decl   Mac Term             '!def' Mac '=' Term

Term    ::= mvar   Var Nat              Var '^' Nat
         |  msym   Sym                  Sym
         |  mprm   Prm                  Prm
         |  mmac   Mac                  Mac

         |  mmmm n Term^n               '[' Term,* ']'

         |  mabs n Var^n Term           '{' Var* '}' Term
         |  mapp   Term Term            Term Term

         |  mlet n Var^n Term Term      '!let' '{' Var* '}' '=' Term '!in' Term
         |  mrec   Binds Term           '!rec' Binds '!in' Term

Binds   ::= bind1  bind                 Bind
         |  binds  Binds                Bind '!and' Binds

Bind   ::=  bind   Var '=' Term         Var '=' Term
```




