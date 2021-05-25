#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "person.h"
//필요한 경우 헤더 파일과 함수를 추가할 수 있음

// 과제 설명서대로 구현하는 방식은 각자 다를 수 있지만 약간의 제약을 둡니다.
// 레코드 파일이 페이지 단위로 저장 관리되기 때문에 사용자 프로그램에서 레코드 파일로부터 데이터를 읽고 쓸 때도
// 페이지 단위를 사용합니다. 따라서 아래의 두 함수가 필요합니다.
// 1. readPage(): 주어진 페이지 번호의 페이지 데이터를 프로그램 상으로 읽어와서 pagebuf에 저장한다
// 2. writePage(): 프로그램 상의 pagebuf의 데이터를 주어진 페이지 번호에 저장한다
// 레코드 파일에서 기존의 레코드를 읽거나 새로운 레코드를 쓰거나 삭제 레코드를 수정할 때나
// 위의 readPage() 함수를 호출하여 pagebuf에 저장한 후, 여기에 필요에 따라서 새로운 레코드를 저장하거나
// 삭제 레코드 관리를 위한 메타데이터를 저장합니다. 그리고 난 후 writePage() 함수를 호출하여 수정된 pagebuf를
// 레코드 파일에 저장합니다. 반드시 페이지 단위로 읽거나 써야 합니다.
//
// 주의: 데이터 페이지로부터 레코드(삭제 레코드 포함)를 읽거나 쓸 때 페이지 단위로 I/O를 처리해야 하지만,
// 헤더 레코드의 메타데이터를 저장하거나 수정하는 경우 페이지 단위로 처리하지 않고 직접 레코드 파일을 접근해서 처리한다.

//
// 페이지 번호에 해당하는 페이지를 주어진 페이지 버퍼에 읽어서 저장한다. 페이지 버퍼는 반드시 페이지 크기와 일치해야 한다.
//
Person ps[1000];
int records_per_pages=(HEADER_AREA_SIZE-4)/8;
char records[1000*((HEADER_AREA_SIZE-4)/8)][MAX_RECORD_SIZE];
int meta[4];
int *numbers;
int *lengths;
int *offsets;
void open_records(FILE*fp)
{
	fread(meta[0],sizeof(int),1,fp);//페이지 수
	fread(meta[1],sizeof(int),1,fp);//레코드 수
	fread(meta[2],sizeof(int),1,fp);//삭제페이지
	fread(meta[3],sizeof(int),1,fp);//삭제 레코드
	lengths=malloc(sizeof(int)*meta[1]);
	numbers=malloc(sizeof(int)*meta[0]);//각 페이지의 레코드 수
	offsets=malloc(sizeof(int)*meta[1]);
	int offset_left=meta[1];
	int curr=0;
	for(int page=0;page<meta[0];page++)
	{
		if(offset_left>records_per_pages)
		{
			for(int i=0;i<records_per_pages;i++)
			{
				fread(numbers[page],sizeof(int),1,fp);
				fread(offsets[curr],sizeof(int),1,fp);
				fread(lengths[curr],sizeof(int),1,fp);
				offset_left-=1;
				curr++;
			}
			for(int i=0;i<numbers[page];i++)
			{
				fread(records[curr-records_per_pages+i],lengths[curr-records_per_pages+i],1,fp);//레코드 저장
				unpack(records[curr-records_per_pages+i],&ps[curr-records_per_pages+i]);
			}
		}
		else
		{
			for(int i=0;i<offset_left;i++)
			{
				fread(numbers[page],sizeof(int),1,fp);
				fread(offsets[curr],sizeof(int),1,fp);
				fread(lengths[curr],sizeof(int),1,fp);
				curr++;
			}
			for(int i=0;i<numbers[page];i++)
			{
				fread(records[curr-offset_left+i],lengths[curr-offset_left+i],1,fp);//레코드 저장
				unpack(records[curr-offset_left+i],&ps[curr-offset_left+i]);
			}
		}
	}
}
void close_record(FILE*fp)
{
	fseek(fp,0,SEEK_SET);
	fwrite(meta[0],sizeof(int),1,fp);
	fwrite(meta[1],sizeof(int),1,fp);
	fwrite(meta[2],sizeof(int),1,fp);
	fwrite(meta[3],sizeof(int),1,fp);
	int offset_left=meta[1];
	int curr=0;
	for(int pa=0;pa<meta[0];pa++)
	{
		if(offset_left>records_per_pages)
		{
			int lengthofrecs=0;
			for(int i=0;i<records_per_pages;i++)
			{
				lengthofrecs+=lengths[curr];
				curr+=1;
				offset_left--;
			}
			char*pagebuff=malloc(sizeof(char)*(lengthofrecs+4+(8*numbers[pa])));
			char*temp=numbers[pa]+'0';
			strcat(pagebuff,temp);
			for(int i=0;i<records_per_pages;i++)
			{
				temp=offsets[curr-records_per_pages+i]+'0';
				strcat(pagebuff,temp);
				temp=lengths[curr-records_per_pages+i]+'0';
				strcat(pagebuff,temp);
				curr++;
			}
			free(temp);
			for(int i=curr-records_per_pages;i<curr;i++)
			{
				char*recordbuff=malloc(sizeof(char)*lengths[i]);
				pack(recordbuff,&ps[i]);
				strcat(pagebuff,recordbuff);
			}
			writePage(fp,pagebuff,pa);
		}
		else
		{
			int lengthofrecs=0;
			for(int i=0;i<numbers[pa];i++)
			{
				lengthofrecs+=lengths[curr];
				curr+=1;
				offset_left--;
			}
			char*pagebuff=malloc(sizeof(char)*(lengthofrecs+4+(8*numbers[pa])));
			char*temp=numbers[pa]+'0';
			strcat(pagebuff,temp);
			for(int i=0;i<records_per_pages;i++)
			{
				temp=offsets[curr-numbers[pa]+i]+'0';
				strcat(pagebuff,temp);
				temp=lengths[curr-numbers[pa]+i]+'0';
				strcat(pagebuff,temp);
				curr++;
			}
			free(temp);
			for(int i=curr-numbers[pa];i<curr;i++)
			{
				char*recordbuff=malloc(sizeof(char)*lengths[i]);
				pack(recordbuff,&ps[i]);
				strcat(pagebuff,recordbuff);
			}
			writePage(fp,pagebuff,pa);
		}
		
	}
}
void readPage(FILE *fp, char *pagebuf, int pagenum)
{
	fseek(fp,16+(PAGE_SIZE*pagenum),SEEK_SET);
	fread(pagebuf,PAGE_SIZE,1,fp);
}

