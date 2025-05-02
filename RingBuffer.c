#include "RingBuffer.h"
bool isFull(FIFO *q)
{
    if (q->count == q->capacity)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void setEmpty(FIFO *q)
{
    int i ;
    q->count = 0;
    q->first = 0;
    q->capacity = (sizeof(q->buffer)/sizeof(uint32_t));
    for(i=0;i<=q->capacity;i++) q->buffer[i]=0;
}

bool isEmpty(FIFO *q)
{
    if (q->count == 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int32_t dequeue(FIFO *q)
{   
    uint32_t data = 0;
    if (! isEmpty(q))
    {
        data = q->buffer[ q->first ];
        q->first++;
        q->first %= q->capacity;
        q->count--;        
    }
    return data;
}

bool enqueue(FIFO *q,int32_t data)
{
    if(!isFull(q))
    {
        q->buffer[(q->first + q->count) % (q->capacity)] = data;
        q->count++;
        return true;
    }
    else
    {
        return false;
    }
}

