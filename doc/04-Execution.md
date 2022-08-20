
# Runtime Expressions

```
Term
 ::= mvar Var
  |  mdef Def
  |  mloc Loc
  |  msym Sym
  |  mprm Prm
  |  mcon n Sym Termⁿ
  |  mclo n Env Varⁿ
  |  mapp Term Term
  |  mmmm n Termⁿ
  |  mabs n Demandⁿ Varⁿ Term
  |  mlet n Demandⁿ Varⁿ Term Term
  |  mrec n Varⁿ Termⁿ Term
  |  mbox
  |  mrun

  |  mnat Nat
```


# Single Step Evaluation
```
E | K | mapp M1 M2              => E | K, rapp'1 M2       | M1
E | K | mmmm n Mⁿ               => E | K, rmmm 0 Mⁿ .ⁿ    | M0
E | K | mabs n Dⁿ X M           => E | K                  | vclo n E Dⁿ Xⁿ M
E | K | mlet n Dⁿ Xⁿ M₁ M₂      => E | K, rlet n Dⁿ Xⁿ M₂ | M₁
E | K | mrun M                  => E | K, rrun            | M

E | K | mvar x                  => E[x]
```
