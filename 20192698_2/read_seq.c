#include <stdio.h>
#include <time.h>

int main(int argc, char **argv)
{
	FILE*lenf=fopen(argv[1],"rb");
	clock_t start,end;
	int result=0;
	start=clock();
	


	end=clock();
	result=(int)((end-start)*1000);
	printf("elapsed time:%d",result);

	return 0;
}
