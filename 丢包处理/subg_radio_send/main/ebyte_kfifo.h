#include <stdio.h>
#include <stdlib.h>
#include "esp_system.h"

#define EBYTE_MAX_SIZE 256

typedef struct {
    uint8_t data[EBYTE_MAX_SIZE];
    int front;
    int rear;
    int size;
} Ebyte_FIFO_t;

void Ebyte_FIFO_Init(Ebyte_FIFO_t* queue) {
    queue->front = -1;
    queue->rear = -1;
    queue->size = 0;
}

bool Ebyte_FIFO_IsEmpty(Ebyte_FIFO_t* queue) {
    return queue->size == 0;
}

bool Ebyte_FIFO_IsFull(Ebyte_FIFO_t* queue) {
    return queue->size == EBYTE_MAX_SIZE;
}

int Ebyte_FIFO_Write(Ebyte_FIFO_t* queue, uint8_t* pData, int len) {
    if (Ebyte_FIFO_IsEmpty(queue)) queue->front = 0;
    int count = 0;

    for(int i = 0; i < len; ++i){
        if (Ebyte_FIFO_IsFull(queue)) {
            break;
        }

        queue->rear = (queue->rear + 1) % EBYTE_MAX_SIZE;
        queue->data[queue->rear] = pData[i];
        queue->size++;
        count++;
    }
    return count;
}

int Ebyte_FIFO_Read(Ebyte_FIFO_t* queue, uint8_t* pData, int len) {
    int count = 0;

    for (int i = 0; i < len; i++) {
        if (Ebyte_FIFO_IsEmpty(queue)) {
            break;
        }

        pData[i] = queue->data[queue->front];
        queue->front = (queue->front + 1) % EBYTE_MAX_SIZE;
        queue->size--;
        count++;
    }

    return count;
}
