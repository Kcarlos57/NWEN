#include <stdio.h>

// Define structure record

// Implement print_record_ptr() function
struct record {
    char name[40];
    short age;
    float height;
};

void print_record_ptr(struct record *rec){
    printf("Name  : %s\n", rec->name);
    printf("Age   : %d\n", rec->age);
    printf("Height: %.2f\n", rec->height);
}


int main(void)
{
    struct record rec;

    scanf("%s %d %f", rec.name, &rec.age, &rec.height);
    print_record_ptr(&rec);
    return 0;
}
