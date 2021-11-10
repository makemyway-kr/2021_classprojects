#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "person.h"
Person ps[1000];
int records_per_pages=(HEADER_AREA_SIZE-4)/8;
char records[1000*((HEADER_AREA_SIZE-4)/8)][MAX_RECORD_SIZE];
int meta[4];
int *numbers;//page별 레코드 수.
int *lengths;//레코드별 길이
int *offsets;//레코드 오프셋
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

void pack(char *recordbuf, const Person *p)
{
	sprintf(recordbuf, "%s#%s#%s#%s#%s#%s#", p -> id, p -> name, p -> age,p -> addr, p -> phone, p -> email);
    memset(recordbuf + strlen(recordbuf), (char)0xFF, MAX_RECORD_SIZE - strlen(recordbuf));
}

void unpack(char *recordbuf, Person *p)
{
	sscanf(recordbuf, "%[^'#']#%[^'#']#%[^'#']#%[^'#']#%[^'#']#%[^'#']#",p-> id, p -> name, p -> age, p -> addr, p -> phone, p -> email);
}

void open_records(FILE*fp)
{
	if(fp!=NULL)
	{
		fread(&meta[0],sizeof(int),1,fp);
		fread(&meta[1],sizeof(int),1,fp);
		fread(&meta[2],sizeof(int),1,fp);
		fread(&meta[3],sizeof(int),1,fp);
		lengths=malloc(sizeof(int)*records_per_pages*1000);
		numbers=malloc(sizeof(int)*1000);
		offsets=malloc(sizeof(int)*records_per_pages*1000);
		int offset_left=meta[1];
		int curr=0;
		for(int page=0;page<meta[0];page++)
		{
			char *pagebuff=malloc(sizeof(char)*PAGE_SIZE);
			readPage(fp,pagebuff,page);
			char *temp=malloc(sizeof(char)*4);
			for(int j=0;j<4;j++)
			{
				temp[j]=pagebuff[j];
			}
			numbers[page]=atoi(temp);
			if(offset_left>records_per_pages)
			{
				for(int i=0;i<numbers[page];i++)
				{		
					memset(temp,0,4);
					for(int j=0;j<4;j++)
					{
						temp[j]=pagebuff[(i*8)+j+4];
					}
					offsets[curr]=atoi(temp);
					memset(temp,0,4);
					for(int j=0;j<4;j++)
					{
						temp[j]=pagebuff[(i*8)+j+8];
					}
					lengths[curr]=atoi(temp);
					offset_left-=1;
					curr++;
				}
				for(int i=0;i<numbers[page];i++)
				{
					for(int j=0;j<lengths[curr-records_per_pages+i]+6;j++)
					{
						if(i==0)
						{
							records[curr-records_per_pages+i][j]=pagebuff[HEADER_AREA_SIZE+j];
						}
						else
						{
							records[curr-records_per_pages+i][j]=pagebuff[HEADER_AREA_SIZE+lengths[curr-records_per_pages+i-1]+j];
						}
					}
					unpack(records[curr-records_per_pages+i],&ps[curr-records_per_pages+i]);
				}
			}
			else
			{
				for(int i=0;i<offset_left;i++)
				{
					memset(temp,0,4);
					for(int j=0;j<4;j++)
					{
						temp[j]=pagebuff[(i*8)+j+4];
					}
					offsets[curr]=atoi(temp);
					memset(temp,0,4);
					for(int j=0;j<4;j++)
					{
						temp[j]=pagebuff[(i*8)+j+8];
					}
					lengths[curr]=atoi(temp);
					curr++;
				}
				for(int i=0;i<numbers[page];i++)
				{
					for(int j=0;j<lengths[curr-records_per_pages+i]+6;j++)
					{
						if(i==0)
						{
							records[curr-offset_left+i][j]=pagebuff[HEADER_AREA_SIZE+j];
						}
						else
						{
							records[curr-offset_left+i][j]=pagebuff[HEADER_AREA_SIZE+lengths[curr-offset_left+i-1]+j];
						}
					}
					unpack(records[curr-offset_left+i],&ps[curr-offset_left+i]);
				}
			}
			free(pagebuff);
		}
	}
	else
	{
		meta[0]=0;
		meta[1]=0;
		meta[2]=-1;
		meta[3]=-1;
	}
}
void close_record(FILE*fp)
{
	fseek(fp,0,SEEK_SET);
	fwrite(&meta[0],sizeof(int),1,fp);
	fwrite(&meta[1],sizeof(int),1,fp);
	fwrite(&meta[2],sizeof(int),1,fp);
	fwrite(&meta[3],sizeof(int),1,fp);
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
			char*pagebuff=malloc(sizeof(char)*(PAGE_SIZE));
			char*temp=malloc(sizeof(char)*4);
			memset(temp,(char)0xFF,4);
			sprintf(temp,"%d",numbers[pa]);
			strcat(pagebuff,temp);
			for(int i=0;i<records_per_pages;i++)
			{
				memset(temp,(char)0xFF,4);
				sprintf(temp,"%d",offsets[curr-records_per_pages+i]);
				strcat(pagebuff,temp);
				memset(temp,(char)0xFF,4);
				sprintf(temp,"%d",lengths[curr-records_per_pages+i]);
				strcat(pagebuff,temp);
			}
			free(temp);
			char*temp2=malloc(sizeof(char)*(HEADER_AREA_SIZE-(4+(8*records_per_pages))));
			memset(temp2,(char)0xFF,sizeof(temp2));
			strcat(pagebuff,temp2);
			free(temp2);
			for(int i=curr-records_per_pages;i<curr;i++)
			{
				char*recordbuff=malloc(sizeof(char)*lengths[i]);
				pack(recordbuff,&ps[i]);
				strcat(pagebuff,recordbuff);
				free(recordbuff);
			}
			char*temp3=malloc(sizeof(char)*(DATA_AREA_SIZE-lengthofrecs));
			memset(temp3,(char)0xFF,sizeof(temp3));
			strcat(pagebuff,temp3);
			free(temp3);
			writePage(fp,pagebuff,pa);
			free(pagebuff);
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
			char*temp=malloc(sizeof(char)*4);
			memset(temp,(char)0xFF,sizeof(temp));
			sprintf(temp,"%d",numbers[pa]);
			strcat(pagebuff,temp);
			for(int i=0;i<records_per_pages;i++)
			{
				memset(temp,(char)0xFF,sizeof(temp));
				sprintf(temp,"%d",offsets[curr-numbers[pa]+i]);
				strcat(pagebuff,temp);
				memset(temp,(char)0xFF,sizeof(temp));
				sprintf(temp,"%d",lengths[curr-numbers[pa]+i]);
				strcat(pagebuff,temp);
			}
			free(temp);
			char*temp2=malloc(sizeof(char)*(HEADER_AREA_SIZE-(4+(8*records_per_pages))));
			memset(temp2,(char)0xFF,sizeof(temp2));
			strcat(pagebuff,temp2);
			free(temp2);
			for(int i=curr-numbers[pa];i<curr;i++)
			{
				char*recordbuff=malloc(sizeof(char)*lengths[i]);
				pack(recordbuff,&ps[i]);
				strcat(pagebuff,recordbuff);
				free(recordbuff);
			}
			char*temp3=malloc(sizeof(char)*(DATA_AREA_SIZE-lengthofrecs));
			memset(temp3,(char)0xFF,sizeof(temp3));
			strcat(pagebuff,temp3);
			free(temp3);
			writePage(fp,pagebuff,pa);
			free(pagebuff);
		}
		
	}
	free(lengths);
	free(numbers);
	free(offsets);
}

