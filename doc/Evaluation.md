

```
 (EvVar)   Σ ⊢ mvar x n ⇒1 Σ[x*n]

 (EvSym)   Σ ⊢ msym s   ⇒1 msym s

 (EvPrm)   Σ ⊢ mprm p   ⇒1 mprm p

            { Σ ⊢ M ⇒1 V }ⁿ
 (EvMmm)  ---------------------
           Σ ⊢ mmmm Mⁿ ⇒n Vⁿ

 (EvAbs)   Σ ⊢ mabs n xⁿ M  ⇒1  mclo Σ n xⁿ M

           Σ ⊢ M₁ ⇒1 mclo Σ' n xⁿ M₃
           Σ ⊢ M₂ ⇒n Vⁿ
           Σ'[(x ↦ V)ⁿ] ⊢ M₃ ⇒a Uᵃ
 (EvApp)  ----------------------------
           Σ ⊢ mapp M₁ M₂ ⇒a U^a


          Σ ⊢ M₁ ⇒m Vᵐ  Σ[(x ↦ V)ᵐ] ⊢ M₂ ⇒n Uⁿ
 (EvLet)  --------------------------------------
           Σ ⊢ let n xᵐ M₁ M₂ ⇒n Uⁿ
```