#define _CAT_SECURE_NO_WARNINGS
#include <stdio.h>
#include<string.h>
#include <stdlib.h>
#define ten 100
int copy(char* filename,char* copyfile){
    
    FILE *original_file=fopen(filename,"r");
    FILE *copy_file=fopen(copyfile,"w");
    if(original_file==NULL)printf("파일이 비었거나 없습니다.");
    char temp[10]={};
    int amount_of_copied_letter=0;
    while(feof(original_file)==0)
    {
        amount_of_copied_letter=fread(temp,sizeof(char),ten,original_file);//10바이트 보다 작더라도 저장해 주기 위해서 읽은 글자의 수를 변수로 저장함.
        fwrite(temp,sizeof(char),amount_of_copied_letter,copy_file);//복사
        memset(temp,0,10);
    }
    fclose(original_file);
    fclose(copy_file);
}
int main(void)
{
    char* filename=malloc(sizeof(char)*100);
    char* copyfile=malloc(sizeof(char)*100);
    scanf("%s",filename);
    scanf("%s",copyfile);
    copy(filename,copyfile);
    free(filename);
    free(copyfile);
}
