###########################
#	Makefile
###########################

#source object target
SOURCE 	:= tools.cpp Database.cpp main.cpp md5.cpp Game.cpp
OBJS 	:= md5.o tools.o Game.o Database.o Server.o
TARGET	:= main  

#compile and lib parameter
CXX		:= g++
LIBS	:= -lm
LDFLAGS	:= -L/usr/lib64/mysql -lmysqlclient -lpthread -lz -lm -ldl -lssl -lcrypto
DEFINES	:=
CFLAGS	:=
CXXFLAGS:= -Wall -std=gnu++11 -I/usr/include/mysql
 
.PHONY: clean

#link parameter
LIB := 

#link
$(TARGET): $(OBJS) main.cpp
	$(CXX) -o $@ $(CXXFLAGS) $(LDFLAGS) $^
	
$(OBJS): %.o: %.cpp %.h
Server.o:OnlineClient.h package.h Database.h tools.h md5.h Game.h


#clean
clean:
	rm -fr *.o
	rm -fr main
	rm -fr *.log
