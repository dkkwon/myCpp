#include <stdio.h>

int main() {

  // FILE* fp = fopen("/dev/null","w");
  // FILE* fp = stdout;
  FILE* fp = fopen("output.txt", "w");
  int integer = 123;
  char character = 'c';
  char string[] = "hello, world";
  int* pointer = &integer;
  double pi = 3.141592;

  fprintf(fp, "integer : (decimal) %d (octal) %o \n", integer, integer);
  fprintf(fp, "character : %c \n", character);
  fprintf(fp, "string : %s \n", string);
  fprintf(fp, "pointer addr : %p \n", pointer);
  fprintf(fp, "floating point : %e // %f \n", pi, pi);
  fprintf(fp, "percent symbol : %% \n");

  return 0;
}
