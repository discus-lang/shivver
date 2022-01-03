
# Related Work

## Typed Self Interpretation by Pattern Matching
Jay, Palsberg, ICFP 2011.
Presents a typed self interpreter based on an extended factorisation calculus.
This is a combinator system with operators Y, K, S, F, E, B.
Y - fixedpoint,
K - constant
S - substitution
F - factorization
E - operator equality
B - block reduction.

Paper defines a self recognizer and self enactor by desugaring a simple functional
language into combinators. The combinators can then be assigned System-F types,
and meaning the self interpreter (enactor) is well typed in this system.

Paper highlights the fact that factorization should only apply to terms
where reduction is blocked, otherwise reduction is not confluent.

Evaluation is presented as a set of rewrite rules that can apply at any point
of the term. An explicit evaluation order is not defined. To apply the self-enactor
the input program must be quoted which deeply adds the B (blocking) operator
to every node in the tree.

The version of System-F uses implicit instantiation. There is a complication with
typing of the E operator but this is dodged in the implementation as it is only
used as a pattern and the program can be rewritten to avoid mentioning it --
recognising the operator using the default case in pattern matching.

The language admits a simple function 'equal' that can decide equality of arbitrary
closed terms. There is also an unquote function that recursively strips the B
tags from a quoted term.


