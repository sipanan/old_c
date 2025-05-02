/* 
 * File:   RingBuffer.h
 * Author: Quednau
 *
 * Created on 8. April 2014, 12:13
 */
#include <stdbool.h>
#include <stdint.h>

#ifndef RINGBUFFER_H
#define	RINGBUFFER_H

typedef struct FiFoQ {
uint16_t capacity;
int32_t buffer[2000];
uint16_t count;
uint16_t first;
} FIFO;

bool isFull(FIFO *q);
bool isEmpty(FIFO *q);
void setEmpty(FIFO *q);
int32_t dequeue(FIFO *q);
bool enqueue(FIFO *q,int32_t data);

#endif	/* RINGBUFFER_H */

