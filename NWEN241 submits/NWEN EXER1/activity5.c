#include <stdio.h>
//carloskhal
float calculate_area(float l, float w)
{
    return l * w;
}

int main(void)
{
    float l, w, a;
    scanf("%f %f", &l, &w);
    a = calculate_area(l, w);
    printf("%f", a);
}
