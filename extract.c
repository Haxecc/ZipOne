#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_LEN_OFFSET 14
#define FILE_NAME_OFFSET 4

void list_files(FILE* path) {

  int len_file_cont=0;
  int len_file_name=0;
  int len_extra_filed=0;
  char* file_name;
  char* file_cont;
  char header[] = "\0\0\0\0";
  
  while(1)
  {
    fread(header, 1, 4, path);
    if (!strcmp(header, "PK\x01\x02"))
      break;

    fseek(path, FILE_LEN_OFFSET, SEEK_CUR);
    fread(&len_file_cont, 1, 4, path);

    fseek(path, FILE_NAME_OFFSET, SEEK_CUR); 
    fread(&len_file_name, 1, 2, path);

    printf("----\nLen File Name: %d\nLen File Conten: %d\n", len_file_name, len_file_cont);

    fread(&len_extra_filed, 1, 2, path);

    file_name = malloc(len_file_name);
    fread(file_name, 1, len_file_name, path);
    file_name[len_file_name] = '\0';

    fseek(path, len_extra_filed, SEEK_CUR);
    
    file_cont = malloc(len_file_cont);
    fread(file_cont, 1, len_file_cont, path);

    printf("----\nFile Name: %s\nFile Contents: %s----\n", file_name, file_cont);

    free(file_name);
    free(file_cont);
  }
}

int main(int argc, char** argv) {
  FILE* fr = fopen("aim.zip", "rb");
  list_files(fr);
  return 0;
}
