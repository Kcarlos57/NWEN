#include <stdio.h>

int main(void)
{
  int a, b, sum;
  FILE *in; // use for handling input file
  FILE *out; // use for handling output file
  
  // Open raw.txt for reading
  in = fopen("raw.txt", "r");
  if (in == NULL) {
    printf("Error opening raw.txt.\n");
    return 1;
  }
  
  // Open processed.txt for writing
  out = fopen("processed.txt", "w");
  if (out == NULL) {
    printf("Error opening processed.txt.\n");
    return 1;
  }
  
  // Go thru raw.txt file and generate processed.txt file accordingly
  while (fscanf(in, "%d%d", &a, &b) == 2) {
    sum = a + b;
    fprintf(out, "%d %d %d\n", a, b, sum);
  }
  fclose(in);
  fclose(out);
  return 0;
}
