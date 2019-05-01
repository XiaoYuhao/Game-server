###########################
#	Makefile
###########################

#source object target
SOURCE 	:=
OBJS 	:= Game.o test.o
TARGET	:= main  

#compile and lib parameter
CC		:= g++
LIBS	:= -lm
LDFLAGS	:= -L.
DEFINES	:=
INCLUDE := 
CFLAGS	:=
CXXFLAGS:=

.PHONY: clean

#link parameter
LIB := 

#link
$(TARGET):${OBJS}
	$(CC)  -o test test.o Game.o ${LIBS}
	
#clean
clean:
	rm -fr *o
	rm -fr test
