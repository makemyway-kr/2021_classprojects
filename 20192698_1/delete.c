#include<stdio.h>
#include<stdlib.h>
#include<string.h>
void delete(char*filename,int offset,int bytetodelete)
{
    FILE*del_file=fopen(filename,"r");//읽기와 쓰기로 오픈
    fseek(del_file,0,SEEK_END);//파일의 크기 확인
    int size_of_file=ftell(del_file);//파일의 크기를 담음.
    if(size_of_file<=offset+bytetodelete)//삭제할 바이트 수 만큼이 존재하지 않을 때 또는 끝까지 지울 떄
    {
        char*temp=malloc(sizeof(char)*offset+1);
        fseek(del_file,0,SEEK_SET);
        fread(temp,sizeof(char)*offset,1,del_file);
        fseek(del_file,0,SEEK_SET);
        fclose(del_file);
        FILE*w_file=fopen(filename,"w");
        fputs(temp,w_file);
        free(temp);
        fclose(w_file);
    }
    else
    {
        fseek(del_file,0,SEEK_SET);
        char*temp1=malloc(sizeof(char)*offset);//오프셋 전의 문자열
        fread(temp1,sizeof(char)*offset,1,del_file);
        fseek(del_file,offset+bytetodelete,SEEK_SET);
        char*temp2=malloc(sizeof(char)*(size_of_file-offset-bytetodelete)+1);//오프셋 뒤의 문자열
        fread(temp2,sizeof(char)*(size_of_file-offset-bytetodelete),1,del_file);
        fclose(del_file);
        FILE*w_file=fopen(filename,"w");
        fputs(temp1,w_file);
        fputs(temp2,w_file);

        free(temp1);
        free(temp2);
        fclose(w_file);
    }
}
int main(void)
{
    char*filename;
    scanf("%s",filename);
    int offset=0;
    int bytetodelete=0;
    scanf("%d %d",&offset,&bytetodelete);
    delete(filename,offset,bytetodelete);
    
}