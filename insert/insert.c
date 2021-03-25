#include <stdio.h>
#include<stdlib.h>
#include <string.h>
void insert(char* filename,int offset,char*data)
{
    FILE*mod_file=fopen(filename,"r+w");//읽기와 쓰기로 오픈
    fseek(mod_file,0,SEEK_END);//파일의 크기 확인
    int size_of_file=ftell(mod_file);//
    char*temp=malloc(sizeof(char)*(size_of_file-offset-1));//뒤의 문장 저장할 임시 공간
    fseek(mod_file,offset+1,SEEK_SET);
    fread(temp,sizeof(temp),1,mod_file);
    char *td=malloc(sizeof(char)*(size_of_file-offset-1));//temporary data->data의 내용중 앞부분을 자름.
    int flag=sizeof(td)/sizeof(char);
     for(int i=0;i<sizeof(td)/sizeof(char);i++)
    {
        td[i]=data[i];
    }
    char *td2=malloc(sizeof(char)*(strlen(data)-strlen(td)));
    for(int i=0;i<sizeof(td2)/sizeof(char);i++)
    {
        td2[i]=data[flag+i];
    }  
    fseek(mod_file,offset,SEEK_SET);
    fwrite(td,sizeof(td),1,mod_file);
    fclose(mod_file);
    free (td);
    FILE*mo_file=fopen(filename,"a");
    fwrite(td2,sizeof(td2),1,mo_file);
    free(td2);
    fclose(mo_file);
    
}
int main(void)
{
    insert("hi.txt",2,"aaaaa");
}