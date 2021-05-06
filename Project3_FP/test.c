#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
int main(int argc, char *argv[])
{
    int BLOCK_SIZE=50;
    int BLOCKS_PER_DEVICE=10;
    FILE*flashfp;
    flashfp=fopen("test.txt","w+b");
    char *blockbuf;
    blockbuf = (char *)malloc(BLOCK_SIZE);
	memset(blockbuf, 0xFF, BLOCK_SIZE);
    
	for(int i = 0; i < BLOCKS_PER_DEVICE; i++)
	{
		fwrite(blockbuf, BLOCK_SIZE, 1, flashfp);
	}
    fclose(flashfp);
    flashfp=fopen("test.txt","rb");
    char*buff;
    buff=malloc(BLOCK_SIZE);
    int size=sizeof(buff);
    char* teststring="1234567890";
    char *tget=malloc(4);
    int count=0;
    for(int i=6;i<10;i++)
    {
        tget[count]=teststring[i];
        count++;
    }
    printf("%s\n",teststring);
    printf("%s",tget);
    free(blockbuf);
    fclose(flashfp);
    //free(buff);
}