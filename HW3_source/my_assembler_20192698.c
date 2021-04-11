#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include "my_assembler_20192698.h"
/* ----------------------------------------------------------------------------------
 * 설명 : 사용자로 부터 어셈블리 파일을 받아서 명령어의 OPCODE를 찾아 출력한다.
 * 매계 : 실행 파일, 어셈블리 파일 
 * 반환 : 성공 = 0, 실패 = < 0 
 * 주의 : 현재 어셈블리 프로그램의 리스트 파일을 생성하는 루틴은 만들지 않았다. 
 *		   또한 중간파일을 생성하지 않는다. 
 * ----------------------------------------------------------------------------------
 */
int main(int args, char *arg[])
{
	if (init_my_assembler() < 0)
	{
		printf("init_my_assembler: 프로그램 초기화에 실패 했습니다.\n");
		return -1;
	}

	if (assem_pass1() < 0)
	{
		printf("assem_pass1: 패스1 과정에서 실패하였습니다.  \n");
		return -1;
	}

	make_opcode_output("output_20192698");

	/*
	 * 추후 프로젝트에서 사용되는 부분
	 *  
	make_symtab_output("symtab_00000000");
	if (assem_pass2() < 0)
	{
		printf(" assem_pass2: 패스2 과정에서 실패하였습니다.  \n");
		return -1;
	}
	make_objectcode_output("output_00000000");
	*/
	return 0;
}

/* ----------------------------------------------------------------------------------
 * 설명 : 프로그램 초기화를 위한 자료구조 생성 및 파일을 읽는 함수이다. 
 * 매계 : 없음
 * 반환 : 정상종료 = 0 , 에러 발생 = -1
 * 주의 : 각각의 명령어 테이블을 내부에 선언하지 않고 관리를 용이하게 하기 
 *		   위해서 파일 단위로 관리하여 프로그램 초기화를 통해 정보를 읽어 올 수 있도록
 *		   구현하였다. 
 * ----------------------------------------------------------------------------------
 */
int init_my_assembler(void)
{
	int result;
	result = 0;
	if ((result = init_inst_file("inst.data")) < 0)
		return -1;
	if ((result = init_input_file("input.txt")) < 0)
		return -1;
	return result;
}

/* ----------------------------------------------------------------------------------
 * 설명 : 머신을 위한 기계 코드목록 파일을 읽어 기계어 목록 테이블(inst_table)을 
 *        생성하는 함수이다. 
 * 매계 : 기계어 목록 파일
 * 반환 : 정상종료 = 0 , 에러 < 0 
 * 주의 : 기계어 목록파일 형식은 자유롭게 구현한다. 예시는 다음과 같다.
 *	
 *	===============================================================================
 *		   | 이름 | 형식 | 기계어 코드 | 오퍼랜드의 갯수 | NULL|
 *	===============================================================================	   
 *		
 * ----------------------------------------------------------------------------------
 */
int init_inst_file(char *inst_file)
{
	FILE *file=fopen(inst_file,"r");
	int errno;
	errno = 0;
	char buff[100];
	if (file == NULL)errno = -1;//빈 파일이면 에러발생
	char* temp=NULL;
	memset(buff, 0, 15);
	inst_index = 0;
	while (feof(file) == 0)
	{
		fgets(buff, 100, file);
		temp = strtok(buff, "|");
		for (int i = 0; i < 4; i++)
		{ 
			if (temp == NULL)
			{
				errno = -1;
			}
			else if (i == 0)
			{
				strcpy(inst_table[inst_index]->str, temp);
			}
			else if (i == 1)
			{
				inst_table[inst_index]->format = atoi(temp);
			}
			else if (i == 2)
			{
				inst_table[inst_index]->op = (unsigned char)strtol(temp, NULL, 16);
			}
			else if (i == 3)
			{
				inst_table[inst_index]->ops = atoi(temp);
			}
			temp = strtok(NULL, "|");
		}
		printf("%s %u %d %d",inst_table[inst_index]->str, inst_table[inst_index]->op, inst_table[inst_index]->format, inst_table[inst_index]->ops);
		inst_index++;
		errno = 0;
	}
	free(temp);
	fclose(file);
	return errno;
}

/* ----------------------------------------------------------------------------------
 * 설명 : 어셈블리 할 소스코드를 읽어 소스코드 테이블(input_data)를 생성하는 함수이다. 
 * 매계 : 어셈블리할 소스파일명
 * 반환 : 정상종료 = 0 , 에러 < 0  
 * 주의 : 라인단위로 저장한다.
 *		
 * ----------------------------------------------------------------------------------
 */
