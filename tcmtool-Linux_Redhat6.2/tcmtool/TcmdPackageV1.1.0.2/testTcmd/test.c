
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include "trousers/tss.h"
#include <sys/types.h>
#include <sys/stat.h>
#include "fcntl.h"
#include "tddl.h"

int64_t g_i;

int T_Startup()
{
	int rst = 0;
	BYTE inBuf[13] = {0x00,0xc1,0x00,0x00,0x00,0x0c,0x00,0x00,0x80,0x99,0x00,0x01};
	UINT32 inLen =13;
	BYTE  outBuf[100] ={0};
	UINT32 outLen =100;
	int i = 0 ;
        printf( "--Startup in buf :");  
    	for( ; i < inLen ; i++)
	  printf(" %02x",inBuf[i]);
	printf("\n");     
     
	rst = Tddli_TransmitData(inBuf,inLen,outBuf,&outLen);
	if( rst == 0 )
	{
	    printf( "--Startup out buf :");  
	    for( i = 0; i < outLen ; i++)
		printf(" %02x",outBuf[i]);
	    printf("\n");
	}
        else
	{
          printf("StartUp failed\n");
	}
        return rst ;
}
int hashstart()
{
     BYTE inBuf[10] = {0x00,0xC1,0x00,0x00,0x00,0x0A,0x00,0x00,0x80,0xEA};
     int  inLen = 10;
     BYTE outBuf[100] = {0};
     int outLen = 100;
     int rst = Tddli_TransmitData( inBuf,inLen,outBuf,&outLen);
     return rst;
}
#define int2Bytes(buf,l)  {(buf)[0] = ( (l) >> 24) & 0xff;\
	(buf)[1] = ( (l) >> 16) & 0xff;\
	(buf)[2] = ( (l) >> 8) & 0xff;\
	(buf)[3] = ( (l) ) & 0xff; }
