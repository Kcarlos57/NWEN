#include <stdio.h>
//carloskhal
int main(void)
{
    float ctemp, ftemp;
    printf("Enter temperature: ");
    scanf("%f", &ctemp);
    ftemp = (9.0/5.0)*ctemp + 32;
    printf("%.3f\n", ftemp);
}
