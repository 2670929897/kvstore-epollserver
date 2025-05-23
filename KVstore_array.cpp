#include "epoll_entry.h"

struct kv_array_item array_table[KV_ARRAY_SIZE] = { 0 };
int array_idx = 0;

int kvstore_array_set(char* key, char* value) {

	if (key == NULL || value == NULL || array_idx == KV_ARRAY_SIZE) return -1;
	printf("set key: %s, value: %s", key, value);
	char* kcopy = (char*)kvs_malloc(strlen(key) + 1);
	if (kcopy == NULL) return -1;
	strcpy(kcopy, key);

	char* vcopy = (char*)kvs_malloc(strlen(value) + 1);
	if (vcopy == NULL) {
		kvs_free(kcopy);
		return -1;
	}
	strcpy(vcopy, value);

	array_table[array_idx].key = kcopy;
	array_table[array_idx].value = vcopy;
	array_idx++;
	return 0;

}

char* kvstore_array_get(char* key) {

	if (key == NULL) return NULL;
	printf("get key: %s", key);
	int i = 0;
	for (i = 0; i < array_idx; i++) {
		if (strcmp(array_table[i].key, key) == 0) {
			return array_table[i].value;
		}
	}
	return NULL;
}

int kvstore_array_del(char* key) {

	printf("del key: %s", key);
	int i = 0;
	if (key == NULL) return -1;
	for (i = 0; i < array_idx; i++) {
		if (strcmp(array_table[i].key, key) == 0) {
			kvs_free(array_table[i].key);
			array_table[i].key = NULL;

			kvs_free(array_table[i].value);
			array_table[i].value = NULL;
			return 0;
		}
	}
	return i;
}

int kvstore_array_mod(char* key, char* value) {

	printf("mod key: %s,value:%s", key, value);

	if (key == NULL || value == NULL) return -1;

	int i = 0;
	for (i = 0; i < array_idx; i++) {
		if (strcmp(array_table[i].key, key) == 0) {

			kvs_free(array_table[i].value);
			array_table[i].value = NULL;

			char* vcopy = (char*)kvs_malloc(strlen(value) + 1);
			if (vcopy == NULL) {
				return -1;
			}
			strcpy(vcopy, value);
			array_table[i].value = vcopy;
			return 0;
		}
	}
	return i;
}