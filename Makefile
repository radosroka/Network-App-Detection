# Author: Radovan Sroka

CC=g++
CXX=g++
CXXFLAGS=-O2 -Wall -Wextra -pedantic -std=c++14
APP=appdetector

default: $(APP)

$(APP): $(APP).o 

$APP).o: $(APP).cc

clean:
	rm $(APP) *.o
