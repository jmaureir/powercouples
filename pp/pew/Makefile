CXX=icc
SWIG=swig
SRC=pew.cpp
TARGETS=pew
FLAGS=-O2 -fPIC
PYTHON_INCLUDE=/home/apps/python/2.7.10/include/python2.7

all: $(TARGETS)

swig: $(TARGETS)
	$(SWIG) -c++ -python $<.i
	$(CXX) $(FLAGS) -c $<.cpp
	$(CXX) $(FLAGS) -c $<_wrap.cxx -I$(PYTHON_INCLUDE)
	$(CXX) -shared $<.o $<_wrap.o -o _$<.so
	touch __init__.py

$(SRC):
	$(CXX) $(FLAGS) -c $< 

clean:
	rm -rf pew.o
	rm -rf pew
	rm -rf pew_wrap.*
	rm *.py
	rm *.pyc
	rm -rf _pew.so
