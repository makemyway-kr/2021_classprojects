#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS
void bubble_ascending(char mammals[10][10])
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10 - i - 1; j++)
		{
			char temp[10];//�ӽ��������
			strcpy(temp, mammals[j]);
			if (strcmp(temp, mammals[j + 1]) > 0)//temp�� ���ĺ� ������ �ڸ� �յڸ� �ٲ���.
			{
				strcpy(mammals[j], mammals[j + 1]);
				strcpy(mammals[j + 1], temp);
			}

		}
	}
}
void bubble_descending(char mammals[10][10])
{
	for (int i = 0; i < 10; i++)
	{
		for (int j = 0; j < 10 - i - 1; j++)
		{
			char* temp[10];
			strcpy(temp, mammals[j]);
			if (strcmp(temp, mammals[j + 1]) < 0)//temp�� ���ĺ� ������ ���̸� �յڸ� �ٲ���.
			{
				strcpy(mammals[j], mammals[j + 1]);
				strcpy(mammals[j + 1], temp);
			}
		}
	}	
}
int main()
{
	char mammals[10][10] = { "PIG","GOAT","HORSE","GIRAFFE","ELEPHANT","BEAR","LION","TIGER","FOX","DOLPHIN" };
	printf("Bubble sort(animals-mammals)\n");
	printf("------------------------------------\n");
	printf("Ascending order:");
	bubble_ascending(mammals);
	for (int i = 0; i < 10; i++)
	{
		printf("%s--", mammals[i]);
	}
	printf("\n");
	bubble_descending(mammals);
	printf("Descending Order:");
	for (int i = 0; i < 10; i++)
	{
		printf("%s--", mammals[i]);
	}
	printf("\n----------------------------");
	return 0;
}