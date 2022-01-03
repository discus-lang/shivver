

## Lemmas

### Strong evaluation
Strong evaluation produces either a value or a nom where there is no substituion
for it in the environment.
```
forall S₁ S₂ E M N.
  (S₁ | E ⊢ M ⇒s S₂ | N)
  implies (exists V.  N = nval V)
       or (exists X . N = nom Xn) and ¬(Xn ∈ dom(E))
```


