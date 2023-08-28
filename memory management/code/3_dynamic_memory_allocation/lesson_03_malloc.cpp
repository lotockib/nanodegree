#include <stdio.h>
#include <stdlib.h>

int main()
{
    int *p = (int*) malloc(3 * sizeof(int));

    // Pointers and arrays are treated the same
    p[0] = 1;
    p[1] = 2;
    p[3] = 3;
    printf("address=%p, valuepointer=%d, value=%d, value2=%d\n", p, *p, p[0], p[1]);

    return 0;
}
