

// Term ::= mvar Bound
//       |  mcon Sym   Term
//       |  mapp Term  Term
//       |  mabs Bind+ Term
//       |  mmmm Term+

// Objects
//  aVarS  variable with statically allocated name
//
//         7  6  5  4  3  2  1  0
//  aVarS  0  .  .  .  bump     F       0 0 0 0 0 0 0 1


// ----------------------------------------------------------------------------
// Object tags.
//
//              7 6 5 4 3 2 1 0
// aSymT        0 0 0 0 0 0 0 1
// aVarT        0 0 0 0 0 1 0 1



