# Author: Radovan Sroka

CC=g++
CXX=g++
CXXFLAGS=-O2 -Wall -Wextra -pedantic -std=c++14
APP=appdetector

sources= $(APP).cpp parsedata.cpp
objs= $(APP).o parsedata.o

default: $(APP)

$(APP): $(objs)

$APP).o: $(sources)

clean:
	rm $(APP) *.o
