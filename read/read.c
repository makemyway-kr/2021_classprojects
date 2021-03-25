#define _CAT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
int read(char* name_of_file,int offset, int number)
{
    FILE* ftr=fopen(name_of_file,"r");//file to read
    fseek(ftr,0,SEEK_END);//파일의 크기 확인
    int size_of_file=ftell(ftr);
    if(ftr==NULL)
    {
        printf("파일이 없거나 비었습니다.");
    }
    if(number>size_of_file || offset+number-1>size_of_file)//number만큼 데이터가 존재하지 않으면
    {
        int ch_to_read=size_of_file-offset+1;//offset부터 끝까지
        char *temp;
        temp=malloc(sizeof(char)*size_of_file+1);
        fseek(ftr,offset,SEEK_SET);//파일 포인터를 offset위치로 이동
        fread(temp,ch_to_read,1,ftr);
        printf("%s",temp);
        free(temp);
        return 0;
    }
    else
    {
        char *temp=malloc(sizeof(char)*number+1);
        fseek(ftr,offset,SEEK_SET);
        fread(temp,number,1,ftr);
        printf("%s",temp);
        free(temp);
        return 0;
    }
}
int main(void)
{
    read("hi.txt",3,19);
    return 0;
}