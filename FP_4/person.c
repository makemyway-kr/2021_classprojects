#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "person.h"
Person ps[100];
int records_per_pages=(HEADER_AREA_SIZE-4)/8;
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

void open_records(FILE*fp)//헤더 입력받음.
{
	meta[0]=0;
	meta[1]=0;
	meta[2]=-1;
	meta[3]=-1;
	numbers=malloc(sizeof(int)*100);
	lengths=malloc(sizeof(int)*records_per_pages);
	offsets=malloc(sizeof(int)*records_per_pages);
	char*buff_to_get=malloc(sizeof(char)*100);
	fread(buff_to_get,4,1,fp);
	if(strlen(buff_to_get)>0)
	{
		fseek(fp,0,SEEK_SET);
		fread(&meta[0],sizeof(int),1,fp);
		fread(&meta[1],sizeof(int),1,fp);
		fread(&meta[2],sizeof(int),1,fp);
		fread(&meta[3],sizeof(int),1,fp);
		int rec=0;
		for(int page=0;page<meta[0];page++)
		{
			int strec=rec;
			char*pagebuff=malloc(sizeof(char)*PAGE_SIZE);
			readPage(fp,pagebuff,page);
			memcpy(&numbers[page],pagebuff,sizeof(int));
			int counter=0;
			for(rec;rec<(page*records_per_pages)+records_per_pages;rec++)
			{
				if(rec>meta[1])
				{
					break;
				}
				else
				{
					memcpy(&offsets[rec],pagebuff+(8*counter)+4,sizeof(int));
					memcpy(&lengths[rec],pagebuff+(8*counter)+4+4,sizeof(int));
					counter++;
				}
			}
			counter=0;
			for(strec;strec<(page*records_per_pages)+records_per_pages;strec++)
			{
				if(strec>meta[1])
				{
					break;
				}
				else
				{
					char*recordbuff=malloc(sizeof(char)*lengths[strec]+6);
					if(counter==0)
					{
						memcpy(recordbuff,pagebuff+HEADER_AREA_SIZE,sizeof(char)*lengths[strec]+6);
						unpack(recordbuff,&ps[strec]);
						counter++;
					}
					else
					{
						memcpy(recordbuff,pagebuff+HEADER_AREA_SIZE+lengths[strec-1],sizeof(char)*lengths[strec]+6);
						unpack(recordbuff,&ps[strec]);
						counter++;
					}
					free(recordbuff);
				}
				
			}
			counter=0;
			free(pagebuff);
		}
	}
	free(buff_to_get);
}
void close_record(FILE*fp)
{
	fseek(fp,0,SEEK_SET);
	fwrite(&meta[0],sizeof(int),1,fp);
	fwrite(&meta[1],sizeof(int),1,fp);
	fwrite(&meta[2],sizeof(int),1,fp);
	fwrite(&meta[3],sizeof(int),1,fp);
	int rec=0;
	for(int i=0;i<meta[0];i++)
	{
		char *pagebuff=malloc(sizeof(char)*PAGE_SIZE);
		memset(pagebuff,(char)0xFF,sizeof(char)*PAGE_SIZE);
		memcpy(pagebuff,&numbers[i],sizeof(int));
		int counter=0;
		int strec=rec;
		for(rec;rec<(i*records_per_pages)+records_per_pages;rec++)
		{
			if(rec<=meta[1])
			{
				memcpy(pagebuff+(counter*8)+4,&offsets[rec],sizeof(int));
				memcpy(pagebuff+(counter*8)+8,&lengths[rec],sizeof(int));
			}
			else
			{
				break;
			}
		}
		for(strec;strec<(i*records_per_pages)+records_per_pages;strec++)
		{
			if(strec<=meta[1])
			{
				char*recbuff=malloc(sizeof(char)*lengths[strec]);
				pack(recbuff,&ps[strec]);
				if(counter==0)
				{
					memcpy(pagebuff+HEADER_AREA_SIZE,recbuff,sizeof(char)*lengths[strec]);
					counter++;
				}
				else
				{
					memcpy(pagebuff+HEADER_AREA_SIZE+lengths[strec-1],recbuff,sizeof(char)*lengths[strec]);
					counter++;
				}
				free(recbuff);
			}
		}
		writePage(fp,pagebuff,i);
		free(pagebuff);
	}
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
	int append=0;
	int originmeta2=meta[2];
	if(meta[0]==0)//파일이 비어있는 경우
	{
		numbers[meta[0]]=1;
		lengths[meta[1]]=recordlen;
		offsets[meta[1]]=meta[1];
		strcpy(ps[meta[1]].id,p->id);
		strcpy(ps[meta[1]].name,p->name);
		strcpy(ps[meta[1]].age,p->age);
		strcpy(ps[meta[1]].addr,p->addr);
		strcpy(ps[meta[1]].phone,p->phone);
		strcpy(ps[meta[1]].email,p->email);
		meta[0]+=1;
		meta[1]+=1;
	}
	else//파일에 적어도 하나가 있는 경우
	{
		if(meta[2]!=-1)//삭제된 레코드가 없는 경우.
		{
			int stp=meta[2]; int str=meta[3]; int page=meta[2]; int record=meta[3];
			search_first(recordlen,&stp,&str,&page,&record);
			if(page==meta[2])//제일 끝에 삭제된 데에 저장할 경우.
			{
				meta[2]=atoi(&ps[(meta[2]*records_per_pages)+meta[3]].id[1]);
				meta[3]=atoi(&ps[(meta[2]*records_per_pages)+meta[3]].id[2]);
			}
			else if(page!=-1)
			{
				ps[(stp*records_per_pages)+str].id[1]=ps[(page*records_per_pages)+record].id[1];
				ps[(stp*records_per_pages)+str].id[2]=ps[(page*records_per_pages)+record].id[2];
			}
			if(page!=-1)
			{
				strcpy(ps[(page*records_per_pages)+record].id,p->id);
				strcpy(ps[(page*records_per_pages)+record].name,p->name);
				strcpy(ps[(page*records_per_pages)+record].age,p->age);
				strcpy(ps[(page*records_per_pages)+record].addr,p->addr);
				strcpy(ps[(page*records_per_pages)+record].phone,p->phone);
				strcpy(ps[(page*records_per_pages)+record].email,p->email);
			}
			else//넣을 수 있는 공간이 없는 경우.
			{
				append=1;
			}

		}
		if(originmeta2==-1 || append==1)
		{
			if(meta[1]%records_per_pages!=0)
			{
				numbers[meta[0]]+=1;
				lengths[meta[1]]=recordlen;
				offsets[meta[1]]=meta[1];
				strcpy(ps[meta[1]].id,p->id);
				strcpy(ps[meta[1]].name,p->name);
				strcpy(ps[meta[1]].age,p->age);
				strcpy(ps[meta[1]].addr,p->addr);
				strcpy(ps[meta[1]].phone,p->phone);
				strcpy(ps[meta[1]].email,p->email);
				meta[1]+=1;
			}
			else//새로운 페이지를 할당해 주어야하는 경우.
			{
				numbers[meta[0]]=1;
				lengths[meta[1]]=recordlen;
				offsets[meta[1]]=meta[1];
				strcpy(ps[meta[1]].id,p->id);
				strcpy(ps[meta[1]].name,p->name);
				strcpy(ps[meta[1]].age,p->age);
				strcpy(ps[meta[1]].addr,p->addr);
				strcpy(ps[meta[1]].phone,p->phone);
				strcpy(ps[meta[1]].email,p->email);
				meta[0]+=1;
				meta[1]+=1;

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