//
// Created by edgar on 13/11/20.
//

#ifndef PRACTICA_OBJECT_H
#define PRACTICA_OBJECT_H
using namespace std;
#include <cstring>
#include <string>

class object{

public:
    object();
    typedef struct {
        char version[3];
        int width;
        int height;
        int maxGrayLevel;
        int **imageData;
        int **gx;
        int **gy;
    } pgm;
    //Atributes for object class
    string name;
    char* image_out;
    string out_put_image;
    static char* image;

    //Mehtods for the object class
    void set_name(string name);
    string get_name();
    void set_image(char* image);char* get_image();
    void name_to_name();
    void set_output_image(string path);
    string get_output_image();

    static void apply_filter(int index);
    static void* create_thread(void *arg);


    static void read_pgm_file(char *name, pgm *image);

    static void read_comments(FILE *pFile);

    static void padding(pgm *ptr);

    static void init_out_image(pgm *ptr, pgm image);

    static void sobel_edge_detector(pgm *ptr, pgm *ptr1);

    static int convolution(pgm *image, int pInt[3][3], int i, int j);

    static void min_max_normalization(pgm *ptr, int **pInt);

    static void write_pgm_file(pgm *ptr, char *name, int **pInt, char string[]);
};

#endif //PRACTICA_OBJECT_H
