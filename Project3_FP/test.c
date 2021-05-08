#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
int main(int argc, char *argv[])
{
   
    char*buff;
    buff=malloc(4);
    int a=12;
    sprintf(buff,"%d",a);
    printf("%d",atoi(buff));
    free(buff);
   
    //free(buff);
}