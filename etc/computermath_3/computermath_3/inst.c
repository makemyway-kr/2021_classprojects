#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS
void insert_sort_ascending(char cities[10][10])
{
	for (int i = 1; i < 10; i++)
	{
		char temp[10];
		strcpy(temp, cities[i]);//�ӽ��������
		int j;
		for (j = i - 1; j >= 0; j--)
		{
			if (strcmp(cities[j], temp) >0)//���� ���ڿ��� �� ũ�� �ڷ� ������
			{
				strcpy(cities[j + 1], cities[j]);
			}
			else
			{
				break;//���ڿ����� ����(���ĺ� ������ �ռ��� ���ڿ�)���ڿ��� ������ for������
			}
		}
		strcpy(cities[j + 1], temp);//�� �ڿ� ���ڿ� �־���.
	}
	
}
void insert_sort_descending(char cities[10][10])
{
	for (int i = 1; i < 10; i++)
	{
		char temp[10];//�ӽ��������
		strcpy(temp, cities[i]);//�ű� ���� �ӽ���������� �Űܳ���.
		int j;
		for (j = i - 1; j >= 0; j--)
		{
			if (strcmp(cities[j],temp)<0)// �տ��ִ� ���ڿ��� �� ������
			{
				strcpy(cities[j + 1], cities[j]);//�ڷ� ������
			}
			else
			{
				break;//�񱳴�󺸴� ū ���ڿ��� ������ ����
			}
		}
		strcpy(cities[j + 1], temp);//�� �ڿ� ���ڿ� ����ֱ�.
	}
}
int main()
{
	char cities[10][10] = { "LONDON",  "SEOUL" , "MOSCOW" ,  "NEWYORK" ,  "TOKYO" ,  "BEIJING" ,  "PARIS",  "BERLIN" ,  "HONGKONG", "LA" };
	printf("Insertion Sort(global big cities)\n");
	printf("------------------------------------\n");
	printf("Ascending order:");
	insert_sort_ascending(cities);
	for (int i = 0; i < 10; i++)
	{
		printf("%s--", cities[i]);
	}
	printf("\n");
	insert_sort_descending(cities);
	printf("Descending Order:");
	for (int i = 0; i < 10; i++)
	{
		printf("%s--", cities[i]);
	}
	printf("\n----------------------------");
	return 0;

}