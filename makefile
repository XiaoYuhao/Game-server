###########################
#	Makefile
###########################

#source object target
SOURCE 	:= tools.cpp Database.cpp test.cpp md5.cpp Game.cpp
OBJS 	:= Database.o test.o
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
$(TARGET):
	$(CC)  -c md5.cpp -std=gnu++11 -I/usr/include/mysql -o md5.o
	$(CC)  -c tools.cpp -std=gnu++11 -I/usr/include/mysql -o tools.o
	$(CC)  -c Game.cpp -std=gnu++11 -I/usr/include/mysql -o Game.o
	$(CC)  -c Database.cpp -std=gnu++11 -I/usr/include/mysql -o Database.o
	$(CC)  -c Server.cpp -std=gnu++11 -I/usr/include/mysql -o Server.o
	$(CC)  -c test.cpp -std=gnu++11 -I/usr/include/mysql -o test.o
	$(CC)  -o test -std=gnu++11 -I/usr/include/mysql -L/usr/lib64/mysql -L/usr/lib64/mysql -lmysqlclient -lpthread -lz -lm -ldl -lssl -lcrypto Database.o test.o tools.o md5.o Server.o Game.o
	
#clean
clean:
	rm -fr *o
	rm -fr test
	rm -fr *.log