///
//first fit을 만족하는 record를 찾아낸다.
///
void search_first(int length,int *stp,int *str,int *page,int *record)//
{
	while(*page!=-1 && *record!=-1)
	{
		if(lengths[((*page)*records_per_pages)+(*record)]>=length+6)
		{
			break;
		}
		else
		{
			*stp=*page;
			*str=*record;
			const char *temp=&ps[((*page)*records_per_pages)+(*record)].id[1];
			*page=atoi(temp);
			temp=&ps[((*page)*records_per_pages)+(*record)].id[2];
			*record=atoi(temp);
		}
	}
}
void add(FILE *fp, const Person *p)
{
	int recordlen=strlen(p->addr)+strlen(p->id)+strlen(p->name)+strlen(p->phone)+strlen(p->email)+strlen(p->age);
	if(meta[2]==-1)//삭제된 레코드가 존재하지 않는 경우
	{
		if(meta[1]%records_per_pages!=0)//제일 끝 페이지에 레코드 하나를 더 넣어줄 수 있는 경우.
		{
			meta[1]+=1;
			numbers[meta[0]]+=1;
			lengths[meta[1]-1]=recordlen;
			offsets[meta[1]-1]=meta[1]-1;
			strcpy(ps[meta[1]-1].addr,p->addr);
			strcpy(ps[meta[1]-1].id,p->id);
			strcpy(ps[meta[1]-1].name,p->name);
			strcpy(ps[meta[1]-1].phone,p->phone);
			strcpy(ps[meta[1]-1].email,p->email);
			strcpy(ps[meta[1]-1].age,p->age);
		}
		else
		{
			meta[0]+=1;
			meta[1]+=1;
			numbers[meta[0]-1]=1;
			lengths[meta[1]-1]=recordlen;
			offsets[meta[1]-1]=meta[1]-1;
			strcpy(ps[meta[1]-1].addr,p->addr);
			strcpy(ps[meta[1]-1].id,p->id);
			strcpy(ps[meta[1]-1].name,p->name);
			strcpy(ps[meta[1]-1].phone,p->phone);
			strcpy(ps[meta[1]-1].email,p->email);
			strcpy(ps[meta[1]-1].age,p->age);
		}
	}
	else//삭제된 레코드 존재
	{
		int stp=meta[2];//이전 페이지와 레코드 넘버
		int str=meta[2];
		int pagenum=meta[2];
		int recordnum=meta[3];
		search_first(recordlen,&stp,&str,&pagenum,&recordnum);
		if(pagenum==-1 || recordnum==-1)//fit하는 record가 존재하지 않으면
		{
			if(meta[1]%records_per_pages!=0)//제일 끝 페이지에 레코드 하나를 더 넣어줄 수 있는 경우.
			{
				meta[1]+=1;
				numbers[meta[0]]+=1;
				lengths[meta[1]-1]=recordlen;
				offsets[meta[1]-1]=meta[1]-1;
				strcpy(ps[meta[1]-1].addr,p->addr);
				strcpy(ps[meta[1]-1].id,p->id);
				strcpy(ps[meta[1]-1].name,p->name);
				strcpy(ps[meta[1]-1].phone,p->phone);
				strcpy(ps[meta[1]-1].email,p->email);
				strcpy(ps[meta[1]-1].age,p->age);
			}
			else
			{
				meta[0]+=1;
				meta[1]+=1;
				numbers[meta[0]-1]=1;
				lengths[meta[1]-1]=recordlen;
				offsets[meta[1]-1]=meta[1]-1;
				strcpy(ps[meta[1]-1].addr,p->addr);
				strcpy(ps[meta[1]-1].id,p->id);
				strcpy(ps[meta[1]-1].name,p->name);
				strcpy(ps[meta[1]-1].phone,p->phone);
				strcpy(ps[meta[1]-1].email,p->email);
				strcpy(ps[meta[1]-1].age,p->age);
			}
		}
		else
		{
			if(pagenum==meta[2] && recordnum==meta[3])//메타 바로 다음(가장 나중에 삭제된 레코드)에 넣어주는 경우.
			{
				if(ps[((pagenum)*records_per_pages)+(recordnum)].id[1]==(char)(-1))//제일 끝에 넣어주는 경우.
				{
					meta[2]=-1;
					meta[3]=-1;
					memset(ps[((pagenum)*records_per_pages)+(recordnum)].id,(char)0xFF,sizeof(ps[((pagenum)*records_per_pages)+(recordnum)].id));
					memset(ps[((pagenum)*records_per_pages)+(recordnum)].name,(char)0xFF,sizeof(ps[((pagenum)*records_per_pages)+(recordnum)].name));
					memset(ps[((pagenum)*records_per_pages)+(recordnum)].phone,(char)0xFF,sizeof(ps[((pagenum)*records_per_pages)+(recordnum)].phone));
					memset(ps[((pagenum)*records_per_pages)+(recordnum)].email,(char)0xFF,sizeof(ps[((pagenum)*records_per_pages)+(recordnum)].email));
					memset(ps[((pagenum)*records_per_pages)+(recordnum)].addr,(char)0xFF,sizeof(ps[((pagenum)*records_per_pages)+(recordnum)].addr));
					memset(ps[((pagenum)*records_per_pages)+(recordnum)].age,(char)0xFF,sizeof(ps[((pagenum)*records_per_pages)+(recordnum)].age));
					strcpy(ps[((pagenum)*records_per_pages)+(recordnum)].id,p->id);
					strcpy(ps[((pagenum)*records_per_pages)+(recordnum)].name,p->name);
					strcpy(ps[((pagenum)*records_per_pages)+(recordnum)].phone,p->phone);
					strcpy(ps[((pagenum)*records_per_pages)+(recordnum)].email,p->email);
					strcpy(ps[((pagenum)*records_per_pages)+(recordnum)].addr,p->addr);
					strcpy(ps[((pagenum)*records_per_pages)+(recordnum)].age,p->age);
				}
				else//삭제 레코드가 이후로 더 있음.
				{
					meta[2]=ps[((pagenum)*records_per_pages)+(recordnum)].id[1];
					meta[3]=ps[((pagenum)*records_per_pages)+(recordnum)].id[2];
					memset(ps[((pagenum)*records_per_pages)+(recordnum)].id,(char)0xFF,sizeof(ps[((pagenum)*records_per_pages)+(recordnum)].id));
					memset(ps[((pagenum)*records_per_pages)+(recordnum)].name,(char)0xFF,sizeof(ps[((pagenum)*records_per_pages)+(recordnum)].name));
					memset(ps[((pagenum)*records_per_pages)+(recordnum)].phone,(char)0xFF,sizeof(ps[((pagenum)*records_per_pages)+(recordnum)].phone));
					memset(ps[((pagenum)*records_per_pages)+(recordnum)].email,(char)0xFF,sizeof(ps[((pagenum)*records_per_pages)+(recordnum)].email));
					memset(ps[((pagenum)*records_per_pages)+(recordnum)].addr,(char)0xFF,sizeof(ps[((pagenum)*records_per_pages)+(recordnum)].addr));
					memset(ps[((pagenum)*records_per_pages)+(recordnum)].age,(char)0xFF,sizeof(ps[((pagenum)*records_per_pages)+(recordnum)].age));
					strcpy(ps[((pagenum)*records_per_pages)+(recordnum)].id,p->id);
					strcpy(ps[((pagenum)*records_per_pages)+(recordnum)].name,p->name);
					strcpy(ps[((pagenum)*records_per_pages)+(recordnum)].phone,p->phone);
					strcpy(ps[((pagenum)*records_per_pages)+(recordnum)].email,p->email);
					strcpy(ps[((pagenum)*records_per_pages)+(recordnum)].addr,p->addr);
					strcpy(ps[((pagenum)*records_per_pages)+(recordnum)].age,p->age);
				}
				
			}
			else
			{
				ps[((stp)*records_per_pages)+str].id[1]=ps[((pagenum)*records_per_pages)+(recordnum)].id[1];
				ps[((stp)*records_per_pages)+str].id[2]=ps[((pagenum)*records_per_pages)+(recordnum)].id[2];
				memset(ps[((pagenum)*records_per_pages)+(recordnum)].id,(char)0xFF,sizeof(ps[((pagenum)*records_per_pages)+(recordnum)].id));
				memset(ps[((pagenum)*records_per_pages)+(recordnum)].name,(char)0xFF,sizeof(ps[((pagenum)*records_per_pages)+(recordnum)].name));
				memset(ps[((pagenum)*records_per_pages)+(recordnum)].phone,(char)0xFF,sizeof(ps[((pagenum)*records_per_pages)+(recordnum)].phone));
				memset(ps[((pagenum)*records_per_pages)+(recordnum)].email,(char)0xFF,sizeof(ps[((pagenum)*records_per_pages)+(recordnum)].email));
				memset(ps[((pagenum)*records_per_pages)+(recordnum)].addr,(char)0xFF,sizeof(ps[((pagenum)*records_per_pages)+(recordnum)].addr));
				memset(ps[((pagenum)*records_per_pages)+(recordnum)].age,(char)0xFF,sizeof(ps[((pagenum)*records_per_pages)+(recordnum)].age));
				strcpy(ps[((pagenum)*records_per_pages)+(recordnum)].id,p->id);
				strcpy(ps[((pagenum)*records_per_pages)+(recordnum)].name,p->name);
				strcpy(ps[((pagenum)*records_per_pages)+(recordnum)].phone,p->phone);
				strcpy(ps[((pagenum)*records_per_pages)+(recordnum)].email,p->email);
				strcpy(ps[((pagenum)*records_per_pages)+(recordnum)].addr,p->addr);
				strcpy(ps[((pagenum)*records_per_pages)+(recordnum)].age,p->age);
			}
		}
	}
}


void delete(FILE *fp, const char *id)
{
	for(int reco=0;reco<meta[1];reco++)
	{
		if(strcmp(ps[reco].id,id)==0)
		{
			if(meta[2]==-1)//삭제된 레코드가 존재하지 않을 경우.
			{
				meta[2]=reco/records_per_pages;
				meta[3]=reco%records_per_pages;
				ps[reco].id[0]='*';
				ps[reco].id[1]=(char)(-1);
				ps[reco].id[2]=(char)(-1);
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
	//받은 매개변수를 unpack해서 tp에 넣어주어야함.
	if(*argv[1]=='a')
	{
		Person *tp=malloc(sizeof(Person));
		fp=fopen(argv[2],"a+b");
		open_records(fp);
		strcpy(tp->id,argv[3]);
		strcpy(tp->name,argv[4]);
		strcpy(tp->age,argv[5]);
		strcpy(tp->addr,argv[6]);
		strcpy(tp->phone,argv[7]);
		strcpy(tp->email,argv[8]);
		add(fp,tp);
	}
	else if(*argv[1]=='d')
	{
		fp=fopen(argv[2],"a+b");
		open_records(fp);
		delete(fp,argv[3]);
	}
	close_record(fp);
	return 1;
}