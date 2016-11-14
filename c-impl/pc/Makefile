CXX=icc
SWIG=swig
SRC=find.cpp
TARGETS=find
GMP_INC=-lgmp -I$$HOME/include -L$$HOME/lib
CFLAGS=-O2 -fPIC -std=c++11 -qopenmp $(GMP_INC)
CPPFLAGS=-O2 -fPIC -std=c++11 -qopenmp $(GMP_INC)
PYTHON_INCLUDE=/home/apps/python/2.7.10/include/python2.7

all: $(TARGETS)

swig: $(TARGETS)
	$(SWIG) -c++ -python $<.i
	$(CXX) $(CFLAGS) -c $<.cpp
	$(CXX) $(CFLAGS) -c $<_wrap.cxx -I$(PYTHON_INCLUDE)
	$(CXX) -shared -qopenmp $(GMP_INC) $<.o $<_wrap.o -o _$<.so
	touch __init__.py

clean:
	rm -rf find.o
	rm -rf find
	rm -rf find_wrap.*
	rm *.py
	rm *.pyc
	rm -rf _find.so
