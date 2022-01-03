
# Grammar


## Syntax
```
Var     → (like 'name')
Def     → (like '@name')
Loc     → (like '*name')
Nom     → (like '?name')
Sym     → (like '%name')
Prm     → (like '#name')

Module  ::= module Var Decl*                '!shivver' Sym Decl*

Decl    ::= decl   Def Term                 '!def' Def '=' Term

Demand  ::= dwkk                            '~'
         |  dstr                            ''    (no annotation)

Value   ::= vloc   Loc                      Loc
         |  vsym   Sym                      Sym
         |  vprm   Prm                      Prm

         |  vcon n Sym Valueⁿ               !CON Sym '[' Susp,* ']'
         |  vclo n Env Varⁿ Term            !CLO Env '{' (Demand Var)* '}' Term

Susp    ::= Value
         |  sdef   Def                      Def
         |  snom   Nom                      Nom
         |  ssub   Env Term                 !SUB Env Term

Term    ::= Susp

         |  mvar   Var Nat                  Var '^' Nat

         |  mbox   Term                     '!box' Term
         |  mrun   Term                     '!run' Term

         |  mabs n Demandⁿ Varⁿ Term        '{' (Demand Var)* '}' Term
         |  mlet n Demandⁿ Varⁿ Term Term   '!let' '{' (Demand Var)* '}' '=' Term '!in' Term
         |  mrec n Varⁿ Termⁿ Term          '!rec' Var '=' Term ('!and' ...)* '!in' Term

         |  mmmm n Termⁿ                    '[' Term,* ']'
         |  mapp   Term Term                Term Term

         |  mcon   Sym Termⁿ                Sym '[' Term,* ']'
```
Suspensions have arity zero because we bind them in the environment.
```
E, x₁ ↦ U₁, x₂ ↦ U₂
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
