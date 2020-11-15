//
// Created by edgar on 13/11/20.
//


#include <cstring>
#include <string>
#include "cmake-build-debug/object.h"
#include <cstdio>
#include <cmath>
#include <cstdlib>
//sobel filter by //https://github.com/fzehracetin/sobel-edge-detection-in-c
using namespace std;
// If there is some default initial values goes here:
/*
object::object() {
    string image = "file/hola.png";
    string name = "hola";
}*/
object::object() = default;

//Implementing the methods:

void object::set_name(string name) {
    this->name = name;
}
string object::get_name() {
    return this->name;
}

void object::set_output_image(string path){
    this->out_put_image = path;
}
string object::get_output_image(){
    return this->out_put_image;
}
void object::apply_filter(int index) {
    pgm image, out_image;
    //char* file_name = get_image();
    char file_name[] = "/home/edgar/Documentos/Principios_de_SO/Taller4_Lab4/brain_492.pgm";
    read_pgm_file(file_name, &image);
    padding(&image);
    init_out_image(&out_image, image);
    sobel_edge_detector(&image, &out_image);
    min_max_normalization(&out_image, out_image.imageData);
    min_max_normalization(&out_image, out_image.gx);
    min_max_normalization(&out_image, out_image.gy);
    char number[100];
    sprintf(number,"%i", index);
    strcat(number, ".pgm");
    char file_name_2[] = "/home/edgar/Documentos/Principios_de_SO/Taller4_Lab4/images/brain_492.pgm";
    write_pgm_file(&out_image, file_name_2, out_image.imageData, number);
    strcpy(number, " ");
    //write_pgm_file(&out_image, file_name, out_image.gx, "1.pgm");
    //write_pgm_file(&out_image, file_name, out_image.gy, "2.pgm");
    free(image.imageData);
    free(out_image.imageData);
    //free(out_image.gx);
    //free(out_image.gy);
}

void object::read_pgm_file(char *name, object::pgm *image) {
    FILE* input_image;
    int i, j, num;

    input_image = fopen(name, "rb");
    if (input_image == nullptr) {
        printf("File could not opened!");
        return;
    }

    fgets(image->version, sizeof(image->version), input_image);
    read_comments(input_image);

    fscanf(input_image, "%d %d %d", &image->width, &image->height, &image->maxGrayLevel);

    image->imageData = (int**) calloc(image->height, sizeof(int*));
    for(i = 0; i < image->height; i++) {
        image->imageData[i] = (int *)malloc(sizeof(int)*image->width);

    }

    if (strcmp(image->version, "P2") == 0) {
        for (i = 0; i < image->height; i++) {
            for (j = 0; j < image->width; j++) {
                fscanf(input_image, "%d", &num);
                image->imageData[i][j] = num;
            }
        }
    }
    else if (strcmp(image->version, "P5") == 0) {
        char *buffer;
        int buffer_size = image->height * image->width;
        buffer = (char*) malloc( ( buffer_size + 1) * sizeof(char));

        if(buffer == nullptr) {
            printf("Can not allocate memory for buffer! \n");
            return;
        }
        fread(buffer, sizeof(char), image->width * image-> height, input_image);
        for (i = 0; i < image->height * image ->width; i++) {
            image->imageData[i / (image->width)][i % image->width] = buffer[i];
        }
        free(buffer);
    }
    fclose(input_image);
    printf("_______________IMAGE INFO__________________\n");
    printf("Version: %s \nWidth: %d \nHeight: %d \nMaximum Gray Level: %d \n", image->version, image->width, image->height, image->maxGrayLevel);
}

void object::read_comments(FILE *input_image) {
    char ch;
    char line[100];

    while ((ch = fgetc(input_image)) != EOF && (isspace(ch)))  {
        ;
    }
    if (ch == '#') {
        fgets(line, sizeof(line), input_image);
    }
    else {
        fseek(input_image, -2L, SEEK_CUR);
    }
}

void object::padding(object::pgm *image) {
    int i;
    for (i = 0; i < image->width; i++) {
        image->imageData[0][i] = 0;
        image->imageData[image->height - 1][i] = 0;
    }

    for (i = 0; i < image->height; i++) {
        image->imageData[i][0] = 0;
        image->imageData[i][image->width - 1] = 0;
    }
}

