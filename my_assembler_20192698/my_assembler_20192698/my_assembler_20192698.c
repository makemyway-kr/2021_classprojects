/*
 * ȭ�ϸ� : my_assembler_00000000.c
 * ��  �� : �� ���α׷��� SIC/XE �ӽ��� ���� ������ Assembler ���α׷��� ���η�ƾ����,
 * �Էµ� ������ �ڵ� ��, ��ɾ �ش��ϴ� OPCODE�� ã�� ����Ѵ�.
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
#include <math.h>

  // ���ϸ��� "00000000"�� �ڽ��� �й����� ������ ��.
#include "my_assembler_20192698.h"

/* ----------------------------------------------------------------------------------
 * ���� : ����ڷ� ���� ����� ������ �޾Ƽ� ��ɾ��� OPCODE�� ã�� ����Ѵ�.
 * �Ű� : ���� ����, ����� ����
 * ��ȯ : ���� = 0, ���� = < 0
 * ���� : ���� ����� ���α׷��� ����Ʈ ������ �����ϴ� ��ƾ�� ������ �ʾҴ�.
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
    make_symtab_output("");
    /*make_literaltab_output("literaltab_20192698");
    if (assem_pass2() < 0)
    {
        printf(" assem_pass2: �н�2 �������� �����Ͽ����ϴ�.  \n");
        return -1;
    }

    make_objectcode_output("output_20192698");

    return 0;
    */
}

