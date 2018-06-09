#include <iostream>
#include <stdlib.h>
#include "Buffer.hpp"

Buffer buffer;

void *producerA(void *)
{
    while(true)
    {

        sleep((rand()%7 +1)/2);
        buffer.produceA();
    }
}

void *producerB(void *)
{
    while(true)
    {

        sleep((rand()%7 +1)/2);
        buffer.produceB();
    }
}

void *consumerA(void *)
{
    while(true)
    {
        sleep((rand()%7 +1)/2);
        buffer.consumeA();
    }
}

void *consumerB(void *)
{
    while(true)
    {
      sleep((rand()%7 +1)/2);
        buffer.consumeB();
    }
}

int main()
{
    pthread_t prodA, prodB, consA, consB;

    srand((unsigned) time(NULL));

    pthread_create(&prodA, NULL, producerA, NULL);
    pthread_create(&prodB, NULL, producerB, NULL);
    pthread_create(&consA, NULL, consumerA, NULL);
    pthread_create(&consB, NULL, consumerB, NULL);

    pthread_join(prodA, NULL);
    pthread_join(prodB, NULL);
    pthread_join(consA, NULL);
    pthread_join(consB, NULL);

    return 0;
}
