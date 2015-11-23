#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE   (10)
#define HIGH_WATER_MARK_BYTES  ((BUFFER_SIZE*9)/10)
#define NUMBER_CC_ENTRIES (900)
#define HIGH_WATER_MARK_DATA    ((NUMBER_CC_ENTRIES*9)/10)


typedef struct
{
      int                  readIdxM;
  //  bool                 fHighWaterM;
      char*                bufferM;     // actual data buffer
      int                  nUsedM;     // This is not an index on the FIFO but the quantity of bytes filled inside , no index just plain quantity

} FIFOBuffer;


/*  PUSH function    */
int PushBytes(
	const  char       *ptr,
	 int        len,
	FIFOBuffer   *aFIFOBuffer)
{
  int bytesremaining = 0;
  bytesremaining = BUFFER_SIZE - aFIFOBuffer->nUsedM;
  if (bytesremaining < len)
    {
       // not enuf memory -  so fill what is left
       len = bytesremaining;
    }

	unsigned int writeIdx = (aFIFOBuffer->readIdxM + aFIFOBuffer->nUsedM) % BUFFER_SIZE;
	unsigned int toCopy   = len;

	if ((writeIdx + len) > BUFFER_SIZE) {
		toCopy = BUFFER_SIZE - writeIdx;

		memcpy(&(aFIFOBuffer->bufferM[writeIdx]), ptr, toCopy);

		ptr      += toCopy;
		toCopy    = len - toCopy;
		writeIdx  = 0;
	}
	memcpy(&(aFIFOBuffer->bufferM[writeIdx]), ptr, toCopy);
	aFIFOBuffer->nUsedM += len;
}

/* POP Function */
void PopBytes(
	int             len,
	char            *ptr,
	FIFOBuffer *aFIFOBuffer )
{
      //we can at max read only whatever there is available in the buffer
    if (len > aFIFOBuffer->nUsedM)
	    len = aFIFOBuffer->nUsedM;

    int toCopy = len;
    if ((aFIFOBuffer->readIdxM + len) > BUFFER_SIZE)
    {
        printf("Length to be read has excceded the BUFFER_SIZE available ,time to loop\n");
        toCopy = BUFFER_SIZE - aFIFOBuffer->readIdxM;

        if (ptr != NULL)
        {
            memcpy(ptr, &(aFIFOBuffer->bufferM[aFIFOBuffer->readIdxM]), toCopy);
            ptr      += toCopy;
        }

        toCopy    = len - toCopy;
        aFIFOBuffer->readIdxM  = 0;
    }

    if (ptr != NULL)
    {
        memcpy(ptr, &(aFIFOBuffer->bufferM[aFIFOBuffer->readIdxM]), toCopy);
    }
    aFIFOBuffer->readIdxM += toCopy;
    if ( aFIFOBuffer->readIdxM >= BUFFER_SIZE) { // == would suffice, but let's be conservative
        aFIFOBuffer->readIdxM = 0;
    }
    aFIFOBuffer->nUsedM   -= len;
 
}



void main()
{

  FIFOBuffer  *video_FIFOBufferi = (FIFOBuffer*)malloc(sizeof(FIFOBuffer));

  video_FIFOBufferi->readIdxM = 0;

  video_FIFOBufferi->nUsedM = 0;

  video_FIFOBufferi->bufferM = (char*)malloc(BUFFER_SIZE) ;
  if(video_FIFOBufferi->bufferM == NULL)
    {
        printf("Error Allocaton\n");
        return;
    }

  char *data = "Hello";
  int i = PushBytes(data,5,video_FIFOBufferi);
  data = "World";
  i = PushBytes(data,5,video_FIFOBufferi);

  char *out = malloc(BUFFER_SIZE);
  PopBytes(8,out,video_FIFOBufferi );
  printf("value at POP after func in main = %s\n",out);


  // free the data 
  free(out);
  free(video_FIFOBufferi->bufferM);
  free(video_FIFOBufferi);

  return;
}
