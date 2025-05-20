// epoll_entry.h
#ifndef EPOLL_ENTRY_H

#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <netdb.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/epoll.h>
#include <bits/stdc++.h>
#define EPOLL_ENTRY_H
#define BUFFER_SIZE 1024
typedef int (*RCALLBACK)(int fd);

struct conn_item{
    int fd;
    /*int idx;
    char buffer[BUFFER_SIZE];*/
	char rbuffer[BUFFER_SIZE];
    int rlen;
	char wbuffer[BUFFER_SIZE];
    int wlen;

    RCALLBACK accept_callback;
    RCALLBACK recv_callback;
    RCALLBACK send_callback;
};

int kvstore_parser_protocol(struct conn_item* item, char** tokens, int count);
int kvstore_split_token(char* msg, char** tokens);
int kvstore_request(struct conn_item* item);
int epoll_entry(void);
void* kvs_malloc(size_t size);
void kvs_free(void* ptr);

#define KVstore_array 1

#if KVstore_array

struct kv_array_item {
	char* key;
	char* value;
};
#define KV_ARRAY_SIZE 1024

int kvstore_array_set(char* key, char* value);
char* kvstore_array_get(char* key);

#endif // KVstore_array




#endif // EPOLL_ENTRY_H