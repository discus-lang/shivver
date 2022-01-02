## Evaluation of Primitives

### Heap
```
(PhMVar)        S ⊢ #heap'mvar ; N ⇒ S.heap[Xl m↦ N] | #unit     Xl fresh

(PhIVar)        S ⊢ #heap'ivar ; - ⇒ S.heap[Xl i↦ -] | #unit     Xl fresh

(PhGet)         S.heap[Xl _↦ N] ⊢ #heap'get ; Xl ⇒ S | N

(PhSetMVar)     S.heap[Xl m↦ N] ⊢ #heap'set ; Xl N' | S.heap[Xl m↦ N'] | #unit

(PhSetIVar)     S.heap[Xl i↦ _] ⊢ #heap'set ; Xl N' | S.heap[Xl i↦ N'] | #unit
```


### Nominal Variables

```
                E₂ = E₁, bindn Xn N
(PiNomSub)     --------------------------------------------------------
                S ↦ #nom'sub ; {nnom Xn, N, nbox E₁ M} ⇒ S | nbox E₂ M
```


### Introspection
```
                N₁ =
                Xnⁿ fresh
                E₃ = E₂, X'₁ ↦ nval (vlst Xnⁿ),
                         X'₂ ↦ nbox E₁ {Xv.i ↦ Xn.i | i←1..n}
(PiMatchAbs)   ------------------------------------------------------------------
                S ⊢ #match'abs ; (vclo n E₁ Xvⁿ M₁) (vclo 2 E₂ {X'₁, X'₂} M₂) N
              ⇒ S |  nbox E₃ M₂

                E₃ = E₁, X₁ ↦ nbox E₁ M₁, X₂ ↦ nbox E₁ M₂
(PiMatchApp)   ------------------------------------------------------------------------
                S ⊢ #match'app ; mnrm (nbox E₁ (mapp M₁ M₂)) (vclo 2 E₂ {X₁ X₂} M₃) N
              ⇒ S |  nbox E₃ M₃

(PiMatchBox)    S ⊢ #match'box ; mnrm (nbox E₁ (nbox E₂ M)) N ⇒ S | nnrm (nbox (E₁, E₂) M)
```



```
 #match'abs
  [ {x} #nat'add [x, x]
  , {ns xx} #nom'sub [#list'index 0 ns, 5, {x} xx] ]
  | #false
```
