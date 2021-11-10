#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int main(int argc, char **argv)
{
    char *record;
    record=malloc(250);
    record="dskajfhdsjfhsdjkfhkdsjfhuweyriuweyrohfdjcndjcndjfhdjfhdjfhwoeuirhewourfhdjfhdsjkfchsdjkcndskjfhdjfhdksjfhweioruyewroewjrfheworjherojhrewojrhweojrhewrohewrohweroewjrhdjfhdskfjhdsfkjwherjhewrjhewjrhewjrhjewhrerjwhjwhrejwhrjwehrjwehrjewrhhjhjhjhjhjhjhhj";
    FILE* recfile=fopen(argv[2],"wb");
    int length=atoi(argv[1]);
    fwrite(&length,sizeof(int),1,recfile);
    fclose(recfile);
    FILE* rfile=fopen(argv[2],"a");
    int count=0;
    while(count<length)
    {
        fputs(record,rfile);
        count++;
    }
    fclose(rfile);
	return 0;
}