int hashcomplete( )
{
   
     /*BYTE hash[512] = {0};
     int hashLen = dataLen;


      BYTE pHead[14] = {0x00,0xc1,
			0x00,0x00,0x00,0x00,
                        0x00,0x00,0x80,0xEC,
                        0x00,0x00,0x00,0x00};
      
      int inLen = 14 + hashLen;
      BYTE* inBuf = (BYTE*) malloc(inLen);
      if( !inBuf ) 
           return -1;
      memset( inBuf,0,inLen);
    
      memcpy( inBuf,pHead,14);
      memcpy( inBuf + 14,hash,hashLen);


      int2Bytes( inBuf + 2 ,inLen);
      int2Bytes( inBuf + 10 ,hashLen);*/


    BYTE inBuf[14] ={0x00,0xc1,
	             0x00,0x00,0x00,0x0E,
                     0x00,0x00,0x80,0xEC,
                     0x00,0x00,0x00,0x00};
    int inLen = 14;
    BYTE outBuf[100] = {0};
    int outLen = 100;

     int i = 0 ;
     printf( "--Has complete in buf :");  
     for( ; i < inLen ; i++)
	printf(" %02x",inBuf[i]);
	printf("\n");
     int rst = Tddli_TransmitData( inBuf,inLen,outBuf,&outLen);
     printf( "--Has complete out buf :");  
     for( i = 0; i < outLen ; i++)
	printf(" %02x",outBuf[i]);
	printf("\n");
     return rst;
}
int hashupdate(char* hashData,int hashDataLen)
{
      if(! hashData)
         return -1;
  
      BYTE pHead[14] = {0x00,0xc1,
			0x00,0x00,0x00,0x00,
                        0x00,0x00,0x80,0xEB,
                        0x00,0x00,0x00,0x00};
      
      int inLen = 14 + hashDataLen;
      BYTE* inBuf = (BYTE*) malloc(inLen);
      if( !inBuf ) 
           return -1;
      memset( inBuf,0,inLen);
      BYTE outBuf[100] = {0};
      int outLen = 100;
      memcpy( inBuf,pHead,14);
      memcpy( inBuf + 14,hashData,hashDataLen);
      
      int2Bytes( inBuf + 2 ,inLen);
      int2Bytes( inBuf + 10 ,hashDataLen);

      int i = 0 ; 
      printf( "--Has update in buf :");  
      for( ; i < inLen ; i++)
	printf(" %02x",inBuf[i]);
	printf("\n");
      int rst= Tddli_TransmitData( inBuf,inLen,outBuf,&outLen);
      if( rst == 0 )
      {
	printf( "--Has update out buf :"); 
	 
       for( i = 0 ; i < outLen ; i++)
	 printf(" %02x",outBuf[i]);
	 printf("\n");
      }
      
      free(inBuf);
      return rst;
  
}
/*int T_HashData(BYTE* filename )
{
       int fd = 0;
       fd = open( filename,O_RDONLY);
       if( fd == 0)
          return -1;
       BYTE buf[10];
       read( fd ,buf,5);
 return 0;
}*/
void GenHashData(BYTE* buf,int * bufLen,int rd )
{
      time_t t;
      localtime(&t);
      srand( rd );
      int i = 0 ;
      
      int nLen =  rand() % 512;
      *bufLen = nLen;
      for( ;i < nLen ; i++ )
	buf[i] = rand() % 255;
}
int T_HashData(int rd )
{
        int rst = hashstart();
        if( rst == 0)
           printf( "Hash Start Successful \n");
	else
	{
	   printf( "Hash Start Failed \n");
	   return rst;
	}
	int bufLen;
	BYTE hashData[512] = {0};
        GenHashData(hashData,&bufLen,rd);
	/*BYTE hashData[512]={0xdd ,0x5b ,0xf8 ,0x33 ,0xda ,0xdf ,0xa9 ,0x30 ,0x72 ,0xe1 ,0xd4 ,0x0 ,0x7d ,0x6e ,0x57 ,0x0c ,0xd3 ,0xe6 ,0x24 ,0xb5 ,0x3b ,0x46 ,0x1d ,0x9d ,0xa9 ,0xb6 ,0x46 
,0x76 ,0x92 ,0xfa ,0x68 ,0xef ,0x56 ,0xe0 ,0x23 ,0xb1 ,0xc0 ,0x4c ,0x61 ,0x33 ,0x2e ,0x36};
bufLen = 42;*/
        rst = hashupdate(hashData,bufLen);
        if( rst == 0) printf( "Hash update %d bytes  Successful\n",bufLen);
        else return rst;
        rst = hashcomplete(  );
        if( rst == 0) printf( "Hash complete %d bytes successful\n",bufLen);
	else return rst;

      return rst;
}
void Logerr( int errcode,int index)
{
   int fd = open( "mylog",O_RDWR);
   if( !fd )
   {
    int fd = open( "mylog",O_RDWR | O_CREAT);
    if( !fd )
      return;
   }
   BYTE msg[100] = {0};
   sprintf( msg ,"---index :%d error code :%x",index,errcode);
   write( fd ,msg,strlen(msg) );
}
int main( int argc, char** argv)
{

	int iFlag = 0;
	int rst = Tddli_Open();
	if( rst != 0 ) 
	{
	  printf("Can not open TCM Driver!\n");
	  printf("Tddli_Open = %x \n",rst);
	  return -1;
	}
	printf("Tddli_Open = %x \n",rst);

	iFlag = 1;
	if( argc == 2 )
	{
	    char* param = argv[1];
	    if( strcmp( param,"startup") == 0 )
	    {
 		if ( 0 != T_Startup() ) 
		 return -1;
	    }
 	    else if( strcmp( param,"hashdata") == 0)
	    {
		rst = T_HashData(0);
		if( rst != 0 )
		    printf( "Err occour ,code : %02x\n" ,rst);
	    }
	}
        else if( argc == 3 )
		;
	if ( 0 != T_Startup() ) 
		 return -1;
	//return 1;

	int i = 0;
	//while( 1 )
//	for( i = 0 ; i < 5000 ; i++)
	while(1)
	{
		
        	rst = T_HashData(i);
		//Logerr( rst ,i);
		if( rst != 0 )
		{
		    printf( "###########Err occour ,code : %02x\n" ,rst);
  		   // Logerr( rst ,i);
	    	    break;
		}
	       //sleep(1);
		printf( "------------------------------index : %d finish --------------------------------\n",i++);
	}
	

	
      
	if(iFlag)
	{
		rst  = Tddli_Close();
		printf("Tddli_Close = %x \n",rst);
		iFlag = 0;
	}
	return 1;
}
