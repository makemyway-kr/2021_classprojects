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

int **addmapt;//address mapping table
int spare;//spare block초기값은 제일 끝의 블럭을 사용함.
void ftl_open()
{
	addmapt=malloc(sizeof(int*)*BLOCKS_PER_DEVICE);
	for(int i=0;i<BLOCKS_PER_DEVICE;i++)//address maping table 초기화.
	{
		addmapt[i]=malloc(sizeof(int)*2);
		addmapt[i][0]=i;//lbn넣어줌.
	}
	int countof0xff=0;
	char* buf = (char *)malloc(PAGE_SIZE);
	for(int i=0;i<PAGES_PER_BLOCK*BLOCKS_PER_DEVICE;i++)
	{
		dd_read(i,buf);
		if(*buf==-1)
		{
			countof0xff++;
		}
		memset(buf,'\0',PAGE_SIZE);
	}
	if(countof0xff==PAGES_PER_BLOCK*BLOCKS_PER_DEVICE)//기존 flash memory file이 없어 0xff로 모두 초기화되어있는경우
	{
		for(int i=0;i<BLOCKS_PER_DEVICE;i++)
		{
			addmapt[i][1]=-1;//pbn -1로 초기화
		}
		spare=BLOCKS_PER_DEVICE-1;//spare block index의 초기값은 제일 끝의 블럭을 사용함.
	}
	else//flash memory file이 존재.
	{
		for(int i=0;i<BLOCKS_PER_DEVICE;i++)
		{
			dd_read(i*PAGES_PER_BLOCK,buf);
			char*bufforlbn=malloc(4);
			int countlbnindex=0;
			if(*buf!=-1)//block이 할당이 되어있으면
			{
				for(int i=(512/sizeof(char));i<(512/sizeof(char))+4;i++)
				{
					bufforlbn[countlbnindex]=buf[i];
					countlbnindex++;
				}
				addmapt[atoi(bufforlbn)][1]=i;
			}
			free(bufforlbn);
		}
		for(int i=0;i<BLOCKS_PER_DEVICE;i++)
		{
			if(addmapt[i][1]==-1)
			{
				spare=i;
				i=BLOCKS_PER_DEVICE-1;//비어있는 pbn중 가장 앞의것을 spare로 지정함.
			}
		}
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
			memset(pagebufff,-1,PAGE_SIZE);
		}
		free(bufforlsn);
		
	}
	for(int j=0;j<SECTOR_SIZE;j++)
	{
		sectorbuf[j]=pagebufff[j];
	}
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
	char*pagebufff=malloc(PAGE_SIZE);//page를 받아오는 buffer
	int index_to_overwrite=0;
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
				memset(pagebufff,-1,PAGE_SIZE);
			}
			free(bufforlsn);
		
		}
		memset(pagebufff,-1,PAGE_SIZE);
		if(is_there_lsn_already==TRUE)//overwrite해줘야할 경우
		{
			for(int j=0;j<PAGES_PER_BLOCK;j++)
			{
				if(j!=index_to_overwrite)
				{
					dd_read(addmapt[lbnnow][1]*PAGES_PER_BLOCK+j,pagebufff);
					dd_write(spare*PAGES_PER_BLOCK+j,pagebufff);
				}
				
				memset(pagebufff,-1,PAGE_SIZE);
			}
			memset(pagebufff,-1,PAGE_SIZE);
			for(int j=0;j<SECTOR_SIZE;j++)//섹터 복사
			{
				pagebufff[j]=sectorbuf[j];
			}
			char*lbnlsnbuff=malloc(4);
			memcpy(lbnlsnbuff,lbnnow,4);//lbn복사
			for(int j=SECTOR_SIZE;j<SECTOR_SIZE+4;j++)
			{
				pagebufff[j]=lbnlsnbuff[j];
			}
			memset(lbnlsnbuff,-1,4);
			memcpy(lbnlsnbuff,lsn,4);//lsn복사
			for(int j=SECTOR_SIZE+4;j<SECTOR_SIZE+8;j++)
			{
				pagebufff[j]=lbnlsnbuff[j];
			}
			dd_wrtie(spare*PAGES_PER_BLOCK+index_to_overwrite,pagebuff);
			dd_erase(addmapt[lbnnow][1]);//기존것을 erase해줌.
			addmapt[lbnnow][1]=spare;//spare block을 lbn에 새로 할당해줌.
			free(lbnlsnbuff);
		}
		else//overwrite하지 않아도 되는 경우
		{
			for(int j=0;j<PAGES_PER_BLOCK;j++)
			{
				dd_read(addmapt[lbnnow][1]*PAGES_PER_BLOCK+j,pagebufff);
				if(*pagebufff==-1)//빈자리 찾음
				{

				}
				else
				{
					memset(pagebufff,-1,PAGE_SIZE);
				}
			}
		}
	
	}
	else//비어있는(table에 할당이 안된)lbn이라면
	{

	}
	free(pagebufff);
	return;
}

void ftl_print()
{

	return;
}