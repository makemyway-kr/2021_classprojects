/*
 * ȭ�ϸ� : my_assembler_00000000.c
 * ��  �� : �� ���α׷��� SIC/XE �ӽ��� ���� ������ Assembler ���α׷��� ���η�ƾ����,
 * �Էµ� ������ �ڵ� ��, ���ɾ �ش��ϴ� OPCODE�� ã�� ����Ѵ�.
 * ���� ������ ���Ǵ� ���ڿ� "00000000"���� �ڽ��� �й��� �����Ѵ�.
 */

 /*
  *
  * ���α׷��� ����� �����Ѵ�.
  *
  */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

#include "my_assembler_20192698.h"
token_line = 0;//��ū ���忡 ����� line�������� �ʱ�ȭ
/* ----------------------------------------------------------------------------------
 * ���� : ����ڷ� ���� ������� ������ �޾Ƽ� ���ɾ��� OPCODE�� ã�� ����Ѵ�.
 * �Ű� : ���� ����, ������� ����
 * ��ȯ : ���� = 0, ���� = < 0
 * ���� : ���� ������� ���α׷��� ����Ʈ ������ �����ϴ� ��ƾ�� ������ �ʾҴ�.
 *		   ���� �߰������� �������� �ʴ´�.
 * ----------------------------------------------------------------------------------
 */
int main(int args, char* arg[])
{
	if (init_my_assembler() < 0)
	{
		printf("init_my_assembler: ���α׷� �ʱ�ȭ�� ���� �߽��ϴ�.\n");
		return -1;
	}

	if (assem_pass1() < 0)
	{
		printf("assem_pass1: �н�1 �������� �����Ͽ����ϴ�.  \n");
		return -1;
	}

	make_opcode_output("output_20192698");

	/*
	 * ���� ������Ʈ���� ���Ǵ� �κ�
	 *
	make_symtab_output("symtab_00000000");
	if (assem_pass2() < 0)
	{
		printf(" assem_pass2: �н�2 �������� �����Ͽ����ϴ�.  \n");
		return -1;
	}

	make_objectcode_output("output_00000000");
	*/
	return 0;
}

/* ----------------------------------------------------------------------------------
 * ���� : ���α׷� �ʱ�ȭ�� ���� �ڷᱸ�� ���� �� ������ �д� �Լ��̴�.
 * �Ű� : ����
 * ��ȯ : �������� = 0 , ���� �߻� = -1
 * ���� : ������ ���ɾ� ���̺��� ���ο� �������� �ʰ� ������ �����ϰ� �ϱ�
 *		   ���ؼ� ���� ������ �����Ͽ� ���α׷� �ʱ�ȭ�� ���� ������ �о� �� �� �ֵ���
 *		   �����Ͽ���.
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
 * ���� : �ӽ��� ���� ��� �ڵ��� ������ �о� ���� ��� ���̺�(inst_table)��
 *        �����ϴ� �Լ��̴�.
 * �Ű� : ���� ��� ����
 * ��ȯ : �������� = 0 , ���� < 0
 * ���� : ���� ������� ������ �����Ӱ� �����Ѵ�. ���ô� ������ ����.
 *
 *	===============================================================================
 *		   | �̸� | ���� | ���� �ڵ� | ���۷����� ���� | NULL|
 *	===============================================================================
 *
 * ----------------------------------------------------------------------------------
 */
int init_inst_file(char* inst_file)//inst.data �Է¹ޱ�.
{
	FILE* inst_file_to_read = fopen(inst_file, "r");//�о����inst.data����

	int errno;
	int i = 0;
	errno = 0;//���� ����
	if (inst_file_to_read == NULL)errno = -1;
	int dic[100];//��ųʸ��� 16���� �����ص�.
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
		inst_table[i] = malloc(sizeof(struct inst_unit));//������� �Ҵ�
		inst_table[i]->str[0] = "\0";
		inst_table[i]->op=malloc(sizeof(unsigned char));
		inst_table[i]->format=0;
		inst_table[i]->ops=0;
		fscanf(inst_file_to_read,"%s %c %c %c %d",st,&b,&o1,&o2,&d);// inst�κ��� �Է¹���
		strcpy(inst_table[i]->str, st);// ������ ����
		inst_table[i]->format = b;
		inst_table[i]->op =(unsigned char)( dic[o1] * 16 + dic[o2]);
		inst_table[i]->ops = d;
		i++;
		inst_index++;
	}
	fclose(inst_file_to_read);// ���ϴݱ�
	return errno;
}

/* ----------------------------------------------------------------------------------
 * ���� : ������� �� �ҽ��ڵ带 �о� �ҽ��ڵ� ���̺�(input_data)�� �����ϴ� �Լ��̴�.
 * �Ű� : ��������� �ҽ����ϸ�
 * ��ȯ : �������� = 0 , ���� < 0
 * ���� : ���δ����� �����Ѵ�.
 *
 * ----------------------------------------------------------------------------------
 */
