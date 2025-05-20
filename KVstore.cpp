#include "epoll_entry.h"

#define MAX_TOKENS 128
const char* commands[] = {
	"GET",
	"SET",
	"DEL",
	"MOD"
};

void* kvs_malloc(size_t size) {
	return malloc(size);
}

void kvs_free(void* ptr) {
	free(ptr);
}
using namespace std;

int kvstore_parser_protocol(struct conn_item* item, char** tokens, int count) {
	if (tokens == NULL || count <= 0) return -1;

	cout << tokens[0] << endl;
	if (strcmp(tokens[0], commands[0]) == 0) {
		char* value = kvstore_array_get(tokens[1]);
		if (value != NULL) {
			cout << "GET OK" << endl;
			cout << value << endl;
		}
		else {
			cout << "GET ERROR" << endl;
		}
	}
	else if (strcmp(tokens[0], commands[1]) == 0) {
		int res = kvstore_array_set(tokens[1], tokens[2]);
		if (!res) {
			cout << "SET OK" << endl;
		}
		else {
			cout << "SET ERROR" << endl;
		}
	}
	else if (strcmp(tokens[0], commands[2]) == 0) {
	}
	else if (strcmp(tokens[0], commands[3]) == 0) {
	}
	else {
	}
}

int kvstore_split_token(char* msg, char** tokens) {
	if (msg == NULL || tokens == NULL) return -1;

	char* token = strtok(msg, " ");
	int idx = 0;
	while (token != NULL) {
		tokens[idx++] = token;
		token = strtok(NULL, " ");
	}
	return idx;
}	

int kvstore_request(struct conn_item *item){

	char* msg = item->rbuffer;
	char* tokens[MAX_TOKENS];
    //cout << "recv " << item->fd << ' ' << item->buffer << endl;
	int count = kvstore_split_token(msg, tokens);

	/*for (int i = 0; i < count; i++) {
		cout << tokens[i] << endl;
	}*/
	kvstore_parser_protocol(item, tokens, count);

	return 0;
}

int main() {
    epoll_entry();
    return 0;
}
