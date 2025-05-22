CC = g++
FLAGS = -lpthread -I .
APP = KVstore
SRC = KVstore.cpp epoll_entry.cpp KVstore_array.cpp
&(APP): 
	$(CC) $(SRC) -o $(APP) $(FLAGS)
