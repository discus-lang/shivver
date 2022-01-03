
# Evaluation

1. evaluation always produces a closed term.
2. strong evaluation always produces a ground term.

```
 State =
  { heap → Loc × Norm }

 Env =
  { binds → Binds
  , defs  → Map Def Norm }

 Binds :: bsim Binds     (List (Var × Susp))
        | brec Binds Env (List (Var × Term))
        | bnom Binds Nom Susp
        | bnil

 Mode ::= sim | rec
```
