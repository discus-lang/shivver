
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

Demand  ::= dwwk                            '~'
         |  dstr                            ''    (no annotation)

Value   ::= vloc   Loc                      Loc
         |  vsym   Sym                      Sym
         |  vprm   Prm                      Prm

         |  vcon n Sym Valueⁿ               !CON Sym '[' Answer,* ']'
         |  vclo n Env Varⁿ Term            !CLO Env '{' (Demand Var)* '}' Term

Answer  ::= Value
         |  adef   Def                      Def
         |  anom   Nom                      Nom
         |  asub   Env Term                 !SUB Env Term

Term    ::= Answer

         |  mvar   Var                      Var

         |  mapp   Term Term                Term Term

         |  mmmm n Termⁿ                    '[' Term,* ']'

         |  mabs n Demandⁿ Varⁿ Term        '{' (Demand Var)* '}' Term

         |  mlet n Demandⁿ Varⁿ Term Term   '!let' '{' (Demand Var)* '}' '=' Term '!in' Term
         |  mrec n Varⁿ Termⁿ Term          '!rec' Var '=' Term ('!and' ...)* '!in' Term

         |  mbox   Term                     '!box' Term
         |  mrun   Term                     '!run' Term
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

### Let bindings

Let bindings of a single variable can be written without parens.
```
  !let x = Term !in Term
  !let {x} = Term !in Term
```

```
!seq    term1;
        !let {x, y} = term2;
        term3

!let {} = term1 !in
!let {x, y} = term2 !in
term3
```
