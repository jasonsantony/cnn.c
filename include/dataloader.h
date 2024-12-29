#pragma once

#define MAX_CLASSES 1000
#define MAX_CLASS_NAME_LENGTH 128

typedef struct image_data {
  unsigned char *data;
  int width;
  int height;
  int channels;
  int label;
} image_data;

int count_files_in_directory(const char *);
int load_images_with_mapping(const char *, image_data *, int *,
                             char[MAX_CLASSES][MAX_CLASS_NAME_LENGTH]);
void free_dataset(image_data *, int);