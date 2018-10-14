#include <cstdio>
#include <cstdlib>
#include <pthread.h>

int thread_count;

void* Hello(void* rank)
{
    long long my_rank = (long long) rank;
    printf("Hello from thread %lld of %d\n", my_rank,thread_count);
    return NULL;
}

int main(int agrc, char* agrv[])
{
    long long thread;
    pthread_t* thread_handles;

    thread_count = strtoll(agrv[1],NULL,10);
    thread_handles = (pthread_t *)malloc(thread_count*sizeof(pthread_t));

    for(thread = 0; thread < thread_count; thread++)
    {
        pthread_create(&thread_handles[thread],NULL,Hello, (void*) thread);
    }

    printf("Hello from the main thread\n");

    for(thread = 0; thread < thread_count; thread++)
    {
        pthread_join(thread_handles[thread], NULL);
    }

    printf("END\n");

    free(thread_handles);
    return 0;
}