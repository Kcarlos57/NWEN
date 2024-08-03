#include <stdio.h>

// Define structure record
struct record{
    char name[40];
    short age;
    float height;
};

// Implement print_record() function
void print_record(struct record rec) {
    printf("Name  : %s\n", rec.name);
    printf("Age   : %hd\n", rec.age);
    printf("Height: %.2f\n", rec.height);
}

int main(void)
{
    struct record rec;

    scanf("%s %d %f", &rec.name, &rec.age, &rec.height);
    print_record(rec);
    return 0;
}
