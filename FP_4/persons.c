#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "person.h"
//필요한 경우 헤더 파일과 함수를 추가할 수 있음

// 과제 설명서대로 구현하는 방식은 각자 다를 수 있지만 약간의 제약을 둡니다.
// 레코드 파일이 페이지 단위로 저장 관리되기 때문에 사용자 프로그램에서 레코드 파일로부터 
// 데이터를 읽고 쓸 때도 페이지 단위를 사용합니다. 따라서 아래의 두 함수가 필요합니다.

// 레코드 파일에서 기존의 레코드를 읽거나 새로운 레코드를 쓰거나 삭제 레코드를 수정할 때나
// 위의 readPage() 함수를 호출하여 pagebuf에 저장한 후, 여기에 필요에 따라서 
// 새로운 레코드를 저장하거나 삭제 레코드 관리를 위한 메타데이터를 저장합니다. 
// 그리고 난 후 writePage() 함수를 호출하여 수정된 pagebuf를
// 레코드 파일에 저장합니다. 반드시 페이지 단위로 읽거나 써야 합니다.
//
// 주의: 데이터 페이지로부터 레코드(삭제 레코드 포함)를 읽거나 쓸 때 
// 페이지 단위로 I/O를 처리해야 하지만, 헤더 레코드의 메타데이터를 저장하거나 수정하는 경우
// 페이지 단위로 처리하지 않고 직접 레코드 파일을 접근해서 처리한다.

//
// 페이지 번호에 해당하는 페이지를 주어진 페이지 버퍼에 읽어서 저장한다. 
// 페이지 버퍼는 반드시 페이지 크기와 일치해야 한다.
// 1. readPage(): 주어진 페이지 번호의 페이지 데이터를 프로그램 상으로 읽어와서 
// pagebuf에 저장한다

Person *person;
void readPage(FILE *fp, char *pagebuf, int pagenum)
{
	fseek(fp, PAGE_SIZE * pagenum, SEEK_SET);
	fread(pagebuf, sizeof(char), PAGE_SIZE, fp);
}

//
// 페이지 버퍼의 데이터를 주어진 페이지 번호에 해당하는 레코드 파일의 위치에 저장한다. 
// 페이지 버퍼는 반드시 페이지 크기와 일치해야 한다.
// 2. writePage(): 프로그램 상의 pagebuf의 데이터를 주어진 페이지 번호에 저장한다
void writePage(FILE *fp, const char *pagebuf, int pagenum)
{
	fseek(fp, PAGE_SIZE * pagenum, SEEK_SET);
	fwrite(pagebuf, sizeof(char), PAGE_SIZE, fp);
}

//
// 새로운 레코드를 저장할 때 터미널로부터 입력받은 정보를 Person 구조체에 먼저 저장하고,
// pack() 함수를 사용하여 레코드 파일에 저장할 레코드 형태를 recordbuf에 만든다. 
void pack(char *recordbuf, const Person *p)
{
    sprintf(recordbuf, "%s#%s#%s#%s#%s#%s#", p -> id, p -> name, p -> age,
                                            p -> addr, p -> phone, p -> email);
    memset(recordbuf + strlen(recordbuf), (char)0xFF, MAX_RECORD_SIZE - strlen(recordbuf));
}

// 
// 아래의 unpack() 함수는 recordbuf에 저장되어 있는 레코드를 구조체로 변환할 때 사용한다.
void unpack(const char *recordbuf, Person *p)
{
    sscanf(recordbuf, "%[^'#']#%[^'#']#%[^'#']#%[^'#']#%[^'#']#%[^'#']#",
            p -> id, p -> name, p -> age, p -> addr, p -> phone, p -> email);
}

void setHead(char *pagebuf, int pageNum, int recordNum, int offset, int length)
{
    memset(pagebuf, 0xFF, PAGE_SIZE);
    memcpy(pagebuf, &pageNum, sizeof(int));
    memcpy(pagebuf + 4, &recordNum, sizeof(int));
    memcpy(pagebuf + 8, &offset, sizeof(int));
    memcpy(pagebuf + 12, &length, sizeof(int));
}


//
// 새로운 레코드를 저장하는 기능을 수행하며,
//터미널로부터 입력받은 필드값들을 구조체에 저장한 후 아래 함수를 호출한다.
void add(FILE *fp, const Person *p)
{
    char pagebuf[PAGE_SIZE + 1] = {};

    int pageNum = 1;
    int recordNum = 0;
    int offset = 0;
    int length = 0;

    readPage(fp, pagebuf, 0);

    if(strlen(pagebuf) == 0)
    {
        printf("pagebuf == 0 \n");
        setHead(pagebuf, pageNum, recordNum, offset, length);
        writePage(fp, pagebuf, 0);
    }
    else
    {
        memcpy(&pageNum, pagebuf, sizeof(int));
        memcpy(&recordNum, pagebuf + 4, sizeof(int));
        memcpy(&offset, pagebuf + 8, sizeof(int));
        memcpy(&length, pagebuf + 12, sizeof(int));
    }

    char *recordbuf = malloc(sizeof(char) * 100);
    pack(recordbuf, p);
    length += strlen(recordbuf);
    int RecordperPage = PAGE_SIZE / MAX_RECORD_SIZE;

    if(offset == 0)
    {
        int maxRecordNum = RecordperPage * (pageNum - 1);
        if(maxRecordNum == recordNum)
        {
            pageNum++;
            recordNum++;
            setHead(pagebuf, pageNum, recordNum, offset, length);
            writePage(fp, pagebuf, 0);

            memset(pagebuf, (char)0xFF, PAGE_SIZE);
            memcpy(pagebuf, recordbuf, MAX_RECORD_SIZE);

            writePage(fp, pagebuf, pageNum - 1);
            printf("maxRecordNum == recordNum\n");
            printf("rn: %d      length: %d\n", recordNum, length);
        }
        else
        {
            int index = recordNum % RecordperPage;
            index = index * MAX_RECORD_SIZE;

            memset(pagebuf, (char)0xFF, PAGE_SIZE);
            readPage(fp, pagebuf, pageNum - 1);

            memcpy(pagebuf + index, recordbuf, MAX_RECORD_SIZE);
            writePage(fp, pagebuf, pageNum - 1);

            recordNum++;
            setHead(pagebuf, pageNum, recordNum, offset, length);
            writePage(fp, pagebuf, 0);
            printf("maxRecordNum != recordNum\n");
            printf("rn: %d      length: %d\n", recordNum, length);
        }
    }
    else
    {
        int index = recordNum % RecordperPage;
        int nextDeletePage, nextDeleteRecord;
        index = index * MAX_RECORD_SIZE;

        memset(pagebuf, (char)0xFF, PAGE_SIZE);
        readPage(fp, pagebuf, offset);

        memcpy(&nextDeletePage, pagebuf + index + 1, sizeof(int));
        memcpy(&nextDeleteRecord, pagebuf + index + 5, sizeof(int));
        memcpy(pagebuf + index, recordbuf, MAX_RECORD_SIZE);
        writePage(fp, pagebuf, offset);

        recordNum++;
        setHead(pagebuf, pageNum, recordNum, offset, length);
        writePage(fp, pagebuf, 0);
        printf("offset != 0 \n");
        printf("rn: %d      length: %d\n", recordNum, length);
    }
}


//
// 주민번호와 일치하는 레코드를 찾아서 삭제하는 기능을 수행한다.
void delete(FILE *fp, const char *id)
{
    Person *person = (Person*)malloc(sizeof(Person));

    char pagebuf[PAGE_SIZE + 1] = { };
 
    int pageNum, recordNum, offset, length ;
    readPage(fp, pagebuf, 0);
    
    memcpy(&pageNum, pagebuf, sizeof(int));
    memcpy(&recordNum, pagebuf + 4, sizeof(int));
    memcpy(&offset, pagebuf + 8, sizeof(int));
    memcpy(&length, pagebuf + 12, sizeof(int));
 
    for( int i = 1; i < pageNum; i++) 
    {
        memset(pagebuf, (char)0xFF, PAGE_SIZE);
        readPage(fp, pagebuf, i);
        printf("i\n");
        for (int j = 0; j < PAGE_SIZE / MAX_RECORD_SIZE; j++) 
        {
            int index = j * MAX_RECORD_SIZE;
            char tempRecord[MAX_RECORD_SIZE + 1];
            memcpy(tempRecord, pagebuf + index, MAX_RECORD_SIZE);
            unpack(tempRecord, person);
            printf("j\n");
            if(!strcmp(person -> id, id))   // id가 가리키는 값이 같으면
            {
                memcpy(pagebuf + index, "*", 1);    // 삭제 레코드의 첫번째 바이트는 delete mark
                memcpy(pagebuf + index + 1, &offset, sizeof(int));   
                memcpy(pagebuf + index + 5, &length, sizeof(int)); 
                writePage(fp, pagebuf, i);
 
                offset = i;
                recordNum++;

                setHead(pagebuf, pageNum, recordNum, offset, length);
                writePage(fp, pagebuf, 0);
                printf("delete complete\n");
                printf("rn: %d      length: %d\n", recordNum, length);
                exit(1);
            }
        }
    }
}

int main(int argc, char *argv[])
{
    person = (Person*)malloc(sizeof(Person));

	FILE *fp;  // 레코드 파일의 파일 포인터

    if((fp = fopen(argv[2], "r+")) == NULL)
        fp = fopen(argv[2], "wb");

    
    if(!strcmp(argv[1], "a")) 
    {
        strcpy(person -> id, argv[3]);
        strcpy(person -> name, argv[4]);
        strcpy(person -> age, argv[5]);
        strcpy(person -> addr, argv[6]);
        strcpy(person -> phone, argv[7]);
        strcpy(person -> email, argv[8]);

        add(fp, person);
    }

    else if(!strcmp(argv[1], "d"))
    {
        delete(fp, argv[3]);
    }

    fclose(fp);
	return 1;
}

