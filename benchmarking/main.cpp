#include <iostream>
#include "cmake-build-debug/object.h"
using namespace std;
#include <string>
#include "png.h"
#include "pthread.h"
#include <time.h>
int main() {
    char image[] = "/home/edgar/Documentos/Principios_de_SO/Taller4_Lab4/brain_492.pgm";
    

    object list_of_objects[10000];
    int indexes[10000];

    for (int i = 0; i < 10000; i++){
        list_of_objects[i].set_name("image");
        indexes[i] = i;
    }

    pthread_t threads_list[10];

    //init time cpunter
    clock_t b_time;
    b_time = clock();

    for (int j = 0; j < 10; j++){
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&threads_list[j], &attr, list_of_objects[j].create_thread, &indexes[j]);

    }

    for (int j = 0; j < 10; j++){
        pthread_join(threads_list[j], NULL);
    }

    b_time = clock() - b_time;
    double tiempo_tomado = ((double)b_time)/CLOCKS_PER_SEC;
    printf("\033[1;31m El programa duró %f, con %i elementos.\033[0m; \n", tiempo_tomado, 10000);


    return 0;
}
