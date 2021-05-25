#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "person.h"
//�ʿ��� ��� ��� ���ϰ� �Լ��� �߰��� �� ����

// ���� ������� �����ϴ� ����� ���� �ٸ� �� ������ �ణ�� ������ �Ӵϴ�.
// ���ڵ� ������ ������ ������ ���� �����Ǳ� ������ ����� ���α׷����� ���ڵ� ���Ϸκ��� �����͸� �а� �� ����
// ������ ������ ����մϴ�. ���� �Ʒ��� �� �Լ��� �ʿ��մϴ�.
// 1. readPage(): �־��� ������ ��ȣ�� ������ �����͸� ���α׷� ������ �о�ͼ� pagebuf�� �����Ѵ�
// 2. writePage(): ���α׷� ���� pagebuf�� �����͸� �־��� ������ ��ȣ�� �����Ѵ�
// ���ڵ� ���Ͽ��� ������ ���ڵ带 �аų� ���ο� ���ڵ带 ���ų� ���� ���ڵ带 ������ ����
// ���� readPage() �Լ��� ȣ���Ͽ� pagebuf�� ������ ��, ���⿡ �ʿ信 ���� ���ο� ���ڵ带 �����ϰų�
// ���� ���ڵ� ������ ���� ��Ÿ�����͸� �����մϴ�. �׸��� �� �� writePage() �Լ��� ȣ���Ͽ� ������ pagebuf��
// ���ڵ� ���Ͽ� �����մϴ�. �ݵ�� ������ ������ �аų� ��� �մϴ�.
//
// ����: ������ �������κ��� ���ڵ�(���� ���ڵ� ����)�� �аų� �� �� ������ ������ I/O�� ó���ؾ� ������,
// ��� ���ڵ��� ��Ÿ�����͸� �����ϰų� �����ϴ� ��� ������ ������ ó������ �ʰ� ���� ���ڵ� ������ �����ؼ� ó���Ѵ�.

//
// ������ ��ȣ�� �ش��ϴ� �������� �־��� ������ ���ۿ� �о �����Ѵ�. ������ ���۴� �ݵ�� ������ ũ��� ��ġ�ؾ� �Ѵ�.
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
	fread(meta[0],sizeof(int),1,fp);//������ ��
	fread(meta[1],sizeof(int),1,fp);//���ڵ� ��
	fread(meta[2],sizeof(int),1,fp);//����������
	fread(meta[3],sizeof(int),1,fp);//���� ���ڵ�
	lengths=malloc(sizeof(int)*meta[1]);
	numbers=malloc(sizeof(int)*meta[0]);//�� �������� ���ڵ� ��
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
				fread(records[curr-records_per_pages+i],lengths[curr-records_per_pages+i],1,fp);//���ڵ� ����
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
				fread(records[curr-offset_left+i],lengths[curr-offset_left+i],1,fp);//���ڵ� ����
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
// ������ ������ �����͸� �־��� ������ ��ȣ�� �ش��ϴ� ���ڵ� ������ ��ġ�� �����Ѵ�. 
// ������ ���۴� �ݵ�� ������ ũ��� ��ġ�ؾ� �Ѵ�.
//
void writePage(FILE *fp, const char *pagebuf, int pagenum)
{
	fseek(fp,16+(PAGE_SIZE*pagenum),SEEK_SET);
	fwrite(pagebuf,PAGE_SIZE,1,fp);
}

//
// ���ο� ���ڵ带 ������ �� �͹̳ηκ��� �Է¹��� ������ Person ����ü�� ���� �����ϰ�, pack() �Լ��� ����Ͽ�
// ���ڵ� ���Ͽ� ������ ���ڵ� ���¸� recordbuf�� �����. 
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
// �Ʒ��� unpack() �Լ��� recordbuf�� ����Ǿ� �ִ� ���ڵ带 ����ü�� ��ȯ�� �� ����Ѵ�.
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
//first fit �� �̿��Ͽ� rightsize������ ã��
///
void search_first(FILE *fp,int* page,int* record)//
{
	
}
//
// ���ο� ���ڵ带 �����ϴ� ����� �����ϸ�, �͹̳ηκ��� �Է¹��� �ʵ尪���� ����ü�� ������ �� �Ʒ� �Լ��� ȣ���Ѵ�.
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
// �ֹι�ȣ�� ��ġ�ϴ� ���ڵ带 ã�Ƽ� �����ϴ� ����� �����Ѵ�.
//
void delete(FILE *fp, const char *id)
{
	for(int reco=0;reco<meta[1];reco++)
	{
		if(strcmp(ps[reco].id,id)==0)
		{
			if(meta[2]==-1)//������ ���� �� ���ڵ尡 ���� ���
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