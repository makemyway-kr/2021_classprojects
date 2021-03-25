#define _CAT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
void read(char* name_of_file,int offset, int number)
{
    FILE* ftr=fopen(name_of_file,"r");//file to read
    if(ftr==NULL)
    {
        printf("파일이 없거나 비었습니다.");
    }
    if(number==NULL)
    {
        fseek(ftr,0,SEEK_END);
        int size_of_file=ftell(ftr)-offset;//파일의 offset위치부터 끝까지의 크기를 입력
        char *temp;
        temp=malloc(sizeof(char)*size_of_file+1);
        fseek(ftr,offset,SEEK_SET);//파일 포인터를 offset위치로 이동
        fread(temp,size_of_file,1,ftr);
        printf("%s",temp);
   
    }
    else
    {
        char *temp=malloc(sizeof(char)*number+1);
        fseek(ftr,offset,SEEK_SET);
        fread(temp,number,1,ftr);
        printf("%s",temp);
   
    }
}
int main(void)
{
    read("hi.txt",3,13);
    return -1;
}