void object::init_out_image(object::pgm *out, object::pgm image) {
    int i, j;
    strcpy(out->version, image.version);
    out->width = image.width;
    out->height = image.height;
    out->maxGrayLevel = image.maxGrayLevel;

    out->imageData = (int**) calloc(out->height, sizeof(int*));
    for(i = 0; i < out->height; i++) {
        out->imageData[i] = (int *) malloc(sizeof(int)*out->width);
    }

    out->gx = (int**) calloc(out->height, sizeof(int*));
    for(i = 0; i < out->height; i++) {
        out->gx[i] = (int *)malloc(sizeof(int)*out->width);
    }

    out->gy = (int**) calloc(out->height, sizeof(int*));
    for(i = 0; i < out->height; i++) {
        out->gy[i] = (int *)malloc(sizeof(int)*out->width);
    }

    for(i = 0; i < out->height; i++) {
        for(j = 0; j < out->width; j++) {
            out->imageData[i][j] = image.imageData[i][j];
            out->gx[i][j] = image.imageData[i][j];
            out->gy[i][j] = image.imageData[i][j];
        };
    }
}

void object::sobel_edge_detector(object::pgm *image, object::pgm *out_image) {
    int i, j, gx, gy;
    int mx[3][3] = {
            {-1, 0, 1},
            {-2, 0, 2},
            {-1, 0, 1}
    };
    int my[3][3] = {
            {-1, -2, -1},
            {0, 0, 0},
            {1, 2, 1}
    };

    for (i = 1; i < image->height - 2; i++) {
        for (j = 1; j < image->width - 2; j++) {
            gx = convolution(image, mx, i, j);
            gy = convolution(image, my, i, j);
            out_image->imageData[i][j] = sqrt(gx*gx + gy*gy);
            out_image->gx[i][j] = gx;
            out_image->gy[i][j] = gy;
        }
    }
}

int object::convolution(object::pgm *image, int kernel[3][3], int row, int col) {
    int i, j, sum = 0;
    for (i = 0; i < 3; i++) {
        for (j = 0; j < 3; j++) {
            sum += image->imageData[i + row][j + col] * kernel[i][j];
        }
    }
    return sum;
}

void object::min_max_normalization(object::pgm *image, int **matrix) {
    int min = 1000000, max = 0, i, j;

    for(i = 0; i < image->height; i++) {
        for(j = 0; j < image->width; j++) {
            if (matrix[i][j] < min) {
                min = matrix[i][j];
            }
            else if (matrix[i][j] > max) {
                max = matrix[i][j];
            }
        }
    }

    for(i = 0; i < image->height; i++) {
        for(j = 0; j < image->width; j++) {
            double ratio = (double) (matrix[i][j] - min) / (max - min);
            matrix[i][j] = ratio * 255;
        }
    }
}

void object::write_pgm_file(object::pgm *image, char *dir, int **matrix, char *name) {
    FILE* out_image;
    int i, j, count = 0;

    char* token = strtok(dir, ".");
    if (token != nullptr) {
        strcat(token, name);
        out_image = fopen(token, "wb");
    }

    out_image = fopen(dir, "wb");
    fprintf(out_image, "%s\n", image->version);
    fprintf(out_image, "%d %d\n", image->width, image->height);
    fprintf(out_image, "%d\n", image->maxGrayLevel);

    if (strcmp(image->version, "P2") == 0) {
        for(i = 0; i < image->height; i++) {
            for(j = 0; j < image->width; j++) {
                fprintf(out_image,"%d", matrix[i][j]);
                if (count % 17 == 0)
                    fprintf(out_image,"\n");
                else
                    fprintf(out_image," ");
                count ++;
            }
        }
    }
    else if (strcmp(image->version, "P5") == 0) {
        for(i = 0; i < image->height; i++) {
            for(j = 0; j < image->width; j++) {
                char num = matrix[i][j];
                fprintf(out_image,"%c", num);
            }
        }
    }
    fclose(out_image);
}
void *object::create_thread(void* arg){
    int *index = (int *) arg;
    int index_ = *index;
    printf("%i\n", index_);
    apply_filter(index_);
}
int isspace(int argument);



















