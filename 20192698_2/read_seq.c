#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
int main(int argc, char **argv)
{
	FILE*lenf=fopen(argv[1],"rb");
	clock_t start,end;
	int result=0;
	int count_of_record[1]={};
	fread(count_of_record,sizeof(int),1,lenf);
	fclose(lenf);
	int count=0;
	FILE*toread=fopen(argv[1],"r");
	fseek(toread,4,SEEK_SET);
	start=clock();
	char buff[250]={};
	while(feof(toread)==0)
	{
		int c=fread(buff,250,1,toread);
		if(c==250)
		{
			count++;
		}
		memset(buff,'\0',250);
	}
	end=clock();
	result=(int)((end-start)*1000);
	printf("records:%d ",count+1);
	printf("elapsed time:%d",result);

	return 0;
}
