schnorr:
	swig -c++ -python lint.i
	g++ -fPIC -c lint.cpp
	g++ -fPIC -c lint_wrap.cxx -I/usr/include/python2.7
	g++ -shared lint.o lint_wrap.o -o _lint.so
	rm lint.o lint_wrap.cxx lint_wrap.o
