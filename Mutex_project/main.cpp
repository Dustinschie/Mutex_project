//
//  main.cpp
//  Semaphore_project
//
//  Created by Dustin on 10/25/13.
//
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <iostream>

using namespace std;

string  buffer = "";
pthread_mutex_t pthread_mutex_id;


// prototype the threads functions
void * parent(void * input);
void * child(void * input);
long fibonacci(long number_of_iterations);

int main(int argc, const char * argv[])
{   /** initilization of mutex */
    int result = pthread_mutex_init(&pthread_mutex_id, NULL), number_of_iterations = 20;
    pthread_t pthread_parent_id, pthread_child_id;
    
    /** Creation of the parent thread */
    result = pthread_create(&pthread_parent_id,NULL, parent, (void *) (long)number_of_iterations);
    
    if (!result) printf("<parent thread created>\n");
    
    /** Creation of the child thread */
    result = pthread_create(&pthread_child_id,NULL, child,(void *) (long)number_of_iterations);
    if (!result) printf("<child thread created>\n");
    
    
    /** Joining of the parent thread */
    result = pthread_join(pthread_parent_id, NULL);
    if (!result) printf("<parent thread joined>\n");
    
    /** Joining of the child thread */
    result = pthread_join(pthread_child_id, NULL);
    if (!result) printf("<child thread joined>\n\n\n");
    
    
    pthread_mutex_destroy(&pthread_mutex_id);
    
    return 0;
}

void * parent(void * input)
{   long i = (long)input;
    while (i > 0)
    {   if(buffer.size() != 0) continue;
        /** locking a mutex */
        int r = pthread_mutex_lock(&pthread_mutex_id);
        printf("\n\t<parent lock>\n");
        
        /** Critical Sections **/
        buffer = to_string((long)input - i + 1);
        buffer.append(")\t");
        buffer.append(to_string(fibonacci((long)input - i)));
        
        /** unlocking a mutex */
        printf("\t<parent unlock>\n");
        r = pthread_mutex_unlock(&pthread_mutex_id);
        i--;
    }
    return NULL;
}

void * child(void * input)
{   long i = (long)input;
    while (i > 0)
    { if (buffer.size() == 0) continue;
        /** locking a mutex */
        int r = pthread_mutex_lock(&pthread_mutex_id);
        printf("\n\t<mutex locked by child>\n");
        
        /** Critical Sections **/
        printf("\t\t%s\t\n",buffer.c_str());
        buffer = "";
        
        /** unlocking a mutex */
        printf("\t<mutex unlocked by child>\n");
        r = pthread_mutex_unlock(&pthread_mutex_id);
        i--;
    }
    return NULL;
}
//  Just something to run
long fibonacci(long number_of_iterations)
{   if (number_of_iterations == 1) return 1;
    if (number_of_iterations == 0) return 0;
    return fibonacci(number_of_iterations - 1) + fibonacci(number_of_iterations - 2);
}
