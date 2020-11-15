#include <iostream>
#include "cmake-build-debug/object.h"
using namespace std;
#include <string>
#include "png.h"
#include "pthread.h"
int main() {
    char image[] = "/home/edgar/Documentos/Principios_de_SO/Taller4_Lab4/brain_492.pgm";
    /*
    object Imagen1;
    Imagen1.set_image(image);
    Imagen1.set_name("asd");
    Imagen1.apply_filter(0);*/

    object list_of_objects[10000];
    int indexes[10000];

    for (int i = 0; i < 10000; i++){
        list_of_objects[i].set_name("image");
        indexes[i] = i;
        //list_of_objects[i].apply_filter(i);
    }

    pthread_t threads_list[10000];

    for (int j = 0; j < 10000; j++){
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_create(&threads_list[j], &attr, list_of_objects[j].create_thread, &indexes[j]);

    }

    for (int j = 0; j < 10000; j++){
        pthread_join(threads_list[j], NULL);
    }

    return 0;
}
