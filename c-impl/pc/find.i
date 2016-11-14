%module find
%{
#include "find.h"
%}

%include "std_vector.i"
namespace std {
   %template(IntVector) vector<int>;
}

%include "find.h"