int init_input_file(char *input_file)
{
	FILE *file=fopen(input_file,"r");
	for (int i= 0; i < MAX_LINES; i++)
	{
		input_data[i] = malloc(sizeof(char) * 100);
		input_data[i] = "\0";
	}
	int errno;
	errno = -1;
	line_num = 0;
	char buff[100];
	memset(buff, 0, 100);
	while (feof(file))
	{
		fgets(buff, 100, file);
		strcpy(input_data[line_num], buff);
		if (buff == NULL)
		{
			errno = -1;
		}
		else
		{
			errno = 0;
		}
		memset(buff, 0, 100);
		line_num++;
	}
	fclose(file);

	return errno;
}

/* ----------------------------------------------------------------------------------
 * 설명 : 소스 코드를 읽어와 토큰단위로 분석하고 토큰 테이블을 작성하는 함수이다. 
 *        패스 1로 부터 호출된다. 
 * 매계 : 파싱을 원하는 문자열  
 * 반환 : 정상종료 = 0 , 에러 < 0 
 * 주의 : my_assembler 프로그램에서는 라인단위로 토큰 및 오브젝트 관리를 하고 있다. 
 * ----------------------------------------------------------------------------------
 */
int token_parsing(char *str)
{
	int err = -1;
	char* temp;
	temp = NULL;
	token_table[token_line]->label = NULL;
	token_table[token_line]->operator=NULL;
	for (int i = 0; i < MAX_OPERAND; i++)
	{
		*token_table[token_line]->operand[i] = NULL;
	}
	*token_table[token_line]->comment = NULL;
	if (str[0] == "\t")//레이블이 없어 tab이 앞에 있는 경우
	{
		temp = strtok(str, "\t");
		int count = 1;
		while (temp != NULL)
		{
			if (temp == NULL || temp == "\t")
			{
				err = -1;
			}
			if (count == 1)
			{
				strcpy(token_table[token_line]->operator,temp);
			}
			else if (count == 2)
			{
				int check[5] = { 0, };
				int c = 1;
				for (int k = 1; k < strlen(str); k++)
				{
					if (str[k] == "\t")
					{
						check[c] = k;
						c++;
					}
				}
				for (int k = 1; k < 4; k++)
				{
					if (check[k] == "\t" && check[k + 1] == "\t")
					{
						c = 1000;
						break;

					}


				}
				if (c == 1000)//두개연속 tab,즉 operand가 존재하지 않는 경우
				{
					strcpy(token_table[token_line]->comment, temp);
				}
				else
				{
					char* t2 = strtok(temp, ",");
					int j = 0;
					while (t2 != NULL)
					{
						strcpy(token_table[token_line]->operand[j], t2);
						t2 = strtok(NULL, ",");
						j++;
					}

				}

			}
			else if (count == 3)
			{
				strcpy(token_table[token_line]->comment, temp);
			}
			temp = strtok(NULL, "\t");
			count++;
		}
		err = 0;
	}
	else if (str[0] == ".")//.으로 시작하는 경우
	{
		strcpy(token_table[token_line]->comment, input_data[token_line]);
		err = 0;
	}
	else//label이 존재하는 경우
	{
		temp = strtok(str, "\t");
		int count = 0;
		while (temp != NULL)
		{
			if (count == 0)
			{
				strcpy(token_table[token_line]->label, temp);
			}
			else if (count == 1)
			{
				strcpy(token_table[token_line]->operator,temp);

			}
			else if (count == 2)
			{
				char* t2 = strtok(temp, ",");
				int j = 0;
				while (t2 != NULL)
				{
					strcpy(token_table[token_line]->operand[j], t2);
					t2 = strtok(NULL, ",");
					j++;
				}
			}
			else
			{
				strcpy(token_table[token_line]->comment, temp);
			}
			temp = strtok(NULL, "\t");
			count++;
		}
		err = 0;
	}

	token_line++;
	return err;

}

/* ----------------------------------------------------------------------------------
 * 설명 : 입력 문자열이 기계어 코드인지를 검사하는 함수이다. 
 * 매계 : 토큰 단위로 구분된 문자열 
 * 반환 : 정상종료 = 기계어 테이블 인덱스, 에러 < 0 
 * 주의 : 
 *		
 * ----------------------------------------------------------------------------------
 */
int search_opcode(char *str)
{
	int an = -1;
	for (int i = 0; i < inst_index; i++)
	{
		if (strcmp(inst_table[i]->str, str) == 0)
		{
			an = i;
			break;
		}
	}
	return an;
}

