#include "epoll_entry.h"

#define MAX_TOKENS 128
const char* commands[] = {
	"GET",
	"SET",
	"DEL",
	"MOD",
	"RGET",
	"RSET",
	"RDEL",
	"RMOD"
};

void* kvs_malloc(size_t size) {
	void* ptr = malloc(size);
	memset(ptr, 0, size);
	return ptr;
}

void kvs_free(void* ptr) {
	free(ptr);
}

#if KVSTORE_RBTREE
int kvstore_rbtree_set(char* key, char* value) {
	return kvs_rbtree_set(&Tree, key, value);
}
char* kvstore_rbtree_get(char* key) {
	return kvs_rbtree_get(&Tree, key);
}
int kvstore_rbtree_del(char* key) {
	return kvs_rbtree_del(&Tree, key);
}
int kvstore_rbtree_mod(char* key, char* value) {
	return kvs_rbtree_mod(&Tree, key, value);
}

#endif

using namespace std;

int kvstore_parser_protocol(struct conn_item* item, char** tokens, int count) {
	if (tokens == NULL || count <= 0) return -1;


	char* msg = item->wbuffer;
	char* key = tokens[1];
	char* value = tokens[2];
	memset(msg, 0, BUFFER_SIZE);

	if (strcmp(tokens[0], commands[0]) == 0) {
		char* val = kvstore_array_get(key);
		if (val != NULL) {
			snprintf(msg, BUFFER_SIZE, "%s", val);
		}
		else {
			snprintf(msg, BUFFER_SIZE, "NO EXIST");
		}
	}
	else if (strcmp(tokens[0], commands[1]) == 0) {
		int res = kvstore_array_set(key, value);
		if (!res) {
			snprintf(msg, BUFFER_SIZE, "SUCCESS");
		}
		else {
			snprintf(msg, BUFFER_SIZE, "FAILED");
		}
	}
	else if (strcmp(tokens[0], commands[2]) == 0) {
		int res = kvstore_array_del(key);
		if (res == 0) {
			snprintf(msg, BUFFER_SIZE, "%s","SUCCESS");
		}
		else if (res > 0) {
			snprintf(msg, BUFFER_SIZE, "NO EXIST");
		}
		else {
			snprintf(msg, BUFFER_SIZE, "%s","ERROR");
		}
	}
	else if (strcmp(tokens[0], commands[3]) == 0) {
		int res = kvstore_array_mod(key, value);
		if (res == 0) {
			snprintf(msg, BUFFER_SIZE, "%s","SUCCESS");
		}
		else if (res > 0) {
			snprintf(msg, BUFFER_SIZE, "NO EXIST");
		}
		else {
			snprintf(msg, BUFFER_SIZE, "%s", "ERROR");
		}
	}
	else if (strcmp(tokens[0], commands[4]) == 0) {
		char* val = kvstore_rbtree_get(key);
		if (val != NULL) {
			snprintf(msg, BUFFER_SIZE, "%s",val);
		}
		else {
			snprintf(msg, BUFFER_SIZE, "RNO EXIST");
		}
	}
	else if (strcmp(tokens[0], commands[5]) == 0) {
		int res = kvstore_rbtree_set(key, value);
		if (!res) {
			snprintf(msg, BUFFER_SIZE, "RSUCCESS");
		}
		else {
			snprintf(msg, BUFFER_SIZE, "RFAILED");
		}
	}
	else if (strcmp(tokens[0], commands[6]) == 0) {
		int res = kvstore_rbtree_del(key);
		if (res == 0) {
			snprintf(msg, BUFFER_SIZE, "%s", "RSUCCESS");
		}
		else if (res > 0) {
			snprintf(msg, BUFFER_SIZE, "RNO EXIST");
		}
		else {
			snprintf(msg, BUFFER_SIZE, "%s", "RERROR");
		}
	}
	else if (strcmp(tokens[0], commands[7]) == 0) {
		int res = kvstore_rbtree_mod(key, value);
		if (res == 0) {
			snprintf(msg, BUFFER_SIZE, "%s", "RSUCCESS");
		}
		else if (res > 0) {
			snprintf(msg, BUFFER_SIZE, "RNO EXIST");
		}
		else {
			snprintf(msg, BUFFER_SIZE, "%s", "RERROR");
		}
	}

	else {
		snprintf(msg, BUFFER_SIZE, "UNKNOWN COMMAND");
	}
	return 0;
}

int kvstore_split_token(char* msg, char** tokens) {
	if (msg == NULL || tokens == NULL) return -1;

	int idx = 0;

	char* token = strtok(msg, " ");

	while (token != NULL) {
		tokens[idx++] = token;
		token = strtok(NULL, " ");
	}
	return idx;
}	

int kvstore_request(struct conn_item *item){

	char* msg = item->rbuffer;
	char* tokens[MAX_TOKENS];
	int count = kvstore_split_token(msg, tokens);

	
	kvstore_parser_protocol(item, tokens, count);

	return 0;
}


int init_kvengine(void) {

#if KVSTORE_ARRAY

#endif // KVstore_array

#if KVSTORE_RBTREE
	kvstore_rbtree_create(&Tree);
#endif // KVstore_rbtree
	return 0;
}

int exit_kvengine(void) {
#if KVSTORE_ARRAY

#endif

#if KVSTORE_RBTREE
	kvstore_rbtree_destory(&Tree);
#endif
}

int main() {

	init_kvengine();
    epoll_entry();
	exit_kvengine();
    return 0;
}
