
## Evaluation of (explicitly) Closed Terms
A closed term has no free variables evaluation further evaluation may be blocked
so it is not necessarily a value. A closed term may not may not be able to be
evaluated further with a given environment.

```
S | E ⊢ C ⇒Dⁿ S | B
```
The judgment `S₁ | E ⊢ C ⇒Dⁿ S₂ | B` reads:
In state `S₁` and environment `E` closed term `C` evaluates with demand `Dⁿ`,
producing a new state `S₁` and blocked term `B`.

```
                S | E ⊢ B₁ ⇒D¹ S | B₂
(EcBlocked)    ---------------------------
                S | E ⊢ cblk B₁ ⇒D¹ S | B₂

                S₁ | E₁, E₂ ⊢ M ⇒D¹ S | B
(EcSub)        -----------------------------------
                S₁ | E₁ ⊢ csub E₂ M ⇒D¹ S | B
```
Rule EcBlocked says that evaluation of an already blocked term continues
to evaluated it as far as possible with the current demand.

Rule EcSubStrong valuates a delayed subsitution (csub E₂ M) by extending
the existing environment E₁ with E₂ and evaluating the body M.