//
// 페이지 버퍼의 데이터를 주어진 페이지 번호에 해당하는 레코드 파일의 위치에 저장한다. 
// 페이지 버퍼는 반드시 페이지 크기와 일치해야 한다.
//
void writePage(FILE *fp, const char *pagebuf, int pagenum)
{
	fseek(fp,16+(PAGE_SIZE*pagenum),SEEK_SET);
	fwrite(pagebuf,PAGE_SIZE,1,fp);
}

//
// 새로운 레코드를 저장할 때 터미널로부터 입력받은 정보를 Person 구조체에 먼저 저장하고, pack() 함수를 사용하여
// 레코드 파일에 저장할 레코드 형태를 recordbuf에 만든다. 
// 
void pack(char *recordbuf, const Person *p)
{
	int length=strlen(p->addr)+strlen(p->age)+strlen(p->email)+strlen(p->id)+strlen(p->name)+strlen(p->phone);
	recordbuf=malloc(sizeof(char)*length+7);
	int ind=0;
	for(ind;ind<strlen(p->id);ind++)
	{
		recordbuf[ind]=p->id[ind];
	}
	recordbuf[ind]='#';
	ind+=1;
	for(int i=0;i<strlen(p->name);i++)
	{
		recordbuf[ind]=p->name[i];
		ind++;
	}
	recordbuf[ind]='#';
	ind+=1;
	for(int i=0;i<strlen(p->age);i++)
	{
		recordbuf[ind]=p->age[i];
		ind++;
	}recordbuf[ind]='#';
	ind+=1;
	for(int i=0;i<strlen(p->addr);i++)
	{
		recordbuf[ind]=p->addr[i];
		ind++;
	}
	recordbuf[ind]='#';
	ind+=1;
	for(int i=0;i<strlen(p->phone);i++)
	{
		recordbuf[ind]=p->phone[i];
		ind++;
	}
	recordbuf[ind]='#';
	ind+=1;
	for(int i=0;i<strlen(p->email);i++)
	{
		recordbuf[ind]=p->email[i];
		ind++;
	}
	recordbuf[ind]='#';
	recordbuf[ind+1]='\0';
}

// 
// 아래의 unpack() 함수는 recordbuf에 저장되어 있는 레코드를 구조체로 변환할 때 사용한다.
//
void unpack(const char *recordbuf, Person *p)
{
	char*temp=strtok(recordbuf,"#");
	int i=0;
	while(i<6)
	{
		if(i==0) strcpy(p->id,temp);
		else if(i==1) strcpy(p->name,temp);
		else if(i==2) strcpy(p->age,temp);
		else if(i==3) strcpy(p->addr,temp);
		else if(i==4) strcpy(p->phone,temp);
		else if(i==5) strcpy(p->email,temp);
		temp=strtok(NULL,"#");
		i++;
	}
}

///
//first fit 을 이용하여 rightsize공간을 찾음
///
void search_first(FILE *fp,int* page,int* record)//
{
	
}
//
// 새로운 레코드를 저장하는 기능을 수행하며, 터미널로부터 입력받은 필드값들을 구조체에 저장한 후 아래 함수를 호출한다.
//
void add(FILE *fp, const Person *p)
{
	fseek(fp,8,SEEK_SET);
	char co1[4];
	char co2[4];
	fread(co1,4,1,fp);
	fread(co2,4,1,fp);
	int page=atoi(co1);
	int record=atoi(co2);
	if(page==-1 && record==-1)
	{
		
	}
	else
	{
		search_first(fp,&page,&record);
	}
}

//
// 주민번호와 일치하는 레코드를 찾아서 삭제하는 기능을 수행한다.
//
void delete(FILE *fp, const char *id)
{
	for(int reco=0;reco<meta[1];reco++)
	{
		if(strcmp(ps[reco].id,id)==0)
		{
			if(meta[2]==-1)//기존에 삭제 된 레코드가 없는 경우
			{
				meta[2]=reco/records_per_pages;
				meta[3]=reco%records_per_pages;
				ps[reco].id[0]='*';
				ps[reco].id[1]='-1';
				ps[reco].id[2]='-1';
			}
			else
			{
				ps[reco].id[0]='*';
				ps[reco].id[1]=meta[2]+'0';
				ps[reco].id[2]=meta[3]+'0';
				meta[2]=reco/records_per_pages;
				meta[3]=reco%records_per_pages;
			}
		}
	}
}

int main(int argc, char *argv[])
{
	FILE *fp;
	fp=fopen(argv[1],"a+b");
	open_records(fp);
	if(strcmp(argv[0],"a")==0)
	{

	}
	else if(strcmp(argv[0],"d")==0)
	{

	}

	return 1;
}