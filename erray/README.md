erray expressions build an "expression tree", that enable lazy
evaluation of expressions upon assignment to an Erray eliminating temporaries
and repeated looping over Errays.
Erray is a contraction of expression-array.
An Erray is considered an erray expression.
Erray expressions support slicing through .slice() for rvalues and through
.window() for lvalues.
Errays can be printed through an overload of std::cout.
operations: +, -, *, /, pow(), %, , cross(), mm(), sqrt(), transpose(), reshape()


functions: sum(), min(), max(), avg(), swap(windows)
initialise: empty(), zeros() ones(), linspace(), enumerate(), random(),
random_int()
Erray operations/assignments are - where appropriate - compatible with
"scalars" i.e entities of the erray expression base type, T.
Errays expressions are optimised to work with trivial/base types however the
module handles arbitrary base types and can be used as an effective container
for any type.
All shape checking can be disabled through the "#define NDEBUG" macro.
All debug printing can be enabled through the "#define DEBUGCOUT" macro.