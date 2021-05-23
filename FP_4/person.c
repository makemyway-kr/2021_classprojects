#include <stdio.h>
#include <stdlib.h>
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
//first fit �� �̿��Ͽ� right size�� �˻���.
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

}

int main(int argc, char *argv[])
{
	FILE *fp;  // ���ڵ� ������ ���� ������
	fp=fopen(argv[1],"a+");
	if(strcmp(argv[0],"a")==0)
	{

	}
	else if(strcmp(argv[0],"d")==0)
	{

	}

	return 1;
}