
# Grammar


## Syntax
```
Var     → (like 'name')
Def     → (like '@name')
Loc     → (like '*name')
Nom     → (like '?name')
Sym     → (like '%name')
Prm     → (like '#name')

Module  ::= module Var Decl*            '!shivver' Sym Decl*

Decl    ::= decl   Def Term             '!def' Def '=' Term

Value   ::= vloc   Loc                  Loc
         |  vsym   Sym                  Sym
         |  vprm   Prm                  Prm

         |  vcon n Sym Valueⁿ           !CON Sym '[' Blocked,* ']'
         |  vclo n Env Varⁿ Term        !CLO Env '{' Var* '}' Term

Blocked ::= bval   Value                Value
         |  bdef   Def                  Def
         |  bnom   Nom                  Nom
         |  bbox   Closed               !BOX Closed

Closed  ::= cblk   Blocked              Blocked
         |  csub   Env Term             !SUB Env Term

Term    ::= mcls   Closed               Closed

         |  mvar   Var Nat              Var '^' Nat

         |  mcon   Sym Termⁿ            Sym '[' Term,* ']'

         |  mlet n Varⁿ Term Term       '!let' '{' Var* '}' '=' Term '!in' Term
         |  mrec n Varⁿ Normⁿ Term      '!rec' Var '=' Norm ('!and' ...)* '!in' Term

         |  mmmm n Termⁿ                '[' Term,* ']'
         |  mabs n Varⁿ Term            '{' Var* '}' Term
         |  mapp   Term Term            Term Term

         |  mbox   Term                 '!box' Term
         |  mrun   Term                 '!run' Term
```



## Sugar (WIP)
Use `|` to attach a final argument on the end of an argument list.
This would avoid excessive nesting when defining match expressions.

```
!def @append {xx yy}
 = #match
   [xx, %list'nil,  {} yy
      , #match [xx, %list'cons, {x xs} %list'cons[x, @append [xs, yy]]]]

!def @append {xx yy}
 = #match [xx, %list'nil,  {} yy]
 | #match [xx, %list'cons, {x xs} %list'cons[x, @append [xs, yy]]]
```
