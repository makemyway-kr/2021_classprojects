#define _CAT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <string.h>
void merge(char *file1, char *file2, char*file3)
{
    FILE* file_after_merge=fopen(file1,"w");//병합한 뒤의 결과 파일
    FILE* file_to_merge1=fopen(file2,"r");
    FILE* file_to_merge2=fopen(file3,"r");//병합할 파일들
    if(file_to_merge1==NULL||file_to_merge2==NULL)
    {
        printf("병합할 파일중 하나 또는 모두가 비어있거나 없는 파일입니다.");
    }
    char temp[10]={};
    int amount_of_char=0;

    while(feof(file_to_merge1)==0)
    {
        amount_of_char=fread(temp,sizeof(char),10,file_to_merge1);
        fwrite(temp,sizeof(char),amount_of_char,file_after_merge);
        memset(temp,0,10);
    }
    while(feof(file_to_merge2)==0)
    {
        amount_of_char=fread(temp,sizeof(char),10,file_to_merge2);
        fwrite(temp,sizeof(char),amount_of_char,file_after_merge);
        memset(temp,0,10);
    }

}
int main(void)
{
    merge("hellowworld.txt","hi.txt","hell.txt");
}