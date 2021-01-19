#pragma once
#include <stdio.h>
#include <stdlib.h>

typedef struct node {
	struct node* next;
	int id_process;

}node_t;

typedef struct queue {
	node_t* head;
	node_t* tail;
}queue_t;

node_t* create_node(int value) {
	node_t* node = (node_t*)malloc(sizeof(node_t));
	node->next = NULL;
	node->id_process = value;
	return node;
}

queue_t* create_queue() {
	queue_t* queue = (queue_t*)malloc(sizeof(queue_t));
	queue->head = NULL;
	queue->tail = NULL;
	return queue;
}

void enqueue(queue_t* queue, int value) {
	node_t* node = create_node(value);
	if (queue->head == NULL) {
		queue->head = node;
		queue->tail = node;
		return;
	}

	queue->head->next = node;
	queue->head = node;
}

int dequeue(queue_t* queue) {
	if (queue->head == NULL || queue->tail == NULL) {
		return INT_MIN;
	}

	if (queue->head == queue->tail) {
		queue->head = NULL;
	}

	node_t* node = queue->tail;
	queue->tail = node->next;
	int temp = node->id_process;
	free(node);
	return temp;
}

void print_queue(queue_t* queue) {
	printf("[");
	while (queue->tail != NULL) {
		int temp = dequeue(queue);
		if (temp != INT_MIN) {
			printf("%d", temp);
		}
	}
	printf("]");
}
