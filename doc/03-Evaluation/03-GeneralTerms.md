
## Evaluation of General Terms
```
                S | E ⊢ M ⇒Dⁿ S | N


                S₁ | E ⊢ N₁ ⇒D S₂ | N₂
(EmNrm)        ----------------------------
                S₁ | E ⊢ mnrm N₁ ⇒D S₂ | N₂


                S₁ | E ⊢ E.binds[Xv*n] ⇒D S₂ | N
(EmVar)        ------------------------------------
                S₁ | E ⊢ mvar Xv n ⇒D S₂ | N


                { S | E ⊢ M.i ⇒w S' | N.i , i←1..n }                   (sequence eval of states)
(EmCon)       ---------------------------------------------------
                S | E ⊢ mcon n Xs Mⁿ ⇒D S' | nval (vcon n Xs Nⁿ)


                S₁ | E₁ ⊢ M₁ ⇒wⁿ S₂ | Nⁿ
                E₂ = E₁[binds = E.binds, vars {Xv.i ↦ N.i | i←1..n}]
                S₂ | E₂ ⊢ M₂ ⇒Dᵐ S₃ | N'ᵐ
(EmLet)        -----------------------------------------------------------
                S₁ | E₁ ⊢ mlet n Xvⁿ M₁ M₂ ⇒Dᵐ S₃ | N'ᵐ


                E₂ = E₁[binds = E₁.binds, rec {Xv.i ↦ nbox E₂ M.i | i←1..n}]
                S₁ | E₂ ⊢ M' ⇒Dᵐ S₂ | Nᵐ
(EmRec)        --------------------------------------------------------------
                S₁ | E₁ ⊢ mrec n Xvⁿ Mⁿ M' ⇒Dᵐ S₂ | Nᵐ


                { S | E ⊢ M.i ⇒D.i S' | N.i , i ← 1..n }               (sequence eval of states)
(EmMmm)        ------------------------------------------
                S | E ⊢ mmmm n Mⁿ ⇒Dⁿ S' | Nⁿ


(EmAbs)         S | E ⊢ mabs n Xvⁿ M  ⇒D  S | nval (vclo n E Xvⁿ M)


                S₁ | E₁ ⊢ M₁ ⇒s  S₂ | nval (vclo n E₂ Xvⁿ M₃)
                S₂ | E₁ ⊢ M₂ ⇒wⁿ S₃ | Nⁿ
                E₃ = E₂[binds = E₁.binds, vars (Xv.i ↦ N.i), i←1..n]
                S₃ | E₃ ⊢ M₃ ⇒Dᵐ S₄ | N'ᵐ
(EmAppAbs)     ------------------------------------------------------
                S₁ ⊢ mapp M₁ M₂ ⇒Dᵐ S₄ | N'ᵐ


                S₁ | E ⊢ M₁ ⇒s  S₂ | nval (vprm Xp)
                S₂ | E ⊢ M₂ ⇒Dⁿ S₃ | Nⁿ         Dⁿ = demand(Xp)
                S₃ ⊢ Xp ; Nⁿ ⇒ S₄ | N'ᵐ
                S₄ | E ⊢ N'ᵐ ⇒D'ᵐ S₅ | N''ᵐ
(EmAppPrm)     -------------------------------------------------
                S₁ ⊢ mapp M₁ M₂ ⇒D'ᵐ S₅ | N''ᵐ


(EmBoxW)        S  | E ⊢ mbox M ⇒w S | nbox (nsub E M)

                S₁ | E ⊢ M ⇒s S₂ | N
(EmBoxS)       ---------------------------
                S₁ | E ⊢ mbox M ⇒s S₂ | N


                S₁ | E ⊢ M ⇒s S₂ | N
(EmRun)        --------------------------
                S₁ | E ⊢ mrun M ⇒D S₂ | N
```
