#!/bin/sh

find ./ -type f -name '*.cpp' -exec sed -i 's/\(YGGR_PP_ANY_VAL_REGISTER_MUTABLE_BINARY_OPERATOR\)(pmgr, \(plus_set\), bool,/\1_UNSAFE(pmgr, \2, bool,/g' {} \;
find ./ -type f -name '*.cpp' -exec sed -i 's/\(YGGR_PP_ANY_VAL_REGISTER_MUTABLE_BINARY_OPERATOR\)(pmgr, \(plus_set\),\(.*\), bool)/\1_UNSAFE(pmgr, \2,\3, bool)/g' {} \;

#YGGR_PP_ANY_VAL_REGISTER_MUTABLE_BINARY_OPERATOR(pmgr, plus_set, bool, bool)

#echo 'defined(a, b, arg0, d)' | sed 's/\(defined\)(a, \(b\),\(.*\), d)/\1_aaa(a, \2,\3, d)/g'