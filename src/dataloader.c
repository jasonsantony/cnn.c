#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dataloader.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// recursively count files in a directory (for memory allocation)
int count_files_in_directory(const char *path) {
  struct dirent *entry;
  DIR *dp = opendir(path);
  if (!dp) {
    printf("Error: Could not open directory %s\n", path);
    return 0;
  }

  int file_count = 0;

  while ((entry = readdir(dp)) != NULL) {
    if (entry->d_type == DT_DIR) {
      // Skip "." and ".." directories
      if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
        continue;
      }

      // Construct the path to the subdirectory
      char sub_dir[512];
      snprintf(sub_dir, sizeof(sub_dir), "%s/%s", path, entry->d_name);

      // Recursively count files in the subdirectory
      file_count += count_files_in_directory(sub_dir);
    } else if (entry->d_type == DT_REG) {
      // Count regular files
      file_count++;
    }
  }

  closedir(dp);
  return file_count;
}

// load images from a directory and create label-to-class mapping
int load_images_with_mapping(
    const char *base_dir, ImageData *dataset, int *dataset_index,
    char class_names[MAX_CLASSES][MAX_CLASS_NAME_LENGTH]) {
  struct dirent *entry;
  DIR *dp = opendir(base_dir);
  if (!dp) {
    printf("Error: Could not open directory %s\n", base_dir);
    return -1;
  }

  int label = 0;
  while ((entry = readdir(dp)) != NULL) {
    if (entry->d_type == DT_DIR) {
      // Skip "." and ".." entries
      if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
        continue;
      }

      // process class label
      char class_dir[512];
      snprintf(class_dir, sizeof(class_dir), "%s/%s", base_dir, entry->d_name);
      // save label to class name mapping
      strncpy(class_names[label], entry->d_name, MAX_CLASS_NAME_LENGTH);
      class_names[label][MAX_CLASS_NAME_LENGTH - 1] = '\0';
      printf("Processing class: %s (label: %d)\n", entry->d_name, label);

      // open class directory
      DIR *class_dp = opendir(class_dir);
      if (!class_dp) {
        printf("Error: Could not open subdirectory %s\n", class_dir);
        continue;
      }

      struct dirent *img_entry;
      while ((img_entry = readdir(class_dp)) != NULL) {
        if (img_entry->d_type == DT_REG) { // regular file
          char img_path[512];
          snprintf(img_path, sizeof(img_path), "%s/%s", class_dir,
                   img_entry->d_name);

          // load image using stb_image
          int width, height, channels;
          unsigned char *img_data =
              stbi_load(img_path, &width, &height, &channels, 3); // force RGB
          if (!img_data) {
            printf("Warning: Could not load image %s\n", img_path);
            continue;
          }

          // add image data to the dataset
          dataset[*dataset_index].data = img_data;
          dataset[*dataset_index].width = width;
          dataset[*dataset_index].height = height;
          dataset[*dataset_index].channels = 3; // RGB
          dataset[*dataset_index].label = label;

          printf("Loaded: %s (Size: %dx%d, Label: %d)\n", img_path, width,
                 height, label);

          (*dataset_index)++;
        }
      }
      closedir(class_dp);

      label++; // Increment label for the next class
    }
  }
  closedir(dp);

  return label; // Return the number of classes processed
}

// free the dataset memory
void free_dataset(ImageData *dataset, int dataset_size) {
  for (int i = 0; i < dataset_size; i++) {
    free(dataset[i].data);
  }
}
