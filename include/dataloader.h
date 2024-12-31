#pragma once

#include "matrix.h"

#define MAX_CLASSES 1000
#define MAX_CLASS_NAME_LENGTH 128

typedef struct _image_data {
  unsigned char *data;
  int width;
  int height;
  int channels;
  int label;
} image_data;

int count_files_in_directory(const char *path);
int load_images_into_matrix(
    const char *data_dir, matrix *data_matrix,
    char class_names[MAX_CLASSES][MAX_CLASS_NAME_LENGTH], int *labels);