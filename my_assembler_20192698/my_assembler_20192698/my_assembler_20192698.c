/*
 * ȭ�ϸ� : my_assembler_20192698.c
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
#include <stdbool.h>
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
    make_symtab_output("symtab_20192698");
    
    make_literaltab_output("literaltab_20192698");
    
    
    if (assem_pass2() < 0)
    {
        printf(" assem_pass2: �н�2 �������� �����Ͽ����ϴ�.  \n");
        return -1;
    }

    make_objectcode_output("output_20192698");
    
    return 0;

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
        fscanf(inst_file_to_read, "%s %d %c %c %d", st, &b, &o1, &o2, &d);// inst�κ��� �Է¹���
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
    if (str != NULL || str[0] != "\0" || str[0] != "\n")
    {
        token_table[token_line] = malloc(sizeof(struct token_unit));//��ū����ü �迭 �ʱ�ȭ
        token_table[token_line]->label = malloc(sizeof(char) * 300);
        token_table[token_line]->operator=malloc(sizeof(char) * 300);
        token_table[token_line]->label[0] = '\0';
        token_table[token_line]->operator[0] = '\0';
        token_table[token_line]->label = NULL;
        token_table[token_line]->operator=NULL;
        token_table[token_line]->operand[0][0] = '\0';
        token_table[token_line]->operand[1][0] = '\0';
        token_table[token_line]->operand[2][0] = '\0';
        token_table[token_line]->comment[0] = '\0';
        token_table[token_line]->addr = 0;
        token_table[token_line]->nixbpe = 0;
        token_table[token_line]->literal = NULL;
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

        if (!strlen(str) || str[0] == '\0' || str == NULL || str[0] == '\n' || str[0] == '.')
        {
            return 0;
        }
        else
        {
            int count = 0;
            int c2 = 0;
            int i = 0;
            if (str[0] != "\t")//operands ����
            {
                while (i < strlen(str) && str[i] != '\n')
                {
                    if (str[i] == '\t')
                    {
                        break;
                    }
                    else if (str[i] != ',' && str[i] != '\t')
                    {
                        token_table[token_line]->operand[count][c2] = str[i];
                        c2++;
                    }
                    else if (str[i] == ','||str[i]=="\n"||str[i]=="\t")
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
                i = 2;
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
            nstr[0] = '\0';
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
    sttadd = 0;
    int section_count = 0;
    for (int i = 0; i < line_num; i++)//token���� �ɰ���.
    {
        err = token_parsing(input_data[i]);//tokenȭ��.
        if (token_table[token_line]->operator!=NULL)
        {
            if (strcmp(token_table[token_line]->operator,"CSECT") == 0)
            {
                progrlength[section_count] = locctr;
                progrlength[section_count] -= sttadd;
                eachsection[section_count] = token_line;
                section_count++;
                locctr = 0;//controlsection�� �Ѿ �ʱ�ȭ����.
                token_table[token_line]->addr = locctr;
               
            }
        }
        if (input_data[i][0] != '.')
        {
            token_table[token_line]->addr = locctr;
            token_table[token_line]->section = (section_count);
        }
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
            sym_table[symbol_count].section = 0;
            token_table[token_line]->section = 0;
            eachsection[section_count ] = token_line;
            symbol_count++;
            section_count++;
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
                sym_table[symbol_count].section = section_count - 1;
                symbol_count++;
            }
            else
            {
                sym_table[symbol_count].symbol = malloc(sizeof(char) * 80);
                sym_table[symbol_count].symbol[0] = "\0";
                strcpy(sym_table[symbol_count].symbol, token_table[token_line]->label);
                sym_table[symbol_count].addr = locctr;
                sym_table[symbol_count].section = section_count - 1;
                symbol_count++;
            
            }
            
        }
        if (token_table[token_line]->operator!=NULL)
        {
            token_table[token_line]->section = (section_count);
            int retid = search_opcode(token_table[token_line]->operator);
            if (strchr(token_table[token_line]->operator,'+')!=NULL)
            {
                locctr += 4;
            }
            else if (retid != -1)
            {
                if (inst_table[retid]->format == 2)//2����
                {
                    locctr += 2;
                }
                else if (inst_table[retid]->format == 1)//1����
                {
                    locctr += 1;
                }
                else
                {
                    locctr += 3;
                }
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
                    if (token_table[token_line]->operand[0][k] == '\'' && count == 0)
                    {
                        s = k;
                        count++;
                    }
                    else if (token_table[token_line]->operand[0][k] == '\'' && count == 1)
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
    for (int i = 0; i < token_line; i++)
    {
        if (token_table[i]->operator)
        {
            if (token_table[i]->operator[0] == '+')
            {
                token_table[i]->nixbpe += 1;//e
            }
            if (token_table[i]->operand[0][0] != '\0')
            {
                if (token_table[i]->operand[0][0] == '#')token_table[i]->nixbpe += 16;//i
                else if (token_table[i]->operand[0][0] == '@')token_table[i]->nixbpe += 32;//n
                else
                {
                    token_table[i]->nixbpe += 48;//simple addressing
                }
            }
            else token_table[i]->nixbpe += 48;//simple addressing
            if (token_table[i]->operand[1][0] != '\0')
            {
                if (token_table[i]->operand[1][0] == 'X')
                {
                    token_table[i]->nixbpe += 8;
                }
            }
        }
    }
    //���ͷ� ���̺�
    litcount = 0;
    for (int i = 0; i < token_line; i++)
    {
        if (token_table[i]->operand[0][0] == '=')//���ͷ��� ������.
        {
            int frs = 0;//�պκ�
            int ts = 0;//�޺κ�, tail(���ͷ��� ��)
            int cstr = 0;
            char* temp = malloc(sizeof(char) * 50);
            for (int fk = 0; fk < 50; fk++)
            {
                temp[fk] = '0';
            }
            for (int kli = 0; cstr <= 1; kli++)
            {
                if (token_table[i]->operand[0][kli] == '\'' && cstr == 0)
                {
                    frs = kli;
                    cstr++;
                }
                else if (token_table[i]->operand[0][kli] == '\'' && cstr == 1)
                {
                    ts = kli;
                    cstr++;
                }
            }
            for (int jlm = frs + 1; jlm < ts; jlm++)
            {
                temp[jlm - frs - 1] = token_table[i]->operand[0][jlm];
            }
            temp[ts - frs - 1] = '\0';
            int errlit = 0;
            for (int litin = 0; litin < litcount; litin++)
            {
                if (strcmp(literal_table[litin].literal, temp) == 0)
                {
                    errlit = -1;
                }
            }
            if (errlit == 0)
            {
                token_table[i]->is_literal = true;
                literal_table[litcount].literal = malloc(sizeof(char) * 50);
                literal_table[litcount].literal[0] = '\0';
                literal_table[litcount].addr = 0;
                int j = i;
                while (token_table[j]->addr != 0 && j < token_line)
                {
                    if (token_table[j]->operator!=NULL)
                    {
                        if (strcmp(token_table[j]->operator,"LTORG\n") == 0)
                        {
                            break;
                        }
                    }
                    if (j + 1 < token_line)
                    {
                        j++;
                    }
                    else if (j + 1 == token_line)
                    {
                        break;
                    }
                }
                strcpy(literal_table[litcount].literal, temp);
                token_table[i]->literal = malloc(sizeof(char) * 50);
                token_table[i]->literal[0] = '\0';
                strcpy(token_table[i]->literal, temp);
                literal_table[litcount].addr = token_table[j]->addr;
                literal_table[litcount].section = token_table[j]->section;
                litcount++;
            }
        }
        else
        {
            token_table[i]->is_literal = false;
            token_table[i]->literal = NULL;
        }
    }
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
        printf("SYMTAB\n");
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
    if (*filen_ame == NULL)
    {
    
        printf("\nLiteral Tab\n");
        for (int i = 0; i < litcount; i++)
        {
            printf("%s\t", literal_table[i].literal);
            printf("%X\n", literal_table[i].addr);
        }
    }
    else
    {
        FILE* f = fopen(filen_ame, "w");
        for (int i = 0; i < litcount; i++)
        {
            fwrite(literal_table[i].literal, sizeof(char) * strlen(literal_table[i].literal), 1, f);
            fwrite("\t", sizeof("\t"), 1, f);
            fprintf(f, "%X", literal_table[i].addr);
            fwrite("\n", sizeof("\n"), 1, f);
        }
        fclose(f);
    }
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
    int err = 0;
    for (int i = 0; i < token_line; i++)
    {
        bool start = false;
        for (int isstart = 0; isstart < 5; isstart++)//section�� �������� �˻�
        {
            if (eachsection[isstart] == i)
            {
                start = true;
            }
        }
        int currsection = 0;//���� ���ǰ˻�
        currsection = token_table[i]->section;//���� ����
        token_table[i]->objectcode[0] = (unsigned char)255;//op+ni�κ�
        token_table[i]->objectcode[1] = (unsigned char)255;//xbpe�κ�
        token_table[i]->objectcode[2] = (unsigned char)255;//3������ ��� ������ ���ڸ� 4�����ǰ�� 3�ڸ�
        token_table[i]->objectcode[3] = (unsigned char)255;//4������ ��� ������ ���ڸ�
        if (start)//START of each section
        {
            token_table[i]->objectcode[0] = token_table[i]->addr;
        }
        else if (token_table[i]->operator!=NULL)
        {
            int opc = search_opcode(token_table[i]->operator);
            if (opc != -1)
            {
                if ((token_table[i]->nixbpe & 1) == 1)//4����
                {

                    if (token_table[i]->operand[0][0] != '\0')
                    {
                        int temporarymarker = 0;
                        if (token_table[i]->operand[0][0] == '@')//indirect
                        {
                            for (int issym = 0; issym < symbol_count; issym++)//���� section�� symbol�� �ִ��� �ľ� �� ���
                            {

                                if (strcmp(sym_table[issym].symbol, strtok(token_table[i]->operand[0], "@")) == 0 && sym_table[issym].section == currsection)
                                {
                                    token_table[i]->nixbpe += 2;//pc relatvie
                                    temporarymarker = 1;
                                    char tempni = 0;
                                    token_table[i]->objectcode[0] = inst_table[opc]->op + (unsigned char)2;
                                    tempni = token_table[i]->nixbpe - 32;
                                    token_table[i]->objectcode[1] = (unsigned char)tempni;
                                    token_table[i]->objectcode[2] = (unsigned char)((sym_table[issym].addr - token_table[i + 1]->addr) / 256);
                                    token_table[i]->objectcode[3] = (unsigned char)((sym_table[issym].addr - token_table[i + 1]->addr) % 256);

                                }

                            }
                            if (temporarymarker == 0)
                            {
                                char tempni = 0;
                                token_table[i]->objectcode[0] = inst_table[opc]->op + (unsigned char)2;
                                tempni = token_table[i]->nixbpe - 32;
                                token_table[i]->objectcode[1] = (unsigned char)tempni;
                                token_table[i]->objectcode[2] = 0;
                                token_table[i]->objectcode[3] = 0;
                            }
                        }
                        else//indirect�� �ƴ�.(simple or immediate)
                        {
                            for (int issym = 0; issym < symbol_count; issym++)//���� section�� symbol�� �ִ��� �ľ� �� ���
                            {

                                if (strcmp(sym_table[issym].symbol, token_table[i]->operand[0]) == 0 && sym_table[issym].section == currsection )
                                {
                                    temporarymarker = 1;
                                    char tempni = 0;
                                    token_table[i]->nixbpe += 2;//pc relatvie
                                    if ((token_table[i]->nixbpe & 32) == 32 && (token_table[i]->nixbpe & 16) == 16)//simple addressing
                                    {
                                        token_table[i]->objectcode[0] = inst_table[opc]->op + (unsigned char)1;
                                        tempni = token_table[i]->nixbpe - 48;
                                        token_table[i]->objectcode[1] = (unsigned char)tempni;
                                        token_table[i]->objectcode[2] = (unsigned char)((sym_table[issym].addr - token_table[i + 1]->addr) / 256);
                                        token_table[i]->objectcode[3] = (unsigned char)((sym_table[issym].addr - token_table[i + 1]->addr) % 256);
                                    }


                                }

                            }
                            if (temporarymarker == 0)
                            {
                                if ((token_table[i]->nixbpe & 16) == 16 && (token_table[i]->nixbpe & 32) != 32)//immediate
                                {
                                    char tempni = 0;
                                    token_table[i]->objectcode[0] = inst_table[opc]->op + (unsigned char)1;
                                    tempni = token_table[i]->nixbpe - 16;
                                    token_table[i]->objectcode[1] = (unsigned char)tempni;
                                    int templength = atoi(strtok(token_table[i]->operand[0], "#"));
                                    token_table[i]->objectcode[2] = (unsigned char)(templength / 256);
                                    token_table[i]->objectcode[3] = (unsigned char)(templength % 256);

                                }
                                else if (token_table[i]->is_literal == true)//literal
                                {
                                    for (int j = 0; j < litcount; j++)
                                    {
                                        if (strcmp(literal_table[j].literal, token_table[i]->literal) == 0)
                                        {
                                            token_table[i]->nixbpe += 2;//pc relatvie
                                            char tempni = 0;
                                            token_table[i]->objectcode[0] = inst_table[opc]->op + (unsigned char)3;
                                            tempni = token_table[i]->nixbpe - 48;
                                            token_table[i]->objectcode[1] = (unsigned char)tempni;
                                            token_table[i]->objectcode[2] = (unsigned char)((literal_table[j].addr - token_table[i + 1]->addr) / 256);
                                            token_table[i]->objectcode[3] = (unsigned char)((literal_table[j].addr - token_table[i + 1]->addr) % 256);
                                        }


                                    }
                                }
                                else//simple && not in section
                                {
                                    char tempni = 0;
                                    token_table[i]->objectcode[0] = inst_table[opc]->op + (unsigned char)3;
                                    tempni = token_table[i]->nixbpe - 48;
                                    token_table[i]->objectcode[1] = (unsigned char)tempni;
                                    token_table[i]->objectcode[2] = 0;
                                    token_table[i]->objectcode[3] = 0;
                                }
                            }
                        }
                    }
                }
                else if (inst_table[opc]->format == 1)//1����
                {
                    token_table[i]->objectcode[0] = inst_table[opc]->op;//opcode�� ��.
                }
                else if (inst_table[opc]->format == 2)//2����
                {

                    if (inst_table[opc]->ops == 1)//operand�� 1��
                    {
                        if (token_table[i]->operand[0][0] == 'X')
                        {
                            token_table[i]->objectcode[0] = inst_table[opc]->op;
                            token_table[i]->objectcode[1] = (unsigned char)16;
                        }
                        else if (token_table[i]->operand[0][0] == 'A')
                        {
                            token_table[i]->objectcode[0] = inst_table[opc]->op;
                            token_table[i]->objectcode[1] = (unsigned char)00;
                        }
                        else if (token_table[i]->operand[0][0] == 'S')
                        {
                            token_table[i]->objectcode[0] = inst_table[opc]->op;
                            token_table[i]->objectcode[1] = (unsigned char)16 * 4;

                        }
                        else if (token_table[i]->operand[0][0] == 'T')
                        {
                            token_table[i]->objectcode[0] = inst_table[opc]->op;
                            token_table[i]->objectcode[1] = (unsigned char)16 * 5;
                        }
                    }
                    else if (inst_table[opc]->ops == 2)
                    {
                        if (token_table[i]->operand[0][0] == 'X')
                        {
                            token_table[i]->objectcode[0] = inst_table[opc]->op;
                            token_table[i]->objectcode[1] = (unsigned char)16;
                        }
                        else if (token_table[i]->operand[0][0] == 'A')
                        {
                            token_table[i]->objectcode[0] = inst_table[opc]->op;
                            token_table[i]->objectcode[1] = (unsigned char)00;
                        }
                        else if (token_table[i]->operand[0][0] == 'S')
                        {
                            token_table[i]->objectcode[0] = inst_table[opc]->op;
                            token_table[i]->objectcode[1] = (unsigned char)16 * 4;
                        }
                        else if (token_table[i]->operand[0][0] == 'T')
                        {
                            token_table[i]->objectcode[0] = inst_table[opc]->op;
                            token_table[i]->objectcode[1] = (unsigned char)16 * 5;
                        }
                        if (token_table[i]->operand[1][0] == 'X')
                        {
                            token_table[i]->objectcode[1] += (unsigned char)1;
                        }
                        else if (token_table[i]->operand[1][0] == 'A')
                        {
                            token_table[i]->objectcode[1] += (unsigned char)0;
                        }
                        else if (token_table[i]->operand[1][0] == 'S')
                        {
                            token_table[i]->objectcode[1] += (unsigned char)4;
                        }
                        else if (token_table[i]->operand[1][0] == 'T')
                        {
                            token_table[i]->objectcode[1] += (unsigned char)1;
                        }
                    }
                }

                else if (inst_table[opc]->format == 3)
                {
                    if (token_table[i]->operand[0][0] != '\0')
                    {
                        int temporarymarker = 0;
                        if (token_table[i]->operand[0][0] == '@')//indirect
                        {
                            for (int issym = 0; issym < symbol_count; issym++)//���� section�� symbol�� �ִ��� �ľ� �� ���
                            {

                                if (strcmp(sym_table[issym].symbol, strtok(token_table[i]->operand[0], "@")) == 0 && sym_table[issym].section == currsection)
                                {
                                    token_table[i]->nixbpe += 2;//pc relatvie
                                    temporarymarker = 1;
                                    char tempni = 0;
                                    token_table[i]->objectcode[0] = inst_table[opc]->op + (unsigned char)2;
                                    tempni = token_table[i]->nixbpe - 32;
                                    token_table[i]->objectcode[1] = (unsigned char)tempni;
                                    token_table[i]->objectcode[2] = (unsigned char)((sym_table[issym].addr - token_table[i + 1]->addr));

                                }

                            }
                            if (temporarymarker == 0)
                            {
                                char tempni = 0;
                                token_table[i]->objectcode[0] = inst_table[opc]->op + (unsigned char)2;
                                tempni = token_table[i]->nixbpe - 32;
                                token_table[i]->objectcode[1] = (unsigned char)tempni;
                                token_table[i]->objectcode[2] = 0;
                            }
                        }
                        else//indirect�� �ƴ�.(simple or immediate)
                        {
                            for (int issym = 0; issym < symbol_count; issym++)//���� section�� symbol�� �ִ��� �ľ� �� ���
                            {

                                if (strcmp(sym_table[issym].symbol, token_table[i]->operand[0]) == 0 && sym_table[issym].section == currsection )
                                {
                                    temporarymarker = 1;
                                    char tempni = 0;
                                    token_table[i]->nixbpe += 2;//pc relatvie
                                    if ((token_table[i]->nixbpe & 32) == 32 && (token_table[i]->nixbpe & 16) == 16)//simple addressing
                                    {
                                        token_table[i]->objectcode[0] = inst_table[opc]->op + (unsigned char)1;
                                        tempni = token_table[i]->nixbpe - 48;
                                        token_table[i]->objectcode[1] = (unsigned char)tempni;
                                        token_table[i]->objectcode[2] = (unsigned char)((sym_table[issym].addr - token_table[i + 1]->addr));
                                    }


                                }

                            }
                            if (temporarymarker == 0)
                            {
                                if ((token_table[i]->nixbpe & 16) == 16 && (token_table[i]->nixbpe & 32) != 32)//immediate
                                {

                                    char tempni = 0;
                                    token_table[i]->objectcode[0] = inst_table[opc]->op + (unsigned char)1;
                                    tempni = token_table[i]->nixbpe - 16;
                                    token_table[i]->objectcode[1] = (unsigned char)tempni;
                                    int templength = atoi(strtok(token_table[i]->operand[0], "#"));
                                    token_table[i]->objectcode[2] = (unsigned char)(templength);


                                }
                                else if (token_table[i]->is_literal == true)//literal
                                {
                                    for (int j = 0; j < litcount; j++)
                                    {
                                        if (strcmp(literal_table[j].literal, token_table[i]->literal) == 0)
                                        {
                                            token_table[i]->nixbpe += 2;//pc relatvie
                                            char tempni = 0;
                                            token_table[i]->objectcode[0] = inst_table[opc]->op + (unsigned char)3;
                                            tempni = token_table[i]->nixbpe - 48;
                                            token_table[i]->objectcode[1] = (unsigned char)tempni;
                                            token_table[i]->objectcode[2] = (unsigned char)((literal_table[j].addr - token_table[i + 1]->addr));

                                        }


                                    }
                                }
                                else//simple && not in section
                                {
                                    char tempni = 0;
                                    token_table[i]->objectcode[0] = inst_table[opc]->op + (unsigned char)3;
                                    tempni = token_table[i]->nixbpe - 48;
                                    token_table[i]->objectcode[1] = (unsigned char)tempni;
                                    token_table[i]->objectcode[2] = 0;

                                }
                            }
                        }
                    }
                    else
                    {
                        char tempni = 0;
                        token_table[i]->objectcode[0] = inst_table[opc]->op + (unsigned char)3;
                        tempni = token_table[i]->nixbpe - 48;
                        token_table[i]->objectcode[1] = (unsigned char)tempni;
                        token_table[i]->objectcode[2] = 0;
                    }
                }



            }
        
            else if (strcmp(token_table[i]->operator,"WORD") == 0 || strcmp(token_table[i]->operator,"BYTE"))
            {
                if (strcmp(token_table[i]->operator,"WORD") == 0)
                {
                    char* temp=malloc(sizeof(token_table[i]->operand[0]));
                    strcpy(temp, token_table[i]->operand[0]);
                    char* temp2 = strtok(temp, "-");
                    char* temp3 = strtok(NULL, "-");
                    int found1 = 0;
                    int found2 = 0;
                    for(int ksearchsym=0;ksearchsym<symbol_count;ksearchsym++)
                    {
                        if (strcmp(sym_table[ksearchsym].symbol, temp2) == 0 && sym_table[ksearchsym].section == token_table[i]->section)
                        {
                            found1 = ksearchsym;
                        }
                        else if (strcmp(sym_table[ksearchsym].symbol, temp3) == 0 && sym_table[ksearchsym].section == token_table[i]->section)
                        {
                            found2 = ksearchsym;
                        }
                    }
                    if(found1!=0 && found2!=0)
                    {
                        token_table[i]->objectcode[0] = (unsigned char)(sym_table[found1].addr - sym_table[found2].addr);
                        token_table[i]->objectcode[1] = 0;
                        token_table[i]->objectcode[2] = 0;
                    }
                    else
                    {
                        token_table[i]->objectcode[0] = 0;
                        token_table[i]->objectcode[1] = 0;
                        token_table[i]->objectcode[2] = 0;
                    }
                }
                else if (strcmp(token_table[i]->operator,"BYTE") == 0)
                {
                    int s = 0;
                    int e = 0;
                    int count = 0;
                  
                    for (int k = 0; k < strlen(token_table[token_line]->operand[0]); k++)
                    {
                        if (token_table[token_line]->operand[0][k] == '\'' && count == 0)
                        {
                            s = k;
                            count++;
                        }
                        else if (token_table[token_line]->operand[0][k] == '\'' && count == 1)
                        {
                            e = k;
                            count++;
                        }
                    }
                    unsigned char* numbbb = malloc(sizeof(char) * (e - s) + 2);
                    numbbb[0] = "0";
                    numbbb[1] = "x";
                    count = 2;
                    for (int k = s + 1; k < e; k++)
                    {
                        numbbb[count] = token_table[token_line]->operand[0][k];
                    }
                    int temper = strtol(numbbb, NULL, 16);
                    token_table[i]->objectcode[0] = (unsigned char)temper;
                }
            }
        }
    }
    return err;
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
    /*
    for (int i = 0; i < token_line; i++)
    {
        printf("%X %s   :", token_table[i]->addr, token_table[i]->operator);
        for (int j = 0; j < 4; j++)
        {

            if (token_table[i]->objectcode[j] != (unsigned char)255)
            {
                if (j == 0)
                {
                    printf("%02X", token_table[i]->objectcode[j]);
                }
                else if (j == 1)
                {
                    printf("%X", token_table[i]->objectcode[j]);
                }
                else if (j == 2)
                {
                    printf("%03X", token_table[i]->objectcode[j]);
                }
                else if (j == 3 && token_table[i]->operator[0]=='+')
                {
                    printf("%02X", token_table[i]->objectcode[j]);
                }
            }
        }
        printf("\n");
    }
    */
    
    if (*file_name == NULL)
    {
        int refindex = 0;
        int countforsection = 0;
        int loop = 0;
        for (int i = 0; i < token_line; i++)
        {
            int linecount = 0;//�ٸ��� ���� ����.
            if (i == eachsection[countforsection])
            {
                
                if (i != 0)
                {
                    
                    for (int modi = eachsection[countforsection - 1];modi<eachsection[countforsection];modi++)
                    {
                        if (token_table[refindex]->operand[0][0] != '\0')
                        {
                            if (strstr(token_table[modi]->operand[0], token_table[refindex]->operand[0]) !=NULL)
                            {
                                if (strchr(token_table[modi]->operand[0], "-") != NULL)
                                {
                                    printf("M %X %02X +%s\n", token_table[modi]->addr,6,strtok(token_table[modi]->operand[0], "-"));
                                    printf("M %X %02X -%s\n", token_table[modi]->addr, 6, strtok(NULL, "-"));
                                }
                                else
                                {
                                    printf("M %X %02X +%s\n", token_table[modi]->addr + 1, 5, token_table[modi]->operand[0]);
                                }
                            }
                        }                     
                        
                    }
                }
               
                printf("H%s", token_table[i]->label);
                printf("%06X %06X\n", sttadd, progrlength[countforsection]);
                countforsection++;
            }
            else if (token_table[i]->operator!=NULL)
            {
                if (strcmp(token_table[i]->operator,"EXTDEF") == 0)
                {
                    printf("D");
                    for (int j = 0; j < MAX_OPERAND; j++)
                    {
                        if (token_table[i]->operand[j][0] != '\0')
                        {
                            for (int searsym = 0; searsym < symbol_count; searsym++)
                            {
                                if (strcmp(sym_table[searsym].symbol, token_table[i]->operand[j]) == 0)
                                {
                                    printf("%s%X", token_table[i]->operand[j], sym_table[searsym].addr);
                                }
                            }
                        }
                    }
                    printf("\n");
                }
                else if (strcmp(token_table[i]->operator,"EXTREF") == 0)
                {
                    refindex = i;
                    printf("R");
                    for (int j = 0; j < MAX_OPERAND; j++)
                    {
                        if (token_table[i]->operand[j][0] != '\0')
                        {
                            printf("%s\t", token_table[i]->operand[j]);
                        }
                    }
                    printf("\n");
                }
                else
                {
                    int j = i;
                    int endofthisline = 0;
                    int lineco = 0;
                    for (j; j < eachsection[countforsection]; j++)
                    {
                        if (token_table[j]->objectcode[0] != 255)
                        {
                            linecount += 2;
                        }
                        if (token_table[j]->objectcode[1] != 255)
                        {
                            linecount += 1;
                        }
                        if (token_table[j]->objectcode[2] != 255)
                        {
                            linecount += 3;
                        }
                        if (token_table[j]->objectcode[3] != 255)
                        {
                            linecount += 2;
                        }
                        if (linecount > 30)
                        {
                            endofthisline = j-1;
                            j = token_line - 1;
                        }
                        if(linecount<=30)
                        {
                            lineco = linecount;
                            endofthisline = j - 1;
                        }
                    }
                    printf("T %06X %02X", token_table[i]->addr,lineco);
                    for (int k = i; k <= endofthisline; k++)
                    {
                        for (int l = 0; l < 4; l++)
                        {
                            if (token_table[k]->objectcode[l] != 255)
                            {
                                if (l == 0)
                                {
                                    printf("%02X", token_table[k]->objectcode[l]);
                                }
                                else if (l == 1)
                                {
                                    printf("%X", token_table[k]->objectcode[l]);
                                }
                                else if (l == 2)
                                {
                                    printf("%03X", token_table[k]->objectcode[l]);
                                }
                                else if (l == 3)
                                {
                                    printf("%02X", token_table[k]->objectcode[l]);
                                }
                            }
                        }
                    }
                    printf("\n");
                    i = endofthisline;

                }
            }
            loop++;
            if (loop > token_line)return -1;
        }
    }
    else
    {
        FILE* f = fopen(file_name, "w");
        int refindex = 0;
        int countforsection = 0;
        int loop = 0;
        for (int i = 0; i < token_line; i++)
        {
            int linecount = 0;//�ٸ��� ���� ����.
            if (i == eachsection[countforsection])
            {

                if (i != 0)
                {

                    for (int modi = eachsection[countforsection - 1]; modi < eachsection[countforsection]; modi++)
                    {
                        if (token_table[refindex]->operand[0][0] != '\0')
                        {
                            if (strstr(token_table[modi]->operand[0], token_table[refindex]->operand[0]) != NULL)
                            {
                                if (strchr(token_table[modi]->operand[0], "-") != NULL)
                                {
                                    fprintf(f,"M %X %02X +%s\n", token_table[modi]->addr, 6, strtok(token_table[modi]->operand[0], "-"));
                                    fprintf(f,"M %X %02X -%s\n", token_table[modi]->addr, 6, strtok(NULL, "-"));
                                }
                                else
                                {
                                    fprintf(f,"M %X %02X +%s\n", token_table[modi]->addr + 1, 5, token_table[modi]->operand[0]);
                                }
                            }
                        }

                    }
                }

                fprintf(f,",H%s", token_table[i]->label);
                fprintf(f,"%X%X\n", sttadd, progrlength[countforsection]);
                countforsection++;
            }
            else if (token_table[i]->operator!=NULL)
            {
                if (strcmp(token_table[i]->operator,"EXTDEF") == 0)
                {
                    fprintf(f,"D");
                    for (int j = 0; j < MAX_OPERAND; j++)
                    {
                        if (token_table[i]->operand[j][0] != '\0')
                        {
                            for (int searsym = 0; searsym < symbol_count; searsym++)
                            {
                                if (strcmp(sym_table[searsym].symbol, token_table[i]->operand[j]) == 0)
                                {
                                    fprintf(f,"%s%X", token_table[i]->operand[j], sym_table[searsym].addr);
                                }
                            }
                        }
                    }
                    fprintf(f,"\n");
                }
                else if (strcmp(token_table[i]->operator,"EXTREF") == 0)
                {
                    refindex = i;
                    fprintf(f,"R");
                    for (int j = 0; j < MAX_OPERAND; j++)
                    {
                        if (token_table[i]->operand[j][0] != '\0')
                        {
                            fprintf(f,"%s", token_table[i]->operand[j]);
                        }
                    }
                    fprintf(f,"\n");
                }
                else
                {
                    int j = i;
                    int endofthisline = 0;
                    int lineco = 0;
                    for (j; j < eachsection[countforsection]; j++)
                    {
                        if (token_table[j]->objectcode[0] != 255)
                        {
                            linecount += 2;
                        }
                        if (token_table[j]->objectcode[1] != 255)
                        {
                            linecount += 1;
                        }
                        if (token_table[j]->objectcode[2] != 255)
                        {
                            linecount += 3;
                        }
                        if (token_table[j]->objectcode[3] != 255)
                        {
                            linecount += 2;
                        }
                        if (linecount > 30)
                        {
                            endofthisline = j - 1;
                            j = token_line - 1;
                        }
                        if (linecount <= 30)
                        {
                            lineco = linecount;
                            endofthisline = j - 1;
                        }
                    }
                    fprintf(f,"T%06X%02X", token_table[i]->addr, lineco);
                    for (int k = i; k <= endofthisline; k++)
                    {
                        for (int l = 0; l < 4; l++)
                        {
                            if (token_table[k]->objectcode[l] != 255)
                            {
                                if (l == 0)
                                {
                                    fprintf(f,"%02X", token_table[k]->objectcode[l]);
                                }
                                else if (l == 1)
                                {
                                    fprintf(f,"%X", token_table[k]->objectcode[l]);
                                }
                                else if (l == 2)
                                {
                                    fprintf(f,"%03X", token_table[k]->objectcode[l]);
                                }
                                else if (l == 3)
                                {
                                    fprintf(f,"%02X", token_table[k]->objectcode[l]);
                                }
                            }
                        }
                    }
                    fprintf(f,"\n");
                    i = endofthisline;

                }
            }
            loop++;
            if (loop > token_line)return -1;

        }
        fclose(f);
    }
    
}