
## Evaluation of Normal Terms
```
S | E ⊢ N ⇒Dⁿ S | N
```
The judgment `S₁ | E ⊢ N₁ ⇒Dⁿ S₂ | N₂` reads:
In state `S₁` and environemnt `E` normal term `N₁` is evaluated with demand `Dⁿ`,
producing a new state `S₁` and normal term `N₂`.

Normal terms are guaranteed to not contain free variables, but they may not be
values, may contain folded definition names (Defs), and may contain unsubstituted
nominal variables (Noms).

Weak reduction substitutes for nominal variable names but not definition names,
and does not reduce boxed terems.

Stong reduction substitutes for both nominal variable names and definition names,
and also reduces boxed terms.


```
(EnVal)         S | E ⊢ nval V ⇒D¹ S | nval V
```
Values cannot be evaluated any further.


```
(EnDefWeak)     S | E ⊢ ndef Xd ⇒w S | ndef Xd

                S₁ | E ⊢ E.defs[Xd] ⇒s S₂ | N
(EnDefStrong)  -------------------------------
                S₁ | E ⊢ ndef Xd ⇒s S₂ | N
```
Rule EnDefWeak for weak normalization leaves definition names as is.
Rule EnDefStrong retrieves the definition with name `Xd` from the definiton
list and strongly normalizes it.


```
                    ¬ (Xn ∈ dom(E.binds))
(EnNomKeep)    --------------------------------
                S | E ⊢ nnom Xn ⇒D¹ S | nnom Xn

                S₁ | E ⊢ N₁ ⇒D¹ S₂ | N₂
(EnNomSub)     ---------------------------------------
                S₁ | E[Xn ↦ N₁] ⊢ nnom Xn ⇒D¹ S₂ | N₂
```
Rule EnNomKeep applies when nom Xn does not have a corresponding binding in
the environment. In this case the nom cannot be reduced further.

Rule EnNomSub applies when nom Xn binds a normal term N₁. The result of
evaluation is the result of evaluating the normal term N₁ with the demand
imposed by the context.


```
(EnBoxWeak)     S | E ⊢ nbox N ⇒w S | nbox N

                S₁ | E₁ ⊢ N₁ ⇒s S₂ | N₂
(EnBoxStrong)  --------------------------------
                S₁ | E₁ ⊢ nbox N₁ ⇒s S₂ | N₂
```
Rule EnBoxWeak says that weak evaluation of a normal term produces that term as-is.

Rule EnBoxStrong strongly evaluates a boxed normal term 'N₁' by evaluating 'N₁' itself.
In this way the context can cause a normal term to be evaluated as far as possible,
given the available nominal bindings in the environment.


```
(EnSubWeak)     S | E ⊢ nsub E M ⇒w S | nsub E M

                S₁ | E₁, E₂ ⊢ M ⇒s S | N
(EnSubStrong)  -----------------------------------
                S₁ | E₁ ⊢ nsub E₂ M ⇒s S | N
```
Rule EnSubWeak says that weak evaluation of a delayed substitution produces that
term as-is, similarly to rule EnBoxWeak.

Rule EnSubStrong strongly evaluates a delayed subsitution (nsub E₂ M) by extending
the existing environment E₁ with E₂ and evaluating the body M.


