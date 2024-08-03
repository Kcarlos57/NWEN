#include <stdio.h>

#define SUM(a, b) a + b
//carloskhal
int main(void)
{
    int a, b, s;
    scanf("%d %d", &a, &b);
    s = SUM(a, b);
    printf("%d", s);
}
