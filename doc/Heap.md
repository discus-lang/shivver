
# Heap Objects

## Atomics

### Atomic Names
The atomic name forms var, sym, prz, and mac have the same format as the general NameA object.
```
NameA   ::= { str: Seq Word8, aux: Word24 }                     name
VarA    ::= { str: Seq Word8, bump: Word2 4}                    name
SymA    ::= { str: Seq Word8 }                                  %name
PrzA    ::= { tag: Word32 }                                     #name
MacA    ::= { str: Seq Word8 }                                  @name
```

### Atomic Values
```
BllA    ::= { val: Word1 }                                      #true | #false
NatA    ::= { nat: Word54 }                                     NAT
```


## Hot Object
Hot objects are stored in the active heap.

```
MmmH    ::= { exps: Seq Exp }                                   [Term₁, …, Termₙ]
AbsH    ::= { params: Seq Var, body: Exp }                      {Var₁,  …, Varₙ} Exp
AppH    ::= { head: Exp, elim: Exp }                            Exp Exp
ApsH    ::= { head: Exp, elims: Seq Var }                       Exp [Exp₁, …, Expₙ]
```

```
CloH    ::= { env: Env, parms: Exp, body: Exp }                 &clo Env {Var₁, …, Varₙ} Exp
EnvH    ::= { parent: Env | 0, parms: Exp, vals: Seq Val }      &env (Var₁, …, Varₙ) (Val₁, …, Valₙ)
```

