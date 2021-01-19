#pragma once
#include <stdio.h>
#include <stdlib.h>
struct nodeee {
	int key;
	int val;
	struct nodeee*next;
};
struct table {
	int size;
	struct nodeee**list;
};
struct table *createTable(int size) {
	struct table *t = (struct table*)malloc(sizeof(struct table));
	t->size = size;
	t->list = (struct nodeee**)malloc(sizeof(struct nodeee*)*size);
	int i;
	for (i = 0; i < size; i++)
		t->list[i] = NULL;
	return t;
}
int hashCode(struct table *t, int key) {
	if (key < 0)
		return -(key%t->size);
	return key % t->size;
}
void insert(struct table *t, int key, int val) {
	int pos = hashCode(t, key);
	struct nodeee*list = t->list[pos];
	struct nodeee*newNode = (struct nodeee*)malloc(sizeof(struct nodeee));
	struct nodeee*temp = list;
	while (temp) {
		if (temp->key == key) {
			temp->val = val;
			return;
		}
		temp = temp->next;
	}
	newNode->key = key;
	newNode->val = val;
	newNode->next = list;
	t->list[pos] = newNode;
}
int lookup(struct table *t, int key) {
	int pos = hashCode(t, key);
	struct nodeee*list = t->list[pos];
	struct nodeee*temp = list;
	while (temp) {
		if (temp->key == key) {
			return temp->val;
		}
		temp = temp->next;
	}
	return -1;
}
int main() {
	struct table *t = createTable(5);
	insert(t, 2, 3);
	insert(t, 5, 4);
	printf("%d", lookup(t, 5));
	return 0;
}
