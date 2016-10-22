# Author: Radovan Sroka

CC=g++
CXX=g++
CXXFLAGS=-O2 -g -Wall -Wextra -pedantic -std=c++14
APP=appdetector

sources= $(APP).cpp entry.cpp entrytable.cpp
objs= $(APP).o entry.o entrytable.o

default: $(APP)

$(APP): $(objs)

$(APP).o: $(sources)

clean:
	rm $(APP) *.o