/* ----------------------------------------------------------------------------------
 * ���� : ���α׷� �ʱ�ȭ�� ���� �ڷᱸ�� ���� �� ������ �д� �Լ��̴�.
 * �Ű� : ����
 * ��ȯ : �������� = 0 , ���� �߻� = -1
 * ���� : ������ ��ɾ� ���̺��� ���ο� �������� �ʰ� ������ �����ϰ� �ϱ�
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
int init_inst_file(char* inst_file)
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
        inst_table[i]->op = malloc(sizeof(unsigned char));
        inst_table[i]->format = 0;
        inst_table[i]->ops = 0;
        fscanf(inst_file_to_read, "%s %c %c %c %d", st, &b, &o1, &o2, &d);// inst�κ��� �Է¹���
        strcpy(inst_table[i]->str, st);// ������ ����
        inst_table[i]->format = b;
        inst_table[i]->op = (unsigned char)(dic[o1] * 16 + dic[o2]);
        inst_table[i]->ops = d;
        i++;
        inst_index++;
    }
    fclose(inst_file_to_read);// ���ϴݱ�
    return errno;
}

/* ----------------------------------------------------------------------------------
 * ���� : ����� �� �ҽ��ڵ带 �о� �ҽ��ڵ� ���̺�(input_data)�� �����ϴ� �Լ��̴�.
 * �Ű� : ������� �ҽ����ϸ�
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
    if (str != NULL || str[0] != "\0" || str[0]!="\n")
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
        else if (strchr(str, '.'))//comment�� �����ϰų� .�� ���� ���� ���
        {
            strcpy(token_table[token_line]->comment, str);
            str[0] = "\0";
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

        if (!strlen(str) || str[0] == "\0" || str==NULL || str[0]=="\n")
        {
            return 0;
        }
        else
        {
            int count = 0;
            int c2 = 0;
            int i=0;
            if (str[0] != "\t")//operands ����
            {
                while(i<strlen(str) && str[i]!='\n')
                {
                    if (str[i] == '\t')
                    {
                        break;
                    }
                    else if (str[i] != ',' && str[i]!='\t')
                    {
                        token_table[token_line]->operand[count][c2]=str[i];
                        c2++;
                    }
                    else if (str[i] == ',')
                    {
                        token_table[token_line]->operand[count][c2] = '\0';
                        count++;
                        c2 = 0;
                    }
                    i++;
                }
                count = 0;
                if (str[i] != '\n')
                {
                    for (i = i + 1; i < strlen(str); i++)
                    {
                        token_table[token_line]->comment[count] = str[i];

                        count++;
                    }
                    token_table[token_line]->comment[count] = '\0';
                }
               
            }
            else if (str[0] == "\t")//operand ������
            {
                int count = 0;
                i =2;
                for (i; i < strlen(str); i++)
                {
                    strcpy(token_table[token_line]->comment[count], str[i]);
                    count++;
                }
            }
        }

        return err;
    }
    
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
    if (str != NULL)
    {
        if (strchr(str,'+')!=NULL)
        {
            char* nstr = malloc(sizeof(str) - sizeof(char));
            nstr[0] = "\0";
            for (int vmv = 1; vmv < strlen(str); vmv++)
            {
                nstr[vmv - 1] = str[vmv];
            }
            while (i < inst_index)
            {
                if (strcmp(inst_table[i]->str, nstr) == 0)
                {
                    an = i;
                    break;
                }
                i++;
            }
        }
        else
        {
            while (i < inst_index)
            {
                if (strcmp(inst_table[i]->str, str) == 0)
                {
                    an = i;
                    break;
                }
                i++;
            }
        }
    }
 
    return an;
}


/* ----------------------------------------------------------------------------------
* ���� : ����� �ڵ带 ���� �н�1������ �����ϴ� �Լ��̴�.
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
    locctr = 0;
    token_line = 0;
    int sttadd = 0;
    for (int i = 0; i < line_num; i++)//token���� �ɰ���.
    {
        err = token_parsing(input_data[i]);//tokenȭ��.
        if (token_line == 0)//ó�� loccation����,10������ �ٲ� ������.
        {
            char* nnum = "0x";
            nnum += token_table[token_line]->operator[0];
            locctr = strtol(nnum, NULL, 16);
            sttadd = locctr;
            token_table[token_line]->addr = locctr;
            sym_table[symbol_count].symbol = malloc(sizeof(char) * 80);
            sym_table[symbol_count].symbol[0] = "\0";
            strcpy(sym_table[symbol_count].symbol, token_table[token_line]->label);
            sym_table[symbol_count].addr = locctr;
            symbol_count++;
        }
        else if(token_table[token_line]->label!=NULL)
        {
            if (strcmp(token_table[token_line - 1]->operator,"EQU") == 0 && strcmp(token_table[token_line]->operator,"EQU") == 0)
            {
                token_table[token_line]->addr = atoi(token_table[token_line - 2]->operand[0]);
                sym_table[symbol_count].symbol = malloc(sizeof(char) * 80);
                sym_table[symbol_count].symbol[0] = "\0";
                strcpy(sym_table[symbol_count].symbol, token_table[token_line]->label);
                sym_table[symbol_count].addr = atoi(token_table[token_line - 2]->operand[0]);
                symbol_count++;
            }
            else
            {
                if (input_data[i][0] != ".")
                {
                    token_table[token_line]->addr = locctr;
                }
                if (symbol_count != 0)
                {
                    for (int j = 0; j < symbol_count; j++)
                    {
                        if (strcmp(token_table[token_line]->label, sym_table[j].symbol) == 0)
                        {
                            err = -1;//error
                            break;
                        }
                    }
                }
                if (err == 0)
                {
                    sym_table[symbol_count].symbol = malloc(sizeof(char) * 80);
                    sym_table[symbol_count].symbol[0] = "\0";
                    strcpy(sym_table[symbol_count].symbol, token_table[token_line]->label);
                    sym_table[symbol_count].addr = locctr;
                    symbol_count++;
                }
            }
            
        }
        if (token_table[token_line]->operator!=NULL)
        {
            if (strchr(token_table[token_line]->operator,'+')!=NULL)
            {
                locctr += 4;
            }
            else if (search_opcode(token_table[token_line]->operator) != -1)
            {
                locctr += 3;
            }
            else if (strcmp(token_table[token_line]->operator,"WORD") == 0)
            {
                locctr += 3;
            }
            else if (strcmp(token_table[token_line]->operator,"RESW") == 0)
            {
                locctr += (3 * atoi(token_table[token_line]->operand[0]));
            }
            else if (strcmp(token_table[token_line]->operator,"RESB") == 0)
            {
                locctr += atoi(token_table[token_line]->operand[0]);
            }
            else if (strcmp(token_table[token_line]->operator,"LTORG\n") == 0)
            {
                locctr += 3;
            }
            else if (strcmp(token_table[token_line]->operator,"BYTE") == 0)
            {
                int s = 0;
                int e = 0;
                int count = 0;
                //byte constant�� ���̸� ���Ұ���.
                for (int k = 0; k < strlen(token_table[token_line]->operand[0]); k++)
                {
                    if (token_table[token_line]->operand[0][k] == "'" && count == 0)
                    {
                        s = k;
                        count++;
                    }
                    else if (token_table[token_line]->operand[0][k] == "'" && count == 1)
                    {
                        e = k;
                        count++;
                    }
                }
                char* numbbb = malloc(sizeof(char) * (e - s)+2);
                numbbb[0] = "0";
                numbbb[1] = "x";
                count = 2;
                for (int k = s + 1; k < e; k++)
                {
                    numbbb[count] = token_table[token_line]->operand[0][k];
                }
                int nuk = strtol(numbbb, NULL, 16);
                locctr += (nuk / (int)pow(2.0, 8.0)) + 1;//2^8���� ���� �� +1�� �Ͽ� length(byte��)�� ����.
            }
        }
        token_line++;
        
    }
    proglength = locctr - sttadd;//���α׷� ���� ����.
    return err;
}

/* ----------------------------------------------------------------------------------
* ���� : �Էµ� ���ڿ��� �̸��� ���� ���Ͽ� ���α׷��� ����� �����ϴ� �Լ��̴�.
*        ���⼭ ��µǴ� ������ ��ɾ� ���� OPCODE�� ��ϵ� ǥ(���� 3��) �̴�.
* �Ű� : ������ ������Ʈ ���ϸ�
* ��ȯ : ����
* ���� : ���� ���ڷ� NULL���� ���´ٸ� ���α׷��� ����� ǥ��������� ������
*        ȭ�鿡 ������ش�.
*        ���� ���� 4�������� ���̴� �Լ��̹Ƿ� ������ ������Ʈ������ ������ �ʴ´�.
* -----------------------------------------------------------------------------------

/* ----------------------------------------------------------------------------------
* ���� : �Էµ� ���ڿ��� �̸��� ���� ���Ͽ� ���α׷��� ����� �����ϴ� �Լ��̴�.
*        ���⼭ ��µǴ� ������ SYMBOL�� �ּҰ��� ����� TABLE�̴�.
* �Ű� : ������ ������Ʈ ���ϸ�
* ��ȯ : ����
* ���� : ���� ���ڷ� NULL���� ���´ٸ� ���α׷��� ����� ǥ��������� ������
*        ȭ�鿡 ������ش�.
*
* -----------------------------------------------------------------------------------
*/
void make_symtab_output(char* file_name)
{
    if (*file_name == NULL)
    {
        for (int i = 0; i < symbol_count; i++)
        {
            printf("%s\t", sym_table[i].symbol);
            printf("%X\n", sym_table[i].addr);
        }
    }
    else
    {
        FILE* f = fopen(file_name, "w");
        for (int i = 0; i < symbol_count; i++)
        {
            fwrite(sym_table[i].symbol, sizeof(char) * strlen(sym_table[i].symbol), 1, f);
            fwrite("\t", sizeof("\t"), 1, f);
            fprintf(f, "%02X", sym_table[i].addr);
            fwrite("\n", sizeof("\n"), 1, f);
        }
        fclose(f);
    }
}

