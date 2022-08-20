
# Source Grammar

```
Var     → (like 'name')
Def     → (like '@name')
Loc     → (like '*name')
Nom     → (like '?name')
Sym     → (like '%name')
Prm     → (like '#name')

Decl
 ::= decl   Def Term                 '!def' Def '=' Term

Demand
 ::= dwwk                            '~'
  |  dstr                            ''    (no annotation)

Value
 ::= vloc   Loc                      Loc
  |  vsym   Sym                      Sym
  |  vprm   Prm                      Prm
  |  vcon n Sym Answer               !CON Sym '[' Answer,* ']'
  |  vclo n Env Demandⁿ Varⁿ Term    !CLO Env '{' (Demand Var)* '}' Term

Answer
 ::= aval   Value
  |  adef   Def                      Def
  |  anom   Nom                      Nom
  |  asub   Env Term                 !SUB Env Term

Term
 ::= mans   Answer
  |  mvar   Var                      Var
  |  mapp   Term Term                Term Term
  |  mmmm n Termⁿ                    '[' Term,* ']'
  |  mabs n Demandⁿ Varⁿ Term        '{' (Demand Var)* '}' Term
  |  mlet n Demandⁿ Varⁿ Term Term   '!let' '{' (Demand Var)* '}' '=' Term '!in' Term
  |  mrec n Varⁿ Termⁿ Term          '!rec' Var '=' Term ('!and' ...)* '!in' Term
  |  mbox   Term                     '!box' Term
  |  mrun   Term                     '!run' Term
```

## Sugar

### Let bindings of a single variable
Let bindings of a single variable can be written without braces
```
  !let x   = Term !in Term
≡ !let {x} = Term !in Term
```

### Argument list extension
Use `&` to attach a final argument on the end of an argument list.
```
[a, b] & c ≡ [a, b, c]
```

This is helpful to write deeply nested applications of matching functions,
where the last argument defines the fall-through case.
```
  f [x1, x2] & g [x3, x4] & h [x5]
≡ f [x1, x2, g [x3, x4, h [x5]]]
```

