#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    int i, sum = 0;
    
    // Check if there are command line arguments
    if (argc == 1) {
        printf("0\n");
        return 0;
    }
    
    // Add up the integers in the command line arguments
    for (i = 1; i < argc; i++) {
        sum += atoi(argv[i]);
    }
    
    // Print the sum
    printf("%d\n", sum);
    
    return 0;
}
