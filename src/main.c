#include <stdio.h>
#include <stdlib.h>

#include "dataloader.h"

// Main function
int main() {
  const char *dataset_path = "../data/train"; // Path to dataset
  int num_images = count_files_in_directory(dataset_path);
  ImageData *dataset = malloc(num_images * sizeof(ImageData));
  if (!dataset) {
    printf("Error: Memory allocation failed\n");
    return -1;
  }

  char class_names[MAX_CLASSES]
                  [MAX_CLASS_NAME_LENGTH]; // Array to store class names
  int dataset_index = 0;

  int num_classes = load_images_with_mapping(dataset_path, dataset,
                                             &dataset_index, class_names);
  if (num_classes > 0) {
    printf("Loaded %d images from %d classes\n", dataset_index, num_classes);
  }

  // Example: Access the dataset and print class names
  for (int i = 0; i < dataset_index; i++) {
    printf("Image %d: %dx%d, Label: %d, Class: %s\n", i, dataset[i].width,
           dataset[i].height, dataset[i].label, class_names[dataset[i].label]);
  }

  // Free dataset memory
  free_dataset(dataset, dataset_index);
  free(dataset);

  return 0;
}
