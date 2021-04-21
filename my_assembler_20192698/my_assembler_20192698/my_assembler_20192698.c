/*
 * 화일명 : my_assembler_00000000.c
 * 설  명 : 이 프로그램은 SIC/XE 머신을 위한 간단한 Assembler 프로그램의 메인루틴으로,
 * 입력된 파일의 코드 중, 명령어에 해당하는 OPCODE를 찾아 출력한다.
 * 파일 내에서 사용되는 문자열 "00000000"에는 자신의 학번을 기입한다.
 */

 /*
  *
  * 프로그램의 헤더를 정의한다.
  *
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

  // 파일명의 "00000000"은 자신의 학번으로 변경할 것.
#include "my_assembler_20192698.h"

/* ----------------------------------------------------------------------------------
 * 설명 : 사용자로 부터 어셈블리 파일을 받아서 명령어의 OPCODE를 찾아 출력한다.
 * 매계 : 실행 파일, 어셈블리 파일
 * 반환 : 성공 = 0, 실패 = < 0
 * 주의 : 현재 어셈블리 프로그램의 리스트 파일을 생성하는 루틴은 만들지 않았다.
 *		   또한 중간파일을 생성하지 않는다.
 * ----------------------------------------------------------------------------------
 */
int main(int args, char* arg[])
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
    // make_opcode_output("output_00000000");

    make_symtab_output("symtab_00000000");
    make_literaltab_output("literaltab_00000000");
    if (assem_pass2() < 0)
    {
        printf(" assem_pass2: 패스2 과정에서 실패하였습니다.  \n");
        return -1;
    }

    make_objectcode_output("output_00000000");

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
int init_inst_file(char* inst_file)
{
    FILE* inst_file_to_read = fopen(inst_file, "r");//읽어야할inst.data파일

    int errno;
    int i = 0;
    errno = 0;//오류 잡기용
    if (inst_file_to_read == NULL)errno = -1;
    int dic[100];//딕셔너리에 16진수 저장해둠.
    dic['0'] = 0;
    dic['1'] = 1;
    dic['2'] = 2;
    dic['3'] = 3;
    dic['4'] = 4;
    dic['5'] = 5;
    dic['6'] = 6;
    dic['7'] = 7;
    dic['8'] = 8;
    dic['9'] = 9;
    dic['A'] = 10;
    dic['B'] = 11;
    dic['C'] = 12;
    dic['D'] = 13;
    dic['E'] = 14;
    dic['F'] = 15;
    char st[15];
    char o1, o2;
    int b;
    int d;
    inst_index = 0;
    while (!feof(inst_file_to_read)) {
        inst_table[i] = malloc(sizeof(struct inst_unit));//저장공간 할당
        inst_table[i]->str[0] = "\0";
        inst_table[i]->op = malloc(sizeof(unsigned char));
        inst_table[i]->format = 0;
        inst_table[i]->ops = 0;
        fscanf(inst_file_to_read, "%s %c %c %c %d", st, &b, &o1, &o2, &d);// inst로부터 입력받음
        strcpy(inst_table[i]->str, st);// 변수에 저장
        inst_table[i]->format = b;
        inst_table[i]->op = (unsigned char)(dic[o1] * 16 + dic[o2]);
        inst_table[i]->ops = d;
        i++;
        inst_index++;
    }
    fclose(inst_file_to_read);// 파일닫기
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
int init_input_file(char* input_file)
{
    int errno;
    errno = 0;//에러 파악
    FILE* file = fopen(input_file, "r");
    for (int i = 0; i < MAX_LINES; i++)
    {
        input_data[i] = malloc(sizeof(char) * 100);
        input_data[i][0] = "\0";
    }
    if (file == NULL)errno = -1;
    line_num = 0;
    while (!feof(file))
    {
        fgets(input_data[line_num], 100, file);
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
int token_parsing(char* str)
{
    token_table[token_line] = malloc(sizeof(struct token_unit));//토큰구조체 배열 초기화
    token_table[token_line]->label = malloc(sizeof(char) * 300);
    token_table[token_line]->operator=malloc(sizeof(char) * 300);
    token_table[token_line]->label = NULL;
    token_table[token_line]->operator=NULL;
    token_table[token_line]->operand[0][0] = '\0';
    token_table[token_line]->operand[1][0] = '\0';
    token_table[token_line]->operand[2][0] = '\0';
    token_table[token_line]->comment[0] = '\0';
    int err = 0;
    if (str == NULL)
    {
        err = -1;
    }
    char* t;

    if (str[0] == '\t') {//label이 없을 경우
        token_table[token_line]->label = NULL;
        t = strtok_s(str, "\t", &str);
        token_table[token_line]->operator=t;
    }
    else {//label 이 있을경우
        t = strtok_s(str, "\t", &str);
        token_table[token_line]->label = t;
        if (strlen(str) > 0) {
            t = strtok_s(str, "\t", &str);
            t = strtok(t, "\n");
            token_table[token_line]->operator = t;
        }
        else token_table[token_line]->operator = NULL;
    }

    if (!strlen(str))
    {
        return 0;
    }
    else
    {
        int i;
        int count = 0;
        for (i = 0; i < strlen(str); i++)
        {
            if (str[i] == "\t")count += 1;
            if (count == 2)break;
        }
        i = i + 1;
        count = 0;
        int c2 = 0;
        if (str[i] != "\t")//operands 존재
        {
            for (i; i < strlen(str); i++)
            {
                if (str[i] == "\t")break;
                else if (str[i] != ",")
                {
                    strcpy(token_table[token_line]->operand[count][c2], str[i]);
                    c2++;
                }
                else if (str[i] == ",")
                {
                    count++;
                    c2 = 0;
                }
            }
            count = 0;
            for (i = i + 1; i < strlen(str); i++)
            {
                strcpy(token_table[token_line]->comment[count], str[i]);

                count++;
            }
        }
        else if (str[i] == "\t")//operand 미존재
        {
            int count = 0;
            i = i + 1;
            for (i; i < strlen(str); i++)
            {
                strcpy(token_table[token_line]->comment[count], str[i]);
                count++;
            }
        }
    }

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
int search_opcode(char* str)
{
    int an = -1;
    int i = 0;
    while (i < inst_index)
    {
        if (strcmp(inst_table[i]->str, str) == 0)
        {
            an = i;
            break;
        }
        i++;
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
    int err = 0;
    for (int i = 0; i < line_num; i++)//token으로 쪼개줌.
    {
        if (token_parsing(input_data[i]) < 0)
        {
            err = -1;
        }
        token_line++;
    }
    return err;
}

/* ----------------------------------------------------------------------------------
* 설명 : 입력된 문자열의 이름을 가진 파일에 프로그램의 결과를 저장하는 함수이다.
*        여기서 출력되는 내용은 명령어 옆에 OPCODE가 기록된 표(과제 3번) 이다.
* 매계 : 생성할 오브젝트 파일명
* 반환 : 없음
* 주의 : 만약 인자로 NULL값이 들어온다면 프로그램의 결과를 표준출력으로 보내어
*        화면에 출력해준다.
*        또한 과제 4번에서만 쓰이는 함수이므로 이후의 프로젝트에서는 사용되지 않는다.
* -----------------------------------------------------------------------------------
*/
// void make_opcode_output(char *file_name)
// {
// 	/* add your code here */

// }

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
void make_symtab_output(char* file_name)
{
    /* add your code here */
}

/* ----------------------------------------------------------------------------------
* 설명 : 입력된 문자열의 이름을 가진 파일에 프로그램의 결과를 저장하는 함수이다.
*        여기서 출력되는 내용은 LITERAL별 주소값이 저장된 TABLE이다.
* 매계 : 생성할 오브젝트 파일명
* 반환 : 없음
* 주의 : 만약 인자로 NULL값이 들어온다면 프로그램의 결과를 표준출력으로 보내어
*        화면에 출력해준다.
*
* -----------------------------------------------------------------------------------
*/
void make_literaltab_output(char* filen_ame)
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
void make_objectcode_output(char* file_name)
{
    /* add your code here */
}