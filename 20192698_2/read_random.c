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
	int *read_order_list=malloc(sizeof(int)*count_of_record[0]);
	if(count_of_record[0]>1)
	{
		GenRecordSequence(read_order_list, count_of_record[0]);
	}
	else{
		read_order_list[0]=0;
	}
	FILE* readf=fopen(argv[1],"r");
	fseek(readf,read_order_list[0]+4,SEEK_SET);
	clock_t start,end;
	int result=0;
	int count=1;
	char buff[251]={};
	start=clock();
	while(count<count_of_record[0])
	{
		fread(buff,250,1,readf);
		memset(buff,0,251);
		fseek(readf,(read_order_list[count])+4,SEEK_SET);
		count++;
	}
	end=clock();
	result=(int)((end-start)*1000);
	printf("#records: %d ",count);
	printf("elapsed_time: %d us\n",result);
	fclose(readf);
	

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
