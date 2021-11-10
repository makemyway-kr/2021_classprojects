#define _CAT_SECURE_NO_WARNINGS
#include<stdio.h>
#include <string.h>
#include <stdlib.h>
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
    fclose(file_after_merge);
    FILE* fam2=fopen(file1,"a");//뒤에 덧붙여 쓰는 모드로 다시 엶.
    while(feof(file_to_merge2)==0)
    {
        amount_of_char=fread(temp,sizeof(char),10,file_to_merge2);
        fwrite(temp,sizeof(char),amount_of_char,fam2);
        memset(temp,0,10);
    }
    fclose(file_to_merge1);
    fclose(file_to_merge2);
    fclose(fam2);

}
int main(void)
{
    char *file1=malloc(sizeof(char)*100);
    char *file2=malloc(sizeof(char)*100);
    char *file3=malloc(sizeof(char)*100);
    scanf("%s",file1);
    scanf("%s",file2);
    scanf("%s",file3);
    merge(file1,file2,file3);
    free(file1);
    free(file2);
    free(file3);
}