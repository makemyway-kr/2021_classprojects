// 주의사항
// 1. blockmap.h에 정의되어 있는 상수 변수를 우선적으로 사용해야 함
// 2. blockmap.h에 정의되어 있지 않을 경우 본인이 이 파일에서 만들어서 사용하면 됨
// 3. 필요한 data structure가 필요하면 이 파일에서 정의해서 쓰기 바람(blockmap.h에 추가하면 안됨)

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include "blockmap.h"
// 필요한 경우 헤더 파일을 추가하시오.

//
// flash memory를 처음 사용할 때 필요한 초기화 작업, 예를 들면 address mapping table에 대한
// 초기화 등의 작업을 수행한다. 따라서, 첫 번째 ftl_write() 또는 ftl_read()가 호출되기 전에
// file system에 의해 반드시 먼저 호출이 되어야 한다.
//
extern int dd_read();
extern int dd_write();
extern int dd_erase();
int **addmapt;//address mapping table
int spare;//spare block초기값은 제일 끝의 블럭을 사용함.
void ftl_open()
{
	addmapt=malloc(sizeof(int*)*BLOCKS_PER_DEVICE);
	for(int i=0;i<BLOCKS_PER_DEVICE;i++)//address maping table 초기화.
	{
		addmapt[i]=malloc(sizeof(int)*2);
		if(i!=BLOCKS_PER_DEVICE-1)//free block이 아니면, addmpat[blocks per device]를 free로 지정하였음.
		{
			addmapt[i][0]=i;//lbn넣어줌.
		}
		else
		{
			addmapt[i][0]=-1;//freeblock의 lbn은 -1
		}
	}
	int countof0xff=0;
	char* buf = (char *)malloc(PAGE_SIZE);
	memset(buf,(char)0xFF,PAGE_SIZE);
	for(int i=0;i<(PAGES_PER_BLOCK*BLOCKS_PER_DEVICE);i++)//0xff로 되어있는 page의 수를 셈.
	{
		dd_read(i,buf);
		if(*buf==-1)
		{
			countof0xff++;
		}
		memset(buf,(char)0xFF,PAGE_SIZE);
	}
	int notusedpbn=0;//사용 안된 pbn저장 for 이미 flash memory file존재하는 경우
	if(countof0xff==PAGES_PER_BLOCK*BLOCKS_PER_DEVICE)//기존 flash memory file이 없어 0xff로 모두 초기화되어있는경우
	{
		for(int i=0;i<BLOCKS_PER_DEVICE-1;i++)
		{
			addmapt[i][1]=-1;//pbn -1로 초기화
		}
		spare=BLOCKS_PER_DEVICE-1;//spare block index의 초기값은 제일 끝의 블럭을 사용함.
		addmapt[BLOCKS_PER_DEVICE-1][1]=spare;
	}
	else//flash memory file이 존재.
	{
		for(int i=0;i<BLOCKS_PER_DEVICE;i++)
		{
			dd_read(i*PAGES_PER_BLOCK,buf);
			char*bufforlbn=malloc(4);
			if(*buf!=-1)//block이 할당이 되어있으면
			{
				for(int ll=0;ll<4;ll++)
				{
					bufforlbn[ll]=buf[SECTOR_SIZE+ll];
				}
				addmapt[atoi(bufforlbn)][1]=i;
			}
			else //block(pbn)이 할당이 안되어있으면 freeblock pbn으로
			{
				notusedpbn=i;
			}
			free(bufforlbn);
		}
		addmapt[BLOCKS_PER_DEVICE-1][1]=notusedpbn;//freeblock할당.
		spare=notusedpbn;
	}
	free(buf);

	return;
}

//
// 이 함수를 호출하는 쪽(file system)에서 이미 sectorbuf가 가리키는 곳에 512B의 메모리가 할당되어 있어야 함
// (즉, 이 함수에서 메모리를 할당 받으면 안됨)
//
void ftl_read(int lsn, char *sectorbuf)
{
	int lbnnow=lsn/PAGES_PER_BLOCK;
	char*pagebufff=malloc(PAGE_SIZE);
	memset(pagebufff,(char)0xFF,PAGE_SIZE);
	for(int i=0;i<PAGES_PER_BLOCK;i++)
	{
		char*bufforlsn=malloc(4);
		dd_read((addmapt[lbnnow][1]*PAGES_PER_BLOCK)+i,pagebufff);
		int countforlsn=0;
		for(int j=(512/sizeof(char))+4;j<(512/sizeof(char))+8;j++)//lsn복사
		{
			bufforlsn[countforlsn]=pagebufff[j];//옮겨담음.
			countforlsn++;
		}
		if(atoi(bufforlsn)==lsn)
		{
			break;
		}
		else
		{
			memset(pagebufff,(char)0xFF,PAGE_SIZE);
		}
		free(bufforlsn);
		
	}
	memcpy(sectorbuf,pagebufff,SECTOR_SIZE);
	free(pagebufff);
	return;
}

//
// 이 함수를 호출하는 쪽(file system)에서 이미 sectorbuf가 가리키는 곳에 512B의 메모리가 할당되어 있어야 함
// (즉, 이 함수에서 메모리를 할당 받으면 안됨)
//
void ftl_write(int lsn, char *sectorbuf)
{
	char is_there_lsn_already=FALSE;//동일한 LSN이 있는지
	int lbnnow=lsn/PAGES_PER_BLOCK;
	char*emptpage=malloc(PAGE_SIZE);//비어있는 페이지로 비교함.
	memset(emptpage,(char)0xFF,PAGE_SIZE);
	char*pagebufff=malloc(PAGE_SIZE);//page를 받아오는 buffer
	char*newpagebufff=malloc(PAGE_SIZE);//새로 넣을 page
	memset(newpagebufff,(char)0xFF,PAGE_SIZE);
	//넣을 데이터 세팅 /////
	memcpy(newpagebufff,sectorbuf,SECTOR_SIZE);
	char*lbnlsnbuff=malloc(4);
	sprintf(lbnlsnbuff,"%d",lbnnow);
	for(int j=SECTOR_SIZE;j<SECTOR_SIZE+4;j++)
	{
		newpagebufff[j]=lbnlsnbuff[j-SECTOR_SIZE];
	}
	memset(lbnlsnbuff,(char)(0xFF),4);
	sprintf(lbnlsnbuff,"%d",lsn);
	for(int j=SECTOR_SIZE+4;j<SECTOR_SIZE+8;j++)
	{
		newpagebufff[j]=lbnlsnbuff[j-SECTOR_SIZE-4];
	}
	free(lbnlsnbuff);
	////넣을 데이터 세팅 끝
	int index_to_overwrite=0;//overwrite해야할 index번호
	if(addmapt[lbnnow][1]!=-1)//이미 최소 하나가 들어가있는 block이면
	{
		for(int i=0;i<PAGES_PER_BLOCK;i++)
		{
			char*bufforlsn=malloc(4);
			dd_read((addmapt[lbnnow][1]*PAGES_PER_BLOCK)+i,pagebufff);
			int countforlsn=0;
			for(int j=(512/sizeof(char))+4;j<(512/sizeof(char))+8;j++)//lsn복사
			{
				bufforlsn[countforlsn]=pagebufff[j];
				countforlsn++;
			}
			if(atoi(bufforlsn)==lsn)//해당 lsn이 적힌 페이지가 존재함.
			{
				is_there_lsn_already=TRUE;
				index_to_overwrite=i;
				break;
			}
			else
			{
				memset(pagebufff,(char)0xFF,PAGE_SIZE);
			}
			free(bufforlsn);
		
		}
		memset(pagebufff,(char)0xFF,PAGE_SIZE);
		if(is_there_lsn_already==TRUE)//overwrite해줘야할 경우
		{
			for(int j=0;j<PAGES_PER_BLOCK;j++)
			{
				if(j!=index_to_overwrite)
				{
					dd_read((addmapt[lbnnow][1]*PAGES_PER_BLOCK)+j,pagebufff);
					dd_write(spare*PAGES_PER_BLOCK+j,pagebufff);//기존 것을 옮겨줌.
				}
				
				memset(pagebufff,(char)0xFF,PAGE_SIZE);
			}
			memset(pagebufff,(char)0xFF,PAGE_SIZE);
			dd_write((spare*PAGES_PER_BLOCK)+index_to_overwrite,newpagebufff);
			dd_erase(addmapt[lbnnow][1]);//기존것을 erase해줌.
			int tempfornewfree=addmapt[lbnnow][1];
			addmapt[lbnnow][1]=spare;//spare block을 lbn에 새로 할당해줌.
			addmapt[BLOCKS_PER_DEVICE-1][1]=tempfornewfree;//기존 pbn을 freeblock으로
			spare=addmapt[BLOCKS_PER_DEVICE-1][1];
			
		}
		else//overwrite하지 않아도 되는 경우
		{
			for(int j=0;j<PAGES_PER_BLOCK;j++)
			{
				dd_read((addmapt[lbnnow][1]*PAGES_PER_BLOCK)+j,pagebufff);
				if(*pagebufff==-1)//빈자리 찾음
				{
					dd_write((addmapt[lbnnow][1]*PAGES_PER_BLOCK)+j,newpagebufff);//새 데이터를 써줌.
					j=PAGES_PER_BLOCK-1;
				}
				else
				{
					memset(pagebufff,(char)0xFF,PAGE_SIZE);
				}
			}
			memset(pagebufff,(char)0xFF,PAGE_SIZE);
		}
	
	}
	else//비어있는(table에 할당이 안된)lbn이라면
	{
		for(int j=0;j<BLOCKS_PER_DEVICE;j++)
		{
			if(j!=spare)
			{
				int is_pbn_used=0;
				for(int k=0;k<DATABLKS_PER_DEVICE;k++)
				{
					if(addmapt[k][1]==j)
					{
						is_pbn_used=1;
					}
				}
				if(is_pbn_used==0)
				{
					addmapt[lbnnow][1]=j;//lbn에 새 pbn할당.
				    dd_write(addmapt[lbnnow][1]*PAGES_PER_BLOCK,newpagebufff);
				    j=BLOCKS_PER_DEVICE-1;
				    break;
				}
				
			}
		}
	}
	free(pagebufff);
	free(newpagebufff);
	free(emptpage);
	return;
}

void ftl_print()
{
	printf("LSN PBN\n");
	for(int i=0;i<DATABLKS_PER_DEVICE;i++)
	{
		printf("%d %d\n",addmapt[i][0],addmapt[i][1]);
	}
	printf("free block's pbn: %d\n",addmapt[BLOCKS_PER_DEVICE-1][1]);
	return;
}