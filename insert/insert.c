#include <stdio.h>
#include<stdlib.h>
#include <string.h>
void insert(char* filename,int offset,char*data)
{
    FILE*mod_file=fopen(filename,"r+w");//읽기와 쓰기로 오픈
    fseek(mod_file,0,SEEK_END);//파일의 크기 확인
    int size_of_file=ftell(mod_file);//파일의 크기를 담음.
    char* temp=malloc(sizeof(char)*(size_of_file-offset));//오프셋+1부터 끝까지의 문자열을 저장할 임시 공간
    fseek(mod_file,offset+1,SEEK_SET);//오프셋 +1로 포인터 이동
    fread(temp,sizeof(temp),1,mod_file);//오프셋+1부터 끝까지의 문자열 저장.
    fseek(mod_file,offset+1,SEEK_SET);//오프셋+1로 포인터 이동.
    fputs(data,mod_file);//집어넣을 데이터 쓰기
    fputs(temp,mod_file);//기존 offset+1부터 있던 데이터 입력
    free(temp);
    fclose(mod_file);    
}
int main(void)
{
    insert("hi.txt",2,"2222");
}