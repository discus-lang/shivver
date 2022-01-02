
# Names
We use six forms of names, which exist in separate namespaces.
Each form of name exists in a separate namespace that is indicated by the preceeding
sigil, except for variable names that have no sigil as they are the most common.

The six forms of names arise from the different combinations of properties that names
typically have. The names and their properties are summarised as follows:

```
                            value ground equality identity fixed
Var     → (like 'name')     X     X      X        X        X
Def     → (like '@name')    O     X      X        X        X
Loc     → (like '*name')    O     O      X        X        X
Nom     → (like '?name')    O     O      O        X        X
Sym     → (like '%name')    O     O      O        O        X
Prm     → (like '#name')    O     O      O        O        O
```

## Propeties of names
The properties of names are:

value:    the name can be produced as the result of evaluation.
ground:   the name is a value that cannot be evaluated further.
equality: two names of the same form can be compared for equality.
identity: evaluation can distinguish two terms where all occurrences of
          some name are replaced by another name of the same class.
fixed:    the set of available names of this class is finite,
          and fixed by the implementation.

If a name lacks a particular property this can imply it must lack some other:
 ¬value    ⇒ ¬ground    if a name is not a value then it cannot be a ground value.
 ¬value    ⇒ ¬equality  if a name is not a value then it cannot be compared for equality.
 ¬value    ⇒ ¬identify  if a name is not a value then does not have identity.
 ¬equality ⇒ ¬identity  if a name does not have equality then it does not have identity.


## Forms of names

### Var: Variable names
These are the variable names common to functional programming languages in the ML tradition.
Variable names are bound locally in the term by lambda and let abstractions, and are targets
for substitution. Evaluation of a closed term that does not introspect on the term structure
cannot distinguish between a variable occurrence and the value that will ultimately be
substituted for it.


### Def: Definition names
Definitions are all bound at the top level of the program. When used in a strict context,
defintion names are automatically unfolded to their bound terms. Definitions are unfolded
on demand so that top-level definitions can include redexes without being sensitive
to the order of the definitions.


### Loc: Location names
Location names are used to identify values that are bound in the heap. Heap bindings
can be write never (cref / immutable), write once (oref), or write many (mref).
The evaluator is free to merge identical immutable or written oref bindings, during
executoin, and to ensure this process is not observable to the client program we do
not permit locations to be compared for equality, and they do not have their own
identity.


### Nom: Nominal variable names
Nominal variables are placeholders that can be compared for equality but do not have
their own identity. New nominal variables can be introduced as globally fresh names
during reduction, without the order of intoducing in a larger program being observable.


### Sym: Symbol names
Symbol names are new values that can be introduced in the client program. As they have
identity they are similar to strings, except that the lexical syntax is constrained
by the source language, rather than being a free form text string.


### Prm: Primitive names
Primitive names are values that are predefined by the language implementation. They are
similar to Symbol names except that new ones cannot be introduced by the client program.


## Coverage of the name set

The six forms of names, Var, Def, Loc, Nom, Sym and Prm cover the useful combinations
of properties which we discuss l

```
value ground equality identity
X     X      X        X         Var
O     X      X        X         Def
X     O      X        X         invalid, ¬value ⇒ ¬ground
O     O      X        X         Loc
X     X      O        X         invalid, ¬value ⇒ ¬equality
O     X      O        X         [Incoherent Nom]
X     O      O        X         invalid, ¬value ⇒ ¬ground
O     O      O        X         Nom
X     X      X        O         invalid, ¬equality ⇒ ¬identity
O     X      X        O         invalid, ¬equality ⇒ ¬identity
X     O      X        O         invalid, ¬value ⇒ ¬ground
O     O      X        O         invalid, ¬equality ⇒ ¬identity
X     X      O        O         invalid, ¬value ⇒ ¬equality
O     X      O        O         [Incoherent Sym]
X     O      O        O         invalid, ¬value ⇒ ¬ground
O     O      O        O         Sym, Prm
```

The function of each form of name arises naturally from its properties.

Variables are names that are not values, so they must refer to something else to be useful.

Definitions are values that are not ground, and do not have equality. As they do not
have equality they must bind something else to be useful. However, as they are values
that can be returned during evaluation they must all be introduced at top level because
the local binding form would be unhygenic: !letdef @a = ... in @a ⇒ @a
This reduction form produces the name @a, but it is no longer in scope of the binder.

Location names are ground values that do not have equality. As they do not have equality
or identify they must be placeholders for some other value to be useful. As they are
ground values they are not unfolded automatically during evaluation, so there must
be some primitive to invoke the unfolding process (#get).

Nominal variables do have equality so they don't necessarily need to refer to something
else to be useful. Provided we can introduce new ones and compare them, we can use
them as fresh names. As they do not have identity we do not give them explicit names
in the source program, rather introduce them using a primitive (#fresh).

Symbols are atomic values that have identify. As they have identity we can give
them explicit names in the source program.

Primitives are like symbols except that the set of primitives is fixed by the implementation
As the set is fixed they are used to name built-in concepts in the implementation,
where new concepts cannot be added by the client program.

