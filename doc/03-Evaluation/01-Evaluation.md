
# Evaluation

1. evaluation always produces a closed term.
2. strong evaluation always produces a ground term.

```
 State =
  { heap → Loc × Norm }

 Env =
  { binds → Binds
  , defs  → Map Def Norm }

 Binds ::= binds Binds (List (Var × Norm))
        |  bindn Binds (Nom × Norm)
        |  bindz

 Mode ::= sim | rec
```
