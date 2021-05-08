// 
// 과제3의 채점 프로그램은 기본적으로 아래와 같이 동작함
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include "blockmap.h"

FILE* flashfp;

int write_cnt;
int erase_cnt;


/****************  prototypes ****************/
void ftl_open();
void ftl_write(int lsn, char* sectorbuf);
void ftl_read(int lsn, char* sectorbuf);
void ftl_print();

int main(int argc, char* argv[])
{
   char* blockbuf;
   int lsn, i;

   flashfp = fopen("flashmemory.txt", "w+b");
   if (flashfp == NULL)
   {
      printf("file open error\n");
      exit(1);
	  blockbuf = (char*)malloc(BLOCK_SIZE);
   	  memset(blockbuf, 0xFF, BLOCK_SIZE);

   	  for (i = 0; i < BLOCKS_PER_DEVICE; i++)
      {
		fwrite(blockbuf, BLOCK_SIZE, 1, flashfp);
      }

   	  free(blockbuf);
   }
   fclose(flashfp);
   flashfp=fopen("flashmemory.txt","rb");
   ftl_open();    // ftl_read(), ftl_write() 호출하기 전에 이 함수를 반드시 호출해야 함
   fclose(flashfp);
   flashfp = fopen("flashmemory.txt", "w+b");
   char *secbuf=malloc(SECTOR_SIZE);
   memset(secbuf,0xFF,SECTOR_SIZE);
   strcpy(secbuf,"dfadsfasdfdsf5555654asdfasdfasdfsadfasdfsdafsdafsdafsdfsadfasdfsdafsadfasdfasdfasdfsadfasdfasdfasdfsdaf5sda41f56sad4f65sda4f65sd4f6541sda65f1sd6352f1sd32f13sd54f16sd541f65sd41f63sd541f65sd41f65sda41f6541dsf6dsa65f46sad5f146sd5af1463s5ad1f46asd351f63sd5af416sad54f9sda874f9sd8a47f96asd584f16sda514f6sd5a14f65sd41f65asd14f65asd4f65asd4f65sd4f65asd4f65as4df65sda4f654sda6f54asd6f456sd54f6sda54fasd56f46ew54qr98q7wer9+84qwe6r5f4we6q54f651sdf3s2a1df32sda1f65asd4f6we4q968r749qwe8r749qewds5f41asd635f1sdafdsfasdfasdff");
   ftl_write(0, secbuf);
   memset(secbuf,0xFF,SECTOR_SIZE);
   strcpy(secbuf,"dfadsfasdfdsf5555654asdfasdfasdfsadfasdfsdafsdafsdafsdfsadfasdfsdafsadfasdfasdfasdfsadfasdfasdfasdfsdaf5sda41f56sad4f65sda4f65sd4f6541sda65f1sd6352f1sd32f13sd54f16sd541f65sd41f63sd541f65sd41f65sda41f6541dsf6dsa65f46sad5f146sd5af1463s5ad1f46asd351f63sd5af416sad54f9sda874f9sd8a47f96asd584f16sda514f6sd5a14f65sd41f65asd14f65asd4f65asd4f65sd4f65asd4f65as4df65sda4f654sda6f54asd6f456sd54f6sda54fasd56f46ew54qr98q7wer9+84qwe6r5f4we6q54f651sdf3s2a1df32sda1f65asd4f6we4q968r749qwe8r749qewds5f41asd635f1sdafdsfasdfasdff");
   ftl_write(1,secbuf);
   memset(secbuf,0xFF,SECTOR_SIZE);
   strcpy(secbuf,"dfadsfasdfdsf5555654asdfasdfasdfsadfasdfsdafsdafsdafsdfsadfasdfsdafsadfasdfasdfasdfsadfasdfasdfasdfsdaf5sda41f56sad4f65sda4f65sd4f6541sda65f1sd6352f1sd32f13sd54f16sd541f65sd41f63sd541f65sd41f65sda41f6541dsf6dsa65f46sad5f146sd5af1463s5ad1f46asd351f63sd5af416sad54f9sda874f9sd8a47f96asd584f16sda514f6sd5a14f65sd41f65asd14f65asd4f65asd4f65sd4f65asd4f65as4df65sda4f654sda6f54asd6f456sd54f6sda54fasd56f46ew54qr98q7wer9+84qwe6r5f4we6q54f651sdf3s2a1df32sda1f65asd4f6we4q968r749qwe8r749qewds5f41asd635f1sdafdsfasdfasdff");
   ftl_write(1,secbuf);
   memset(secbuf,0xFF,SECTOR_SIZE);
   strcpy(secbuf,"dfadsfasdfdsf5555654asdfasdfasdfsadfasdfsdafsdafsdafsdfsadfasdfsdafsadfasdfasdfasdfsadfasdfasdfasdfsdaf5sda41f56sad4f65sda4f65sd4f6541sda65f1sd6352f1sd32f13sd54f16sd541f65sd41f63sd541f65sd41f65sda41f6541dsf6dsa65f46sad5f146sd5af1463s5ad1f46asd351f63sd5af416sad54f9sda874f9sd8a47f96asd584f16sda514f6sd5a14f65sd41f65asd14f65asd4f65asd4f65sd4f65asd4f65as4df65sda4f654sda6f54asd6f456sd54f6sda54fasd56f46ew54qr98q7wer9+84qwe6r5f4we6q54f651sdf3s2a1df32sda1f65asd4f6we4q968r749qwe8r749qewds5f41asd635f1sdafdsfasdfasdff");
   ftl_write(14,secbuf);
   memset(secbuf,0xFF,SECTOR_SIZE);
   strcpy(secbuf,"dfadsfasdfdsf5555654asdfasdfasdfsadfasdfsdafsdafsdafsdfsadfasdfsdafsadfasdfasdfasdfsadfasdfasdfasdfsdaf5sda41f56sad4f65sda4f65sd4f6541sda65f1sd6352f1sd32f13sd54f16sd541f65sd41f63sd541f65sd41f65sda41f6541dsf6dsa65f46sad5f146sd5af1463s5ad1f46asd351f63sd5af416sad54f9sda874f9sd8a47f96asd584f16sda514f6sd5a14f65sd41f65asd14f65asd4f65asd4f65sd4f65asd4f65as4df65sda4f654sda6f54asd6f456sd54f6sda54fasd56f46ew54qr98q7wer9+84qwe6r5f4we6q54f651sdf3s2a1df32sda1f65asd4f6we4q968r749qwe8r749qewds5f41asd635f1sdafdsfasdfasdff");
   ftl_write(55,secbuf);
   int j;
   fclose(flashfp);
   memset(secbuf,0xFF,SECTOR_SIZE);
   flashfp=fopen("flashmemory.txt","r+b");
   ftl_read(1,secbuf);
   printf("%s\n",secbuf);
   ftl_print();
   free(secbuf);
   return 0;
}