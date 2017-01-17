
#include "stdlib.h"
#include "stdio.h"
#include "stdarg.h"
#include "string.h"

#define __BSG_DIGEST (1UL << 26)
#define __BSG_CONST  (1UL << 31)
#define BSG_U32(value) (__BSG_CONST | __BSG_DIGEST | 4UL), (value)

void Build(int* bufsize, int cmd, va_list * args)
{
	va_list args2;
        memcpy(&args2, args, sizeof(args2));
        for (int argc = 0;argc<4; argc++)
        {
		int flags = (int) va_arg(args2, int);
		printf("\r\n flags=%d",flags);
	}
}

void BuildSimple(int* bufsize, int cmd, ...)
{
        va_list args;
        va_start(args, cmd);
        Build(bufsize, cmd, &args);
        va_end(args);
}

int myadd(int start,...)
{
	va_list arg_ptr;
	va_list arg_s;
	int sum=0;
	int nArgValue=start;

	va_start(arg_s,start);
	memcpy( &arg_ptr, &arg_s, sizeof(arg_ptr));
	do
	{
		sum+=nArgValue;
		nArgValue=va_arg(arg_ptr,int);
	}while(nArgValue!=0);
	
	va_end(arg_ptr);
	return sum;
}

int main(int argc,char **argv)
{
	long long ll=0;
	long l=0;
	int i;
	FILE * pf = NULL;
	int len=0;

	//BuildSimple(&len, 10,BSG_U32(10),BSG_U32(11),BSG_U32(12),0);
	BuildSimple(&len,10,10,11,12,0);
	len=myadd(1,2,3,0);
	printf("\r\nmyadd=%d",len);

	len=sizeof(BSG_U32(10));
	printf("\r\n BSG_U32=%d",len);

	len = sizeof(int);
	printf("\r\n int=%d",len);
	len=0;

	len=sizeof(pf);
	printf("\r\nFile*=%d\r\n",len);

	len=sizeof(long);
	printf("\r\nlong=%d\r\n",len);

	pf = fopen("./test.log","w");
	l = (long)pf;
	ll=(long long)pf;
	len=(int)l;
	i=(int)(long long)pf;
	printf("\r\nl=%ld\r\nlen=%d\r\nll=%ld\r\ni=%d\r\n",l,len,ll,i);

	if(pf)
	{
		fclose(pf);
		pf=NULL;
	}

	return 0;
}