int init_input_file(char* input_file)
{
	int errno;
	errno = 0;//���� �ľ�
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
 * ���� : �ҽ� �ڵ带 �о�� ��ū������ �м��ϰ� ��ū ���̺��� �ۼ��ϴ� �Լ��̴�.
 *        �н� 1�� ���� ȣ��ȴ�.
 * �Ű� : �Ľ��� ���ϴ� ���ڿ�
 * ��ȯ : �������� = 0 , ���� < 0
 * ���� : my_assembler ���α׷������� ���δ����� ��ū �� ������Ʈ ������ �ϰ� �ִ�.
 * ----------------------------------------------------------------------------------
 */
int token_parsing(char* str)
{
	token_table[token_line] = malloc(sizeof(struct token_unit));//��ū����ü �迭 �ʱ�ȭ
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

	if (str[0] == '\t') {//label�� ���� ���
		token_table[token_line]->label = NULL;
		t = strtok_s(str, "\t", &str);
		token_table[token_line]->operator=t;
	}
	else {//label �� �������
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
		if (str[i] != "\t")//operands ����
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
		else if (str[i] == "\t")//operand ������
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
 * ���� : �Է� ���ڿ��� ���� �ڵ������� �˻��ϴ� �Լ��̴�.
 * �Ű� : ��ū ������ ���е� ���ڿ�
 * ��ȯ : �������� = ���� ���̺� �ε���, ���� < 0
 * ���� :
 *
 * ----------------------------------------------------------------------------------
 */
int search_opcode(char* str)
{
	int an = -1;
	int i = 0;
	while(i<inst_index)
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
* ���� : ������� �ڵ带 ���� �н�1������ �����ϴ� �Լ��̴�.
*		   �н�1������..
*		   1. ���α׷� �ҽ��� ��ĵ�Ͽ� �ش��ϴ� ��ū������ �и��Ͽ� ���α׷� ���κ� ��ū
*		   ���̺��� �����Ѵ�.
*
* �Ű� : ����
* ��ȯ : ���� ���� = 0 , ���� = < 0
* ���� : ���� �ʱ� ���������� ������ ���� �˻縦 ���� �ʰ� �Ѿ �����̴�.
*	  ���� ������ ���� �˻� ��ƾ�� �߰��ؾ� �Ѵ�.
*
* -----------------------------------------------------------------------------------
*/
static int assem_pass1(void)
{
	int err = 0;
	for (int i = 0; i < line_num; i++)//token���� �ɰ���.
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
* ���� : �Էµ� ���ڿ��� �̸��� ���� ���Ͽ� ���α׷��� ����� �����ϴ� �Լ��̴�.
*        ���⼭ ��µǴ� ������ ���ɾ� ���� OPCODE�� ��ϵ� ǥ(���� 3��) �̴�.
* �Ű� : ������ ������Ʈ ���ϸ�
* ��ȯ : ����
* ���� : ���� ���ڷ� NULL���� ���´ٸ� ���α׷��� ����� ǥ��������� ������
*        ȭ�鿡 ������ش�.
*        ���� ���� 3�������� ���̴� �Լ��̹Ƿ� ������ ������Ʈ������ ������ �ʴ´�.
* -----------------------------------------------------------------------------------
*/
void make_opcode_output(char* file_name)
{
	if (*file_name == NULL)//���ϸ��� NULL���϶� CMDâ�� ���
	{
		for (int i = 0; i < token_line; i++)
		{
			if (token_table[i]->label != NULL)
			{
				printf("%s", token_table[i]->label);

			}
			printf("\t");
			if (token_table[i]->operator!=NULL)
			{
				printf("%s", token_table[i]->operator);
			}
			printf("\t");
			if (token_table[i]->operand[0][0] != "\0")
			{
				for (int j = 0; j < 3; j++)
				{
					if (token_table[i]->operand[j][0] != "\0")
					{
						printf("%s", token_table[i]->operand[j]);
					}

				}
			}
			printf("\t");
			int a = -1;
			if (token_table[i]->operator!=NULL)
			{
				a = search_opcode(token_table[i]->operator);
				if (a >= 0)//opcode�� �����Ѵٸ�
				{
					printf("%02X", inst_table[a]->op);
				}
				else if (strcmp("START",token_table[i]->operator)==0)
				{
					printf("00");
				}
				
			}
			printf("\n");
		
		}
	}
	else
	{
		FILE* f = fopen(file_name, "w");

		for (int i = 0; i < token_line; i++)
		{
			if (token_table[i]->label != NULL)
			{
				fwrite(token_table[i]->label, sizeof(char) * strlen(token_table[i]->label), 1, f);

			}
			fwrite("\t", sizeof("\t"), 1, f);
			if (token_table[i]->operator!=NULL)
			{
				fwrite(token_table[i]->operator,sizeof(char) * strlen(token_table[i]->operator), 1, f);
			}
			fwrite("\t", sizeof("\t"), 1, f);
			if (token_table[i]->operand[0][0] != "\0")
			{
				for (int j = 0; j < 3; j++)
				{
					if (token_table[i]->operand[j][0] != "\0")
					{
						fwrite(token_table[i]->operand[j], sizeof(char) * strlen(token_table[i]->operand[j]), 1, f);
					}

				}
			}
			fwrite("\t", sizeof("\t"), 1, f);
			int a = -1;
			if (token_table[i]->operator!=NULL)
			{
				a = search_opcode(token_table[i]->operator);
				if (a >= 0)
				{

					fprintf(f, "%02X", inst_table[a]->op);
				}
				else if (strcmp("START", token_table[i]->operator) == 0)
				{
					fwrite("00", sizeof("00"), 1, f);
				}
				
			}
			
			fwrite("\n", sizeof("\n"), 1, f);
		}
		fclose(f);
	}
}