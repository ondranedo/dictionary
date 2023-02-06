#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

size_t nOPD(size_t num)
{
    size_t count = 0;

    while (floor(num / 10) > 0) { num /= 10; count++; };

    return count + 1;
}

void decodeLine(size_t ln, char* buff)
{
    int i;
    size_t n = nOPD(ln);
    for (i = n - 1; i >= 0; i--)
    {
        buff[i] = (ln % 10) + 'A';
        ln /= 10;
    }
    buff[n] = '\0';
}

void addLine(FILE* fp, size_t ln, int max_ln, size_t* counter)
{
    char buff[60], pbuff[60];
    decodeLine(ln, buff);
    memcpy(pbuff, buff, 60);
    size_t n = rand() % 20;

    fprintf(fp, "%d;%s;p%s\n", n, buff, pbuff);

    if ((*counter) > (max_ln/10)) {
        (*counter) = 0;
        printf("Completed: %2.f%%\n", ((double)ln / (double)max_ln) * 100.0);
    }
    (*counter)++;
}

int main(int argc, char** argv)
{
    size_t counter = 0;
    int length = 0;
    FILE* fp = fopen("dic.csv", "w");
    srand(time(0));
    fprintf(fp, "lesson;t1;t2\n");

    printf("Enter number of lines: ");
    scanf("%d", &length);

    for (size_t i = 0; i < length; i++)
        addLine(fp, i, length, &counter);

    fclose(fp);
    system("pause");
    return 0;
}
