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
	FILE*toread=fopen(argv[1],"r");
	fseek(toread,4,SEEK_SET);
	char buff[251]={};
	int count=0;
	start=clock();	
	while(feof(toread)==0)
	{
		int c=fread(buff,sizeof(char),250,toread);
		memset(buff,0,250);
		if (c==250)count++;
	}
	end=clock();
	result=(int)((end-start)*1000);
	printf("#records: %d ",count);
	printf("elapsed_time: %d us\n",result);
	fclose(toread);

	return 0;
}
