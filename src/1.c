#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
  DIR *directory;

  directory = opendir("./");
  if (directory == NULL) {
    fprintf(stderr, "Directory not exists!\n");
    return -1;
  }

  struct dirent *de = NULL;
  FILE *fin = NULL, *fout = NULL;
  int input_file = 0, output_file = 0;
  char buffer[BUFSIZ];
  int read_bytes = 0;
  output_file = open("out.txt", O_CREAT | O_TRUNC | O_WRONLY, 0644);
  if (output_file == -1) {
    fprintf(stderr, "File opening error\n");
    close(input_file);
    return -1;
  }
  while ((de = readdir(directory))) {
    if (de->d_type != 8 || !strcmp(de->d_name, ".") ||
        !strcmp(de->d_name, "..") || !strcmp("out.txt", de->d_name)) {
      continue;
    }
    input_file = open(de->d_name, O_RDONLY);
    if (input_file == -1) {
      fprintf(stderr, "File opening error %s\n", de->d_name);
      close(output_file);
      return -1;
    }

    strcat(strcat(strcat(buffer, "\\#\\"), de->d_name), "\n");

    write(output_file, buffer, strlen(buffer));

    while ((read_bytes = read(input_file, buffer, BUFSIZ))) {
      if (read_bytes == -1) {
        fprintf(stderr, "File reading error %s\n", de->d_name);
        close(input_file);
        close(output_file);
        closedir(directory);
        return -1;
      }
      write(output_file, buffer, read_bytes);
    }
    close(input_file);
  }
  closedir(directory);
  close(output_file);
}
