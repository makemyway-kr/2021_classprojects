#include <stdio.h>
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
void readPage(FILE *fp, char *pagebuf, int pagenum)
{
	fseek(fp,16+(PAGE_SIZE*pagenum),SEEK_SET);
	fread(pagebuf,PAGE_SIZE,1,fp);
}


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
	sprintf(recordbuf, "%s#%s#%s#%s#%s#%s#", p -> id, p -> name, p -> age,p -> addr, p -> phone, p -> email);
    memset(recordbuf + strlen(recordbuf), (char)0xFF, MAX_RECORD_SIZE - strlen(recordbuf));
}

void unpack(char *recordbuf, Person *p)
{
	sscanf(recordbuf, "%[^'#']#%[^'#']#%[^'#']#%[^'#']#%[^'#']#%[^'#']#",p-> id, p -> name, p -> age, p -> addr, p -> phone, p -> email);
}

//
// 새로운 레코드를 저장하는 기능을 수행하며, 터미널로부터 입력받은 필드값들을 구조체에 저장한 후 아래 함수를 호출한다.
//
void add(FILE *fp, const Person *p)
{

}

//
// 주민번호와 일치하는 레코드를 찾아서 삭제하는 기능을 수행한다.
//
void delete(FILE *fp, const char *id)
{

}
//
// 주어진 레코드 파일(recordfp)을 이용하여 심플 인덱스 파일(idxfp)을 생성한다.
//
void createIndex(FILE *idxfp, FILE *recordfp)
{
	int meta[4];
	meta[0]=0;
	meta[1]=0;
	meta[2]=-1;
	meta[3]=-1;
	fseek(recordfp,0,SEEK_SET);
	fread(&meta[0],sizeof(int),1,recordfp);
	fread(&meta[1],sizeof(int),1,recordfp);
	fread(&meta[2],sizeof(int),1,recordfp);
	fread(&meta[3],sizeof(int),1,recordfp);
	int *pageofrec=malloc(sizeof(int)*meta[0]);
	int *recinpage=malloc(sizeof(int)*meta[1]);//각 레코드의 페이지 내 순서 저장 배열.
	int pages=meta[1];
	Person p[1000];
	char*pagebuff=malloc(sizeof(char)*PAGE_SIZE);
	for(int i=0;i<meta[0];i++)
	{
		memset(pagebuff,0xFF,PAGE_SIZE);
		readPage(recordfp,pagebuff,i);
		int recordsofpage=0;
		memcpy(&recordsofpage,pagebuff,sizeof(int));
		int *offsets=malloc(sizeof(int)*(recordsofpage));
		int *lengths=malloc(sizeof(int)*recordsofpage);
		for(int j=0;j<recordsofpage;j++)
		{
			memcpy(&offsets[j],pagebuff+(4*(j+1)),sizeof(int));
			memcpy(&lengths[j],pagebuff+(8*(j+1)),sizeof(int));
		}
		for(int j=0;j<recordsofpage;j++)
		{
			char*recordbuff=malloc(sizeof(char)*lengths[j]+1);
			memsest(recordbuff,0xFF,sizeof(recordbuff));
			if(j==0)
			{
				memcpy(recordbuff,pagebuff+HEADER_AREA_SIZE,lengths[j]);
			}
			else
			{
				memcpy(recordbuff,pagebuff+HEADER_AREA_SIZE+lengths[j-1],lengths[j]);
			}
			recinpage[offsets[j]]=j;
			pageofrec[offsets[j]]=i;
			if(recordbuff[0]!='*')//삭제 레코드가 아닌 경우
			{
				unpack(recordbuff,&p[offsets[j]]);
			}
			else//삭제 레코드.
			{
				p[offsets[j]].id[0]='-';
			}
			free(recordbuff);
			
		}
		free(offsets);
		free(lengths);
	}
	//Person구조체에 담아주었음 여기까지.

	///
	//이제 sort할 차례
	///
	int *sorted_sequence=malloc(sizeof(int)*meta[1]);//레코드들의 sorted된 순서를 나타내줄 별도의 배열.
	int temseq=0;
	for(int i=0;i<meta[1];i++)
	{
		if(p[i].id[0]!='-')
		{
			sorted_sequence[temseq]=i;
			temseq++;
		}
	}
	for(int j=0;j<temseq;j++)
	{
		for(int k=0;k<temseq-j-1;k++)
		{
			long long int a=atoi(p[sorted_sequence[k]].id);
			long long int b=atoi(p[sorted_sequence[k+1]].id);
			if(a>b)
			{
				int temp=sorted_sequence[k+1];
				sorted_sequence[k+1]=sorted_sequence[k];
				sorted_sequence[k]=temp;
			}
		}
	}
	//sorted된 데로 인덱스 파일에 이제 넣어줄 것임.
	fwrite(temseq,sizeof(int),1,idxfp);
	for(int j=0;j<temseq;j++)
	{
		fwrite(p[sorted_sequence[j]].id,13,1,idxfp);
		fwrite(pageofrec[sorted_sequence[j]],sizeof(int),1,idxfp);
		fwrite(recinpage[sorted_sequence[j]],sizeof(int),1,idxfp);
	}
	free(sorted_sequence);
	free(recinpage);
	free(pageofrec);
	free(pagebuff);
}

//
// 주어진 심플 인덱스 파일(idxfp)을 이용하여 주민번호 키값과 일치하는 레코드의 주소, 즉 페이지 번호와 레코드 번호를 찾는다.
// 이때, 반드시 이진 검색 알고리즘을 사용하여야 한다.
//
void binarysearch(FILE *idxfp, const char *id, int *pageNum, int *recordNum)
{

}

int main(int argc, char *argv[])
{
	if(argv[1]=='i')
	{
		FILE *idxfp=fopen(argv[3],"wb");
		FILE *recdfp=fopen(argv[2],"rb");
		createIndex(idxfp,recdfp);
		fclose(idxfp);
		close(recdfp);
	}
	else if(argv[1]=='b')
	{
		int pageNum;
		int recordNum;
		//search
		binarysearch(argv[3],argv[4],pageNum,recordNum);
	}


	return 0;
}