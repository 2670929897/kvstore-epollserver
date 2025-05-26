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

inline void* kvs_malloc(size_t size);
inline void kvs_free(void* ptr);

#define KVSTORE_ARRAY 1
#define KVSTORE_RBTREE 1

#if KVSTORE_ARRAY

struct kv_array_item {
	char* key;
	char* value;
};
#define KV_ARRAY_SIZE 1024

int kvstore_array_set(char* key, char* value);
char* kvstore_array_get(char* key);
int kvstore_array_del(char* key);
int kvstore_array_mod(char* key, char* value);


#endif // KVstore_array

#if KVSTORE_RBTREE


typedef struct _rbtree rbtree_t;

extern rbtree_t Tree;

int kvstore_rbtree_create(rbtree_t* tree);
void kvstore_rbtree_destory(rbtree_t* tree);
int kvs_rbtree_set(rbtree_t* tree, char* key, char* value);
char* kvs_rbtree_get(rbtree_t* tree, char* key);
int kvs_rbtree_delete(rbtree_t* tree, char* key);
int kvs_rbtree_modify(rbtree_t* tree, char* key, char* value);
int kvs_rbtree_count(rbtree_t* tree);


#endif // KVstore_rbtree


#endif // EPOLL_ENTRY_H