#include <stdio.h>
#include <stdlib.h>

#define FILE_LEN_OFFSET 18
#define FILE_NAME_OFFSET 26

void list_files(FILE* path) {
  
  int len_file_name;
  int len_file_cont;
  int len_extra_filed;
  char* file_name;
  char* file_cont;

  fseek(path, FILE_LEN_OFFSET, SEEK_SET);
  fread(&len_file_cont, 1 , 4, path);

  fseek(path, FILE_NAME_OFFSET, SEEK_SET); 
  fread(&len_file_name, 1, 2, path);

  printf("Len File Name: %d\nLen File Conten: %d\n", len_file_name, len_file_cont);

  fread(&len_extra_filed, 1, 2, path);

  file_name = malloc(len_file_name);
  fread(file_name, 1, len_file_name, path);
 
  fseek(path, len_extra_filed, SEEK_CUR);
  
  file_cont = malloc(len_file_cont);
  fread(file_cont, 1, len_file_cont, path);

  printf("----\nFile Name: %s\nFile Contents: %s----\n", file_name, file_cont);
}

int main(int argc, char** argv) {
  FILE* fr = fopen("aim.zip", "rb");
  list_files(fr);
  return 0;
}
