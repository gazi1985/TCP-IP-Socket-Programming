#makefile
CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-g
LDFALGS=-g
LDLIBS=-ltest
AR=ar

SRCS=main.cc function.cc testobj.cc
OBJS=$(subst .cc,.o,$(SRCS))

all:main
main:$(OBJS) libtest.so
	$(CXX) $(LDFALGS) -o main $(OBJS) -L. $(LDLIBS)

main.o: main.cc function.h testobj.h
function.o:function.h function.cc

clean:
	$(RM) $(OBJS)
	
all-clean:clean
	$(RM) main