/* ----------------------------------------------------------------------------------
* 설명 : 어셈블리 코드를 위한 패스1과정을 수행하는 함수이다.
*		   패스1에서는..
*		   1. 프로그램 소스를 스캔하여 해당하는 토큰단위로 분리하여 프로그램 라인별 토큰
*		   테이블을 생성한다.
*
* 매계 : 없음
* 반환 : 정상 종료 = 0 , 에러 = < 0
* 주의 : 현재 초기 버전에서는 에러에 대한 검사를 하지 않고 넘어간 상태이다.
*	  따라서 에러에 대한 검사 루틴을 추가해야 한다.
*
* -----------------------------------------------------------------------------------
*/
static int assem_pass1(void)
{
	int starting_add = 0;
	token_line = 0;
	int count_of_sym = 0;
	for (int i = 0; i < line_num; i++)//token으로 쪼개줌.
	{
		token_parsing(input_data[i]);
		if (token_parsing(input_data[i]) < 0)
		{
			return -1;
			break;
		}
		token_line++;
	}
	return 0;

}

/* ----------------------------------------------------------------------------------
* 설명 : 입력된 문자열의 이름을 가진 파일에 프로그램의 결과를 저장하는 함수이다.
*        여기서 출력되는 내용은 명령어 옆에 OPCODE가 기록된 표(과제 3번) 이다.
* 매계 : 생성할 오브젝트 파일명
* 반환 : 없음
* 주의 : 만약 인자로 NULL값이 들어온다면 프로그램의 결과를 표준출력으로 보내어
*        화면에 출력해준다.
*        또한 과제 3번에서만 쓰이는 함수이므로 이후의 프로젝트에서는 사용되지 않는다.
* -----------------------------------------------------------------------------------
*/
void make_opcode_output(char *file_name)
{
	init_my_assembler();
	assem_pass1();
	if (file_name == NULL)
	{
		for (int i = 0; i < line_num; i++)
		{
			if (token_table[i]->operator==NULL)
			{
				printf("%s\n", input_data[i]);
			}
			else
			{
				if (search_opcode(token_table[i]->operator) == -1)//opcode가 존재하지않는 start, LTORG등일 경우
				{
					printf("%s\n", input_data[i]);
				}
				else
				{
					printf("%s", input_data[i]);
					unsigned char bb = search_opcode(token_table[i]->operator);
					printf("%u\n", bb);
				}
			}
		}
	}
	else
	{
		FILE* f = fopen(file_name, "w");
		for (int i = 0; i < line_num; i++)
		{
			if (token_table[i]->operator==NULL)
			{
				fwrite(input_data[i], strlen(input_data[i]) * sizeof(char), 1, f);
				fwrite("\n", sizeof("\n"), 1, f);
			}
			else
			{
				if (search_opcode(token_table[i]->operator) == -1)//opcode가 존재하지않는 start, LTORG등일 경우
				{
					fwrite(input_data[i], strlen(input_data[i]) * sizeof(char), 1, f);
					fwrite("\n", sizeof("\n"), 1, f);
				}
				else
				{
					fwrite(input_data[i], strlen(input_data[i]) * sizeof(char), 1, f);
					unsigned char bb = search_opcode(token_table[i]->operator);
					fwrite(bb, sizeof(bb), 1, f);
					fwrite("\n", sizeof("\n"), 1, f);
				}
			}
		}
	}
	
}

/* ----------------------------------------------------------------------------------
* 설명 : 입력된 문자열의 이름을 가진 파일에 프로그램의 결과를 저장하는 함수이다.
*        여기서 출력되는 내용은 SYMBOL별 주소값이 저장된 TABLE이다.
* 매계 : 생성할 오브젝트 파일명
* 반환 : 없음
* 주의 : 만약 인자로 NULL값이 들어온다면 프로그램의 결과를 표준출력으로 보내어
*        화면에 출력해준다.
*
* -----------------------------------------------------------------------------------
*/
void make_symtab_output(char *file_name)
{
	/* add your code here */
}

/* ----------------------------------------------------------------------------------
* 설명 : 어셈블리 코드를 기계어 코드로 바꾸기 위한 패스2 과정을 수행하는 함수이다.
*		   패스 2에서는 프로그램을 기계어로 바꾸는 작업은 라인 단위로 수행된다.
*		   다음과 같은 작업이 수행되어 진다.
*		   1. 실제로 해당 어셈블리 명령어를 기계어로 바꾸는 작업을 수행한다.
* 매계 : 없음
* 반환 : 정상종료 = 0, 에러발생 = < 0
* 주의 :
* -----------------------------------------------------------------------------------
*/
static int assem_pass2(void)
{

	/* add your code here */
}

/* ----------------------------------------------------------------------------------
* 설명 : 입력된 문자열의 이름을 가진 파일에 프로그램의 결과를 저장하는 함수이다.
*        여기서 출력되는 내용은 object code (프로젝트 1번) 이다.
* 매계 : 생성할 오브젝트 파일명
* 반환 : 없음
* 주의 : 만약 인자로 NULL값이 들어온다면 프로그램의 결과를 표준출력으로 보내어
*        화면에 출력해준다.
*
* -----------------------------------------------------------------------------------
*/
void make_objectcode_output(char *file_name)
{
	/* add your code here */
}