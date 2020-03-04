

```
Term  ::= mmmm Term+            '[' Term,* ']'

       |  mvar Name Nat         (like 'name^0')
       |  msym Name             (like '%name')
       |  mprm Name             (like '#name')

       |  mabs Name* Term       '{' Name* '}' Term

       |  mapv Term  Term       Term Term
       |  maps Term  Term*      Term '[' Term,* ']'

       |  mlet Binds Term       ':let' Binds ':in' Term
       |  mrec Binds Term       ':rec' Binds ':in' Term

Binds ::= Bind
       |  Bind ':and' Binds

Bind  ::= bind Name Term        Name '=' Term
```

```
@length
 = #case'list
        [ {} 0
        , {x xs} #nat'add [1, @length xs] ]
```


