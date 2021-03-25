#define _CAT_SECURE_NO_WARNINGS
#include <stdio.h>
#define ten 100
void read(char* filename,char* copyfile){
    
    FILE *original_file=fopen(filename,"r");
    FILE *copy_file=fopen(copyfile,"w");
    if(original_file==NULL)printf("파일이 비었거나 없습니다.");
    while(feof(original_file)==0)
    {
        char temp[10]={};
        int amount_of_copied_letter=fread(temp,sizeof(char),10,original_file);//10바이트 보다 작더라도 저장해 주기 위해서 읽은 글자의 수를 변수로 저장함.
        fwrite(temp,sizeof(char),amount_of_copied_letter,copy_file);//복사
    }
    fclose(original_file);
    fclose(copy_file);
}
int main(void)
{
    read("hi.txt","hello.txt");
}