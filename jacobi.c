/* Joe Gildner
 * Joey Xiong
 * CSCI 347
 * 12/02/2018
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define THREAD_COUNT 96

pthread_t thread_ids[THREAD_COUNT];

int results[THREAD_COUNT];

void* mathstuff(void* args);

int main(int argc, char* argv[]){

  for(int i=0; i<THREAD_COUNT; i++){
    results[i] = 0;
    if(pthread_create(&(thread_ids[i]), NULL, &mathstuff,NULL) != 0){
      perror("pthread");
    }else{
      printf("Created thread %d successfully\n",i);
    }
  }

  for(int i=0; i<THREAD_COUNT; i++){
    pthread_join(thread_ids[i],NULL);
  }

  for(int i=0; i<THREAD_COUNT; i++){
    printf("%d\n",results[i]);
  }

}

void* mathstuff(void* args){
  pthread_t thisthread = pthread_self();

  printf("%ld",thisthread);
  // for(int i=0; i<THREAD_COUNT; i++){
  //   if(pthread_equal(thisthread,thread_ids[i])){
  //     results[i] = i;
  //   }
  // }

  return NULL;
}
