#include <stdio.h>
#include <sys/types.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
//필요하면 header file 추가 가능

#define SUFFLE_NUM	10000	// 이 값은 마음대로 수정할 수 있음.

void GenRecordSequence(int *list, int n);
void swap(int *a, int *b);
// 필요한 함수가 있으면 더 추가할 수 있음.

int main(int argc, char **argv)
{
	FILE*lenf=fopen(argv[1],"rb");
	int count_of_record[1]={};
	fread(count_of_record,sizeof(int),1,lenf);
	fclose(lenf);
	int num_of_records=count_of_record[1]; 
	int *read_order_list=malloc(sizeof(int)*num_of_records);
	if(num_of_records>1)
	{
		GenRecordSequence(read_order_list, num_of_records);
	}

	FILE* readf=fopen(argv[1],"r");
	fseek(readf,read_order_list[0]+4,SEEK_SET);
	clock_t start,end;
	int result=0;
	int count=0;
	char buff[250]={};
	start=clock();
	while(feof(readf)==0)
	{
		int c=fread(buff,250,1,readf);
		if(c==250)
		{
			count++;
		}
		memset(buff,'\0',250);
		fseek(readf,(read_order_list[count+1]*2)+4,SEEK_SET);
	}
	end=clock();
	result=(int)((end-start)*1000);
	printf("records:%d ",count+1);
	printf("elapsed_time:%d",result);
	

	return 0;
}

void GenRecordSequence(int *list, int n)
{
	int i, j, k;

	srand((unsigned int)time(0));

	for(i=0; i<n; i++)
	{
		list[i] = i;
	}
	
	for(i=0; i<SUFFLE_NUM; i++)
	{
		j = rand() % n;
		k = rand() % n;
		swap(&list[j], &list[k]);
	}
}

void swap(int *a, int *b)
{
	int tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}
