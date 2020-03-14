

```
Term  ::= mvar   Name Nat       (like 'name^0')
       |  msym   Name           (like '%name')
       |  mprm   Name           (like '#name')

       |  mmmm n Term^n         '[' Term,* ']'

       |  mabs n Name^n Term    '{' Name* '}' Term
       |  mapp   Term   Term    Term Term

       |  mlet Bind  Term       '!let' Bind  '!in' Term
       |  mrec Binds Term       '!rec' Binds '!in' Term

Binds ::= Bind
       |  Bind ':and' Binds

Bind  ::= bind n Name^n Term    '[' Name,* ']' '=' Term
```

```
@length
 = #case'list
        [ {} 0
        , {x xs} #nat'add [1, @length xs] ]
```

```
:let [x, y, z] = #prm [a, b, c]
```


```
 Σ ⊢ mvar x n ⇒1 Σ[x*n]

 Σ ⊢ msym s   ⇒1 msym s

 Σ ⊢ mprm p   ⇒1 mprm p

 Σ ⊢ mabs n x^n M  ⇒1  mclo Σ n x^n M

 Σ ⊢ M₁ ⇒1  mclo Σ' n x^n M₃
 Σ ⊢ M₂ ⇒n  V^n
 Σ'[(x ↦ V)^n] ⊢ M₃ ⇒a V'^a
 ---------------------------
 Σ ⊢ mapp M₁ M₂ ⇒a V'^a

 { Σ ⊢ M ⇒1 V }^n
 --------------------
 Σ ⊢ mmmm M^n ⇒n V^n



```