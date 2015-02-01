%module lint
%{
/* Includes the header in the wrapper code */
#include "lint.h"
%}

%include "std_vector.i"
%include "std_string.i"

/* Parse the header file to generate wrappers */
%include "lint.h"
