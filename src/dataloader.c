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
      // skip "." and ".." directories
      if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
        continue;
      }

      // construct the path to the subdirectory
      char sub_dir[512];
      snprintf(sub_dir, sizeof(sub_dir), "%s/%s", path, entry->d_name);

      // recursively count files in the subdirectory
      file_count += count_files_in_directory(sub_dir);
    } else if (entry->d_type == DT_REG) {
      // count regular files
      file_count++;
    }
  }

  closedir(dp);
  return file_count;
}

// load images from directory into a matrix (batch_size, channels * h * w)
int load_images_into_matrix(
    const char *data_dir, matrix *data_matrix,
    char class_names[MAX_CLASSES][MAX_CLASS_NAME_LENGTH], int *labels) {

  struct dirent *entry;
  DIR *dp = opendir(data_dir);
  if (!dp) {
    printf("Error: Could not open directory %s\n", data_dir);
    return -1;
  }

  int dataset_index = 0;
  int label = 0;
  int channels = 3; // rgb

  while ((entry = readdir(dp)) != NULL && dataset_index < data_matrix->rows) {
    if (entry->d_type == DT_DIR) {
      // skip "." and ".."
      if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
        continue;
      }

      // process class directory
      char class_dir[512];
      snprintf(class_dir, sizeof(class_dir), "%s/%s", data_dir, entry->d_name);

      // save label to class name mapping
      strncpy(class_names[label], entry->d_name, MAX_CLASS_NAME_LENGTH);
      class_names[label][MAX_CLASS_NAME_LENGTH - 1] = '\0';
      printf("Processing class: %s (label: %d)\n", entry->d_name, label);

      DIR *class_dp = opendir(class_dir);
      if (!class_dp) {
        printf("Error: Could not open subdirectory %s\n", class_dir);
        continue;
      }

      struct dirent *img_entry;
      while ((img_entry = readdir(class_dp)) != NULL &&
             dataset_index < data_matrix->rows) {
        if (img_entry->d_type == DT_REG) { // regular file
          char img_path[512];
          snprintf(img_path, sizeof(img_path), "%s/%s", class_dir,
                   img_entry->d_name);

          // load image using stb_image
          int width, height, img_channels;
          unsigned char *img_data =
              stbi_load(img_path, &width, &height, &img_channels, channels);
          if (!img_data) {
            printf("Warning: Could not load image %s\n", img_path);
            continue;
          }

          // ensure consistent dimensions for the first image
          if (dataset_index == 0) {
            data_matrix->cols = channels * width * height;
            data_matrix->vals =
                (float **)malloc(data_matrix->rows * sizeof(float *));
          } else if (data_matrix->cols != channels * width * height) {
            printf("Error: Image dimensions are inconsistent (%s has different "
                   "size).\n",
                   img_path);
            stbi_image_free(img_data);
            continue;
          }

          // allocate memory for the row and normalize pixel data
          data_matrix->vals[dataset_index] =
              (float *)malloc(data_matrix->cols * sizeof(float));
          for (int i = 0; i < data_matrix->cols; i++) {
            data_matrix->vals[dataset_index][i] =
                img_data[i] / 255.0f; // Normalize to [0, 1]
          }

          // store the label
          labels[dataset_index] = label;

          printf("Loaded: %s (Size: %dx%d, Label: %d)\n", img_path, width,
                 height, label);

          stbi_image_free(img_data);
          dataset_index++;
        }
      }

      closedir(class_dp);
      label++;
    }
  }

  closedir(dp);
  return dataset_index; // Return number of images loaded
}
