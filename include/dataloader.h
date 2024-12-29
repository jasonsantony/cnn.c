#pragma once

#define MAX_CLASSES 1000
#define MAX_CLASS_NAME_LENGTH 128

typedef struct {
  unsigned char *data;
  int width;
  int height;
  int channels;
  int label;
} ImageData;

int count_files_in_directory(const char *);
int load_images_with_mapping(const char *, ImageData *, int *,
                             char[MAX_CLASSES][MAX_CLASS_NAME_LENGTH]);
void free_dataset(ImageData *, int);