
## Evaluation of General Terms
```
S | E ⊢ M ⇒Dⁿ S | U  e-term
```

```
                S₁ | E ⊢ U₁ ⇒D S₂  | U₂  e-susp
(EmSusp)       ---------------------------------
                S₁ | E ⊢ U₁ ⇒D¹ S₂ | U₂  e-term
```
Rule `EmSusp` evaluates a suspsension wrapped up as a term.
Suspensions have result arity of one, so the demand of the context must also have arity one.


```
                S₁ | E ⊢ U₁ ⇒D S₂ | U₂  e-susp
(EmVar)        ----------------------------------------------------
                S₁ | E[Xv*n ↦ U₁] ⊢ mvar Xv n ⇒D¹ S₂ | U₂  e-term
```
Rule `EmVar` retrieves the suspension corresponding to variable `Xv` from the environment.
Suspensions have result arity one, so the demand of the context must also have arity one.


```
(EmBoxWeak)     S  | E ⊢ mbox M ⇒w S | ssub E M  e-term

                S₁ | E ⊢ M ⇒s S₂ | U  e-term
(EmBoxStrong)  -----------------------------------
                S₁ | E ⊢ mbox M ⇒s S₂ | U  e-term


                S₁ | E ⊢ M ⇒s S₂ | N  e-term
(EmRun)        -----------------------------------
                S₁ | E ⊢ mrun M ⇒D S₂ | N  e-term
```
Rule `EmBoxWeak` suspends evaluation of a term by packaging it up together with the
environment in a delayed substitution.

Rule `EmBoxStrong` performs strong evaluation of a boxed body term by evaluating
the both term itself.

Rule `EmRun` performs strong evaluation of a body term independendently from
the demand imposed by the context. The term `!run M` is equivalent to applying the
strict identity function to the body term `M`, that is `({x} x) M`. We include a
separate `!run` form for symmetry with the `!box` form, and for convenience when
writing metaprograms that introspect on the term structure.


```
(EmAbs)         S | E ⊢ mabs n Dⁿ Xvⁿ M  ⇒D'  S | vclo n E Dⁿ Xvⁿ M  e-term
```
Rule `EmAbs` converts an abstraction into a value by packing the current
environment `E` along with the abstraction body `M` into a closure.


```
                S₁ | E₁ ⊢ M₁ ⇒Dⁿ S₂ | Uⁿ  e-term
                E₂ = E₁[binds = E.binds, sim {Xv.i ↦ U.i | i←1..n}]
                S₂ | E₂ ⊢ M₂ ⇒D'ᵐ S₃ | U'ᵐ  e-term
(EmLet)        -----------------------------------------------------
                S₁ | E₁ ⊢ mlet n Dⁿ Xvⁿ M₁ M₂ ⇒D'ᵐ S₃ | U'ᵐ  e-term
```
Rule `EmLet` evaluates a term of arity `n` and binds its results to the variables `Xvⁿ`.

Let-binding is strict in the number of results as we must resolve the individual
results to bind them in the environment, but not necessarily strict in the result
values themselves. See Note [Only single arity terms can be suspended.]

The term `(mlet n Dⁿ Xvⁿ M₁ M₂)` can also be expressed with a combination function
abstraction and application, that is `(mapp (mabs n Dⁿ Xvⁿ M₁) M₂)`. We include
a separate let form for convenience when writing metaprograms, and to improve
the performance of simple runtime implementations, so they do not need to construct
a closure and then immediately eliminate it.


```
                E₂ = E₁[binds = E₁.binds, rec {Xv.i ↦ ssub E₂ M.i | i←1..n}]
                S₁ | E₂ ⊢ M' ⇒Dᵐ S₂ | Uᵐ  e-term
(EmRec)        --------------------------------------------------------------
                S₁ | E₁ ⊢ mrec n Xvⁿ Mⁿ M' ⇒Dᵐ S₂ | Uᵐ  e-term

```
Rule `EmRec` adds recursive bindings to the environment which are in scope in
the body term `M'`. The recursive knot is tied at the meta-level, by defining
a recursive environment `E₂` whose bindings contain references back to `E₂`.

For implementation the rule can be modified so that the environment is attached
to the environment frame instead of allocating a new `ssub` constructor each time.


```
                { S | E ⊢ M.i ⇒D.i S' | U.i e-term, i←1..n } (sequence eval of states)
(EmMmm)        ----------------------------------------------
                S | E ⊢ mmmm n Mⁿ ⇒Dⁿ S' | Uⁿ e-term
```
Rule `EmMmm` evaluates each term in a list with the demand imposed by the context.


```
                S₁ | E₁ ⊢ M₁ ⇒s  S₂ | vclo n E₂ Dⁿ Xvⁿ M₃  e-term
                S₂ | E₁ ⊢ M₂ ⇒Dⁿ S₃ | Uⁿ  e-term
                E₃ = E₂[binds = E₁.binds, sim {Xv.i ↦ U.i, i←1..n}]
                S₃ | E₃ ⊢ M₃ ⇒D'ᵐ S₄ | U'ᵐ  e-term
(EmAppAbs)     -----------------------------------------------------
                S₁ ⊢ mapp M₁ M₂ ⇒D'ᵐ S₄ | U'ᵐ  e-term


                S₁ | E ⊢ M₁ ⇒s  S₂ | vprm Xp  e-term
                S₂ | E ⊢ M₂ ⇒Dⁿ S₃ | Uⁿ e-term        Dⁿ = demand(Xp)
                S₃ ⊢ Xp ; Uⁿ ⇒ S₄ | U'ᵐ e-prim
                S₄ | E ⊢ U'ᵐ ⇒D'ᵐ S₅ | U''ᵐ e-term
(EmAppPrm)     -------------------------------------------------
                S₁ ⊢ mapp M₁ M₂ ⇒D'ᵐ S₅ | U''ᵐ e-term
```
Rule `EmAppAbs` performs function application. Strong reduction of the functional
term `M₁` yields a closure and we then use the demand from the closure to
evaluate the arguments `Uⁿ`. The arguments are bound into the environment when
reducing the function body `M₃`. The function body is reduced with the demand
imposed by the context, and may be high arity.

Rule `EmAppPrm` performs primitive application. Strong reduction of the functional
term `M₁` yields a primitive name. The demand of the primitive is retrieved
to the meta-level primitive table, which we use to evaluate the arguments `Uⁿ`.
We use the primitive evaluation judgment e-term to evaluate the primtiive
itself producing a list of residue terms `U'`, then evaluate the residue terms
with the demand of imposed by the context. This residue evaluation is needed
for the case where the primitive produces a suspended term but the context
needs a value. In practice if the context places a weak demand on all the results
we can skip the residue evaluation as it has no effect.


```
                { S | E ⊢ M.i ⇒w S' | U.i , i←1..n }      (sequence eval of states)
(EmCon)       --------------------------------------------
                S | E ⊢ mcon n Xs Mⁿ ⇒D S' | vcon n Xs Uⁿ
```
Rule `EmCon` constructs a data value from a symbol name and a list of arguments.
The constructor itself places no particular demand on the arguments, so we
evaluate them weakly. In practice one can force the arguments of constructed
data to be values by using the `!run` form, eg `%sym [!run M₁, !run M₂]`.


## Only single arity terms can be suspended
Let-binding is strict in the number of results as we need to resolve
references to the individual results to bind them in the environment.
However, it is non-strict in the result values themselves.

For this reason lazy bindings of arity one are valid, but lazy binding
of higher arity are not. For example, this is valid:
```
    E₁ ⊢ mbox M₁ ⇒w¹ ssub E₁ M₁
    E₁, let x ↦ ssub E₁ M₂ ⊢ M₂ ⇒Dⁿ Uⁿ
  -------------------------------------
    E₁ ⊢ mlet 1 x (mbox M₁) !in M₂ ⇒Dⁿ Uⁿ
```
Here we can capture the environment in a delayed substitution and bind it
into as a new variable.

But this is not:
```
    E₁ ⊢ mbox (mmmm 2 {M₁₁ M₁₂}) ⇒w² ssub E₁ (mmmm 2 {M₁₁ M₁₂})    (stuck)
    E₁, let x ↦ ssub E₁ (mmmm {M₁₁ M₁₂}) ⊢ M₂ ⇒Dⁿ Uⁿ
  ---------------------------------------------------------- (no rule)
    E₁ ⊢ mlet 2 {x y} (mbox (mmmm 2 {M₁₁ M₁₂})) M₂ ⇒Dⁿ Uⁿ
```
There is no rule that will reduce a term boxing with arity two,
because all suspensions must have arity one.

If we were to instead write:
```
    E₁ ⊢ mbox 2 (mmmm 2 {M₁₁ M₁₂}) ⇒w₁ ssub E₁ (mmmm 2 {M₁₁ M₁₂})
    E₁, let x ↦ ssub E₁ (mmmm 2 {M₁₁ M₁₂}) ⊢ M₂ ⇒Dⁿ Uⁿ
  ------------------------------------------------------------
    E₁ ⊢ mlet 1 {x} (mbox (mmmm 2 {M₁₁ M₁₂})) M₂ ⇒Dⁿ Uⁿ
```
This will encounter a runtime arity error when we demand the result of 'x',
as the EsSubStrong rule requires the embedded term to have arith one.

```
   S₁ | E₁, E₂ ⊢ (mmmm 2 {M₁₁ M₁₂}) ⇒s S₂ | ???
  ------------------------------------------------- (no rule)
   S₁ | E₁ ⊢ ssub E₂ (mmmm 2 {M₁₁ M₁₂}) ⇒s S₂ | U
```


## Delayed substitutions, closures and thunks

The delayed substitution form `(ssub E M)` and a zero-arity closure `(vclo 0 E {} M)`
contain identical information, and can both be used to encode call-by-name evaluation.
The difference is that delayed substitutions can be transparently evaluated by
the calling context by placing a strong demand on the result. On the other hand,
zero arity closures must be applied to an empty argument list to continue evaluation.
In the source language this is `({} M) []`.

