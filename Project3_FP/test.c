#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
int main(int argc, char *argv[])
{
   
    char*buff;
    buff=malloc(4);
    int a=512;
    sprintf(buff,"%d",a);
    printf("%c",buff[2]);
    free(buff);
    //free(buff);
}