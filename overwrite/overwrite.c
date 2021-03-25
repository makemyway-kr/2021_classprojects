#define _CAT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void overwrite(char *filename,int offset,char *data)
{
    FILE* filetowrite=fopen(filename,"r+w");//읽기+쓰기
    fseek(filetowrite,0,SEEK_END);//파일의 크기 확인
    int size_of_file=ftell(filetowrite);
    if(size_of_file>offset+strlen(data)-1)//eof를 만나지 않는 경우
    {
        fseek(filetowrite,offset,SEEK_SET);//오프셋으로 포인터 위치 조정
        fwrite(data,sizeof(data)*sizeof(char),1,filetowrite);
        fclose(filetowrite);
    }
    else
    {
        fseek(filetowrite,offset,SEEK_SET);
        char *temp=malloc((size_of_file-offset+1)*sizeof(char));//파일의 현재 끝까지 저장할 수 있는 만큼 할당
        int flag=sizeof(temp)/sizeof(char);//첫번째 쓰기 작업의 마지막 부분을 flag로 표시함.
        for(int i=0;i<sizeof(temp)/sizeof(char);i++)
        {
            temp[i]=data[i];
        }
        fwrite(temp,strlen(temp)*sizeof(char),1,filetowrite);
        fclose(filetowrite);
        free(temp);
        FILE*filetowrite2=fopen(filename,"a");//뒤에 쓰는 형식으로 다시 엶(eof를 무시하고 계속 쓰기 위함)
        char*temp2=malloc((strlen(data)-(size_of_file-offset+1))*sizeof(char));//나머지 할당
        for(int i=0;i<sizeof(temp2)/sizeof(char);i++)
        {
            temp[i]=data[flag+i];

        }
        fwrite(temp,strlen(temp2)*sizeof(char),1,filetowrite2);
        fclose(filetowrite2);
        free(temp2);
    }
}
int main(void)
{
    overwrite("hi.txt",4,"Idontwannadothis");
}