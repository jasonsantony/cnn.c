#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "dataloader.h"

int main() {
  // params
  char *train_dir = "../data/train";
  int batch_size = count_files_in_directory(train_dir);
  matrix data_matrix = {.rows = batch_size, .cols = 0, .vals = NULL};
  int labels[batch_size];
  char class_names[MAX_CLASSES][MAX_CLASS_NAME_LENGTH];

  int num_images =
      load_images_into_matrix(train_dir, &data_matrix, class_names, labels);

  if (num_images <= 0) {
    printf("No images loaded.\n");
    return 1;
  }

  int h = (int)sqrt(data_matrix.cols / 3);
  printf("Loaded data_matrix with dimension (images=%d, channels=%d * h=%d * "
         "w=%d).\n",
         data_matrix.rows, 3, h, h);

  free_matrix(data_matrix);

  return 0;
}
