#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILE_LEN_OFFSET 14
#define FILE_NAME_OFFSET 4

void list_files(FILE* path);
void extract_file(FILE* path, char* filename);

int main(int argc, char** argv)
{
  if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))
  {
    printf("Usage: oz [zip archieve] [full file name]\n\nYou can also:\noz -l or --list [zip archieve] | to list files in zip\n\n");
    return 0;
  }

  if (!strcmp(argv[1], "-l") || !strcmp(argv[1], "--list"))
  {
    FILE* fr = fopen(argv[2], "rb");

    if (fr == NULL)
    {
      printf("Error while reading file %s\n", argv[2]);
      exit(1);
    }

    list_files(fr);
    return 0;
  }

  FILE* fr = fopen(argv[1], "rb");
  if (fr == NULL)
  {
    printf("Error while reading file %s\n", argv[1]);
    exit(1);
  }

  extract_file(fr, argv[2]);

  return 0;
}

void list_files(FILE* path)
{

  int len_file_cont=0;
  int len_file_name=0;
  int len_extra_filed=0;
  char* file_name;
  char* file_cont;
  char header[] = "\0\0\0\0";

  puts("Length | File Name");
  
  while(1)
  {
    fread(header, 1, 4, path);
    if (!strcmp(header, "PK\x01\x02"))
      break;

    fseek(path, FILE_LEN_OFFSET, SEEK_CUR);
    fread(&len_file_cont, 1, 4, path);

    fseek(path, FILE_NAME_OFFSET, SEEK_CUR); 
    fread(&len_file_name, 1, 2, path);

    fread(&len_extra_filed, 1, 2, path);

    file_name = malloc(len_file_name+1);
    fread(file_name, 1, len_file_name, path);
    file_name[len_file_name] = '\0';

    fseek(path, len_extra_filed, SEEK_CUR);
    
    file_cont = malloc(len_file_cont);
    fread(file_cont, 1, len_file_cont, path);

    printf("%6d | %s\n", len_file_cont, file_name);

    free(file_name);
    free(file_cont);
  }
}

void extract_file(FILE* path, char* filename)
{
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

    fread(&len_extra_filed, 1, 2, path);

    file_name = malloc(len_file_name+1);
    fread(file_name, 1, len_file_name, path);
    file_name[len_file_name] = '\0';

    fseek(path, len_extra_filed, SEEK_CUR);
    
    file_cont = malloc(len_file_cont);
    fread(file_cont, 1, len_file_cont, path);

    if (!strcmp(file_name, filename))
    {
      FILE* fw = fopen(file_name, "wb");
      fwrite(file_cont, 1, len_file_cont, fw);
      return;
    }

    free(file_name);
    free(file_cont);

  }

  printf("No such file: %s\n", filename);
}
