
## Evaluation of Suspensions
A suspension is a closed term with an arity of one, but but not necessarily
a value. It may or may not be able to be evaluated further with a particular
environment.

```
S | E ⊢ U ⇒D S | U susp
```
The judgment `S₁ | E ⊢ U₁ ⇒D S₂ | U₂` reads:
In state `S₁` and environment `E` suspension `U₁` evaluates with demand `Dⁿ`,
producing a new state `S₁` and suspension `U₂`.

Suspensions are guaranteed to be closed, that is not contain free variables
However, a suspension may not be a value, as it may be a Nom that is not
unfolded because there is no binding for it in the environment, or a Def name
or subsitution that is not reduced further as it is in a weak evaluation context.

Weak evaluation substitutes for nominal variables but not definition names,
and does not reduce delayed substitutions.

Stong evaluation substitutes for both nominal variable names and definition names,
and also reduces delayed substitutions.

Evaluation that starts with an environment containing a binding for some nominal
variable `Xn` cannot distinguish between a term that contains occurrences of `Xn`
and the same term with the bound term substituted for `Xn`. This is similar to the
case of attempted evaluation of a term with a free variable `Xv`, except that if
evaluation encounters a nominal variable `Xn` with no corresponding binding then
it is not stuck


```
(EsVal)         S | E ⊢ V ⇒D S | V  e-susp
```
Values are already fully evaluated.


```
(EsDefWeak)     S | E ⊢ sdef Xd ⇒w S | sdef Xd  e-susp

                S₁ | E ⊢ U₁ ⇒s S₂ | U₂  e-susp
(EsDefStrong)  --------------------------------------
                S₁ | E[Xd ↦ U₁] ⊢ sdef Xd ⇒s S₂ | U₂  e-susp
```
Rule EsDefWeak for weak evaluation leaves definition names as is.
As definition names are all bound at top level they are guaranteed to still be
in scope in the calling context. This is unlike variable names whose associated
bindings may be in the local environment which is not available to the calling
context.

Rule EsDefStrong retrieves the definition with name `Xd` from the definiton
list and strongly evaluates it.


```
                    ¬ (Xn ∈ dom(E.binds))
(EsNomKeep)    ---------------------------------
                S | E ⊢ snom Xn ⇒D S | snom Xn  e-susp

                S₁ | E ⊢ U₁ ⇒D S₂ | U₂  e-susp
(EsNomSub)     ----------------------------------------------
                S₁ | E[Xn ↦ U₁] ⊢ snom Xn ⇒D S₂ | U₂  e-susp
```
Rule EsNomKeep applies when nom Xn does not have a corresponding binding in
the environment. In this case the nom cannot be reduced further. Compared with
EsDefWeak, in this case evaluation can produce a nominal variable name directly,
but can supply its own binding or not, as needed. It is not an error for the
environment to not contain a binding for a particular nominal variable, as
per the case with plain variables and definition names.

Rule EsNomSub applies when nom `Xn` binds a suspension `U₁`. The result of
evaluation is the result of evaluating the suspension `U₁` with the demand
imposed by the context.

With each form of evaluation, if a binding for some nominal variable `Xn` exists
in the environment then it will be substituted for `Xn`. In this sense both plain
variable names `Xv` and nominal varible names `Xn` are substitutive, so that
evaluation cannot distinguish beteen the name and the term that is substituted
for it. We could instead implement the evaluation semantics using direct
substitution for plain variables and nominal variables, instead of binding terms
in an auxilliary environment.


```
(EsSubWeak)     S | E ⊢ ssub M ⇒w S | ssub M  e-susp

                S₁ | E₁, E₂ ⊢ M ⇒s S₂ | U  e-term
(EsSubStrong)  -----------------------------------------
                S₁ | E₁ ⊢ ssub E₂ M ⇒s S₂ | U  e-susp
```
Rule EsSubWeak says that weak evaluation of a delayed substitution produces
that term as-is.

Rule EsSubStrong evaluates a suspended term `M₁` with an outer substitution
by adding the environment from the substitution to the existing environment
and continuing evaluation.

As we use an environment machine we do not need "push rules" to push the
substitution down into the term structure, for example:
```
  push (msub E (mapp M₁ M₂)) => mapp (msub E M₁) (msub E M₂)
```
In particular, pushing substitutions under binders requires a mechanism
to avoid variable capture which would complicate the semantics. For shivver
we instead lift the term level delayed substitution into the environment,
which is a meta-level delayed substitution.


### Coverage of Variable Behaviour
Compare the behaviour of each form of variable:
 1) plain variable names are automatically unfolded,
    and this is always possible in well-formed terms.

 2) declaration names are unfolded according to the demand of the context,
    and the unfolding is always possible in well-formed terms.

 3) nominal variables are automatically unfolded,
    and this may or may not be possible in well-formed terms.

```
       unfolded       unfolding
       automatically  always possible
  var   O              O
  def   X              O
  nom   O              X
  ???   X              X
```
The final variable form ??? doesn't seem useful. As the bindings are
not unfolded automatically they would need to be defined at top level,
because it would not be valid to keep them in the environment of a
delayed subsitution and apply them automatically via the push rules.
If we wanted a name defined at top level that was not unfolded automatically,
but strong evaluation was not always possible we could express this with
a combination of def and nom, eg `@stop = ?stop`.