/* ----------------------------------------------------------------------------------
* ���� : �Էµ� ���ڿ��� �̸��� ���� ���Ͽ� ���α׷��� ����� �����ϴ� �Լ��̴�.
*        ���⼭ ��µǴ� ������ LITERAL�� �ּҰ��� ����� TABLE�̴�.
* �Ű� : ������ ������Ʈ ���ϸ�
* ��ȯ : ����
* ���� : ���� ���ڷ� NULL���� ���´ٸ� ���α׷��� ����� ǥ��������� ������
*        ȭ�鿡 ������ش�.
*
* -----------------------------------------------------------------------------------
*/
void make_literaltab_output(char* filen_ame)
{
    /* add your code here */
}

/* ----------------------------------------------------------------------------------
* ���� : ����� �ڵ带 ���� �ڵ�� �ٲٱ� ���� �н�2 ������ �����ϴ� �Լ��̴�.
*		   �н� 2������ ���α׷��� ����� �ٲٴ� �۾��� ���� ������ ����ȴ�.
*		   ������ ���� �۾��� ����Ǿ� ����.
*		   1. ������ �ش� ����� ��ɾ ����� �ٲٴ� �۾��� �����Ѵ�.
* �Ű� : ����
* ��ȯ : �������� = 0, �����߻� = < 0
* ���� :
* -----------------------------------------------------------------------------------
*/
static int assem_pass2(void)
{

    /* add your code here */
}

/* ----------------------------------------------------------------------------------
* ���� : �Էµ� ���ڿ��� �̸��� ���� ���Ͽ� ���α׷��� ����� �����ϴ� �Լ��̴�.
*        ���⼭ ��µǴ� ������ object code (������Ʈ 1��) �̴�.
* �Ű� : ������ ������Ʈ ���ϸ�
* ��ȯ : ����
* ���� : ���� ���ڷ� NULL���� ���´ٸ� ���α׷��� ����� ǥ��������� ������
*        ȭ�鿡 ������ش�.
*
* -----------------------------------------------------------------------------------
*/
void make_objectcode_output(char* file_name)
{
    /* add your code here */
}