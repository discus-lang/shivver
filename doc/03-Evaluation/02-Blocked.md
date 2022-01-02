
## Evaluation of Blocked Terms
A blocked term is closed but not necessarily a value. It may or may not be able
to be evaluated further with a particular environment.

```
S | E ⊢ B ⇒Dⁿ S | B
```
The judgment `S₁ | E ⊢ B₁ ⇒Dⁿ S₂ | B₂` reads:
In state `S₁` and environment `E` blocked term `B₁` evaluates with demand `Dⁿ`,
producing a new state `S₁` and blocked term `B₂`.

Blocked terms are guaranteed to be closed, that is not contain free variables
However, a blocked term may not be a value, as it may be a Nom that is not unfolded
because there is no binding for it in the environment, or a Def or boxed term that
is not reduced further as it is in a weak evaluation context.

Weak evaluation substitutes for nominal variables but not definition names,
and does not reduce boxed terms.

Stong evaluation substitutes for both nominal variable names and definition names,
and also reduces boxed terms.

Evaluation that starts with an environment containing a binding for some nominal
variable Xn cannot distinguish between a term that contains occurrences of Xn
and the same term with the bound term substituted for Xn. This is similar to the
case of attempted evaluation of a term with a free variable Xv, except that if
evaluation encounters a nominal variable Xn with no corresponding binding then
it is not stuck


```
(EbVal)         S | E ⊢ bval V ⇒D¹ S | bval V
```
Values are already fully evaluated.


```
(EbDefWeak)     S | E ⊢ bdef Xd ⇒w S | bdef Xd

                S₁ | E ⊢ B₁ ⇒s S₂ | B₂
(EbDefStrong)  --------------------------------------
                S₁ | E[Xd ↦ B₁] ⊢ bdef Xd ⇒s S₂ | B₂
```
Rule EbDefWeak for weak normalization leaves definition names as is.
Evaluation can produce a definiton name directly. As definition names are
all bound at top level they are guaranteed to still be in scope in the calling
context. This is unlike variable names whose associated bindings may be in the
local environment which is not available to the calling context.

Rule EbDefStrong retrieves the definition with name `Xd` from the definiton
list and strongly evaluattes it.


```
                    ¬ (Xn ∈ dom(E.binds))
(EbNomKeep)    ---------------------------------
                S | E ⊢ bnom Xn ⇒D¹ S | bnom Xn

                S₁ | E ⊢ B₁ ⇒D¹ S₂ | B₂
(EbNomSub)     ---------------------------------------
                S₁ | E[Xn ↦ B₁] ⊢ bnom Xn ⇒D¹ S₂ | B₂
```
Rule EbNomKeep applies when nom Xn does not have a corresponding binding in
the environment. In this case the nom cannot be reduced further. Compared with
EbDefWeak, in this case evaluation can produce a nominal variable name directly,
but can supply its own binding or not, as needed. It is not an error for the
environment to not contain a binding for a particular nominal variable, as
per the case with plain variables and definition names.

Rule EbNomSub applies when nom Xn binds a blocked term B₁. The result of
evaluation is the result of evaluating the blocked term B₁ with the demand
imposed by the context.

With each form of evaluation, if a binding for some nominal variable `Xn` exists
in the environment then it will be substituted for `Xn`. In this sense both plain
variable names `Xv` and nominal varible names `Xn` are substitutive, so that
evaluation cannot distinguish beteen the name and the term that is substituted
for it. We could also implement the evaluation semantics using direct substitution
for plain variables and nominal variables, instead of binding terms in an
auxilliary environment.


```
(EnBoxWeak)     S | E ⊢ bbox C ⇒w S | bbox C

                S₁ | E₁ ⊢ C ⇒s S₂ | B
(EnBoxStrong)  --------------------------------
                S₁ | E₁ ⊢ bbox C ⇒s S₂ | B
```
Rule EnBoxWeak says that weak evaluation of a boxed term produces that term as-is.

Rule EnBoxStrong strongly evaluates a boxed term 'C₁' by evaluating 'C₁' itself.
In this way the context can cause the blocked term to be evaluated as far as
possible, given the available nominal bindings in the environment.



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
a combination of def and nom, eg @stop = ?stop.





