#include "epoll_entry.h"

#define MAX_TOKENS 128
const char* commands[] = {
	"GET",
	"SET",
	"DEL",
	"MOD"
};

void* kvs_malloc(size_t size) {
	void* ptr = malloc(size);
	memset(ptr, 0, size);
	return ptr;
}

void kvs_free(void* ptr) {
	free(ptr);
}
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
	else {
		snprintf(msg, BUFFER_SIZE, "UNKNOWN COMMAND");
	}
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

int main() {
    epoll_entry();
    return 0;
}
