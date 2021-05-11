#include<stdio.h>
#include <stdlib.h>
#include <string.h>
#define _CRT_SECURE_NO_WARNINGS
void insert_sort_ascending(char cities[10][10])
{
	for (int i = 1; i < 10; i++)
	{
		char temp[10];
		strcpy(temp, cities[i]);//임시저장공간
		int j;
		for (j = i - 1; j >= 0; j--)
		{
			if (strcmp(cities[j], temp) >0)//앞의 문자열이 더 크면 뒤로 땡겨줌
			{
				strcpy(cities[j + 1], cities[j]);
			}
			else
			{
				break;//문자열보다 작은(알파벳 순서가 앞서는 문자열)문자열을 만나면 for문종료
			}
		}
		strcpy(cities[j + 1], temp);//그 뒤에 문자열 넣어줌.
	}
	
}
void insert_sort_descending(char cities[10][10])
{
	for (int i = 1; i < 10; i++)
	{
		char temp[10];//임시저장공간
		strcpy(temp, cities[i]);//옮길 내용 임시저장공간에 옮겨놓음.
		int j;
		for (j = i - 1; j >= 0; j--)
		{
			if (strcmp(cities[j],temp)<0)// 앞에있는 문자열이 더 작으면
			{
				strcpy(cities[j + 1], cities[j]);//뒤로 땡겨줌
			}
			else
			{
				break;//비교대상보다 큰 문자열이 나오면 멈춤
			}
		}
		strcpy(cities[j + 1], temp);//그 뒤에 문자열 집어넣기.
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