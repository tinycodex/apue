/**
 * copy from https://cloud.tencent.com/developer/article/2091221
 */


#include <stdio.h>
#include <setjmp.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <execinfo.h>

#define ARRAY_SIZE(a) sizeof(a)/sizeof(a[0])

#define DEBUG

#ifdef DEBUG
	#define LOG(fmt,args...) printf("%s():%d " fmt, __FUNCTION__, __LINE__, ##args)
#else
	#define LOG(fmt,args...) 
#endif

void dump_backtrace(void)
{
	int j, nptrs;
	void *buffer[32];
	char **strings;

	nptrs = backtrace(buffer,  ARRAY_SIZE(buffer) );
	printf("backtrace() returned %d addresses:\n", nptrs);

	strings = backtrace_symbols(buffer, nptrs);
	if (NULL == strings)
	{
	   perror("backtrace_symbols");
	   exit(-1);
	}

	for (j = 0; j < nptrs; j++)
	{
	   printf("    %s\n", strings[j]);
    }

	free(strings);
}

void test()
{
	*((int*)0x04)  = 0;
}

void siginfo_handler(int signum, siginfo_t * siginfo, void * context)
{
	LOG("Signal Number: %d\n", signum);	
	LOG("Address: %p\n", siginfo->si_addr);
	dump_backtrace();
	exit(-1);
}

int main()
{
	int ret = 0;
	struct sigaction act;
	act.sa_flags = SA_SIGINFO;
	act.sa_sigaction = siginfo_handler;
	
	sigemptyset(&act.sa_mask);
	ret = sigaction(SIGSEGV, &act, NULL);
	if(0 != ret)
	{
		perror("sigaction() failed");
	}

	test();
	return 0;
}