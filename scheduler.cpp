#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

#define MAX_PROCESSES 32  /* the maximal number of processes in the system */
#define MAX_NAME_LEN  32

typedef int (*ProcessFunc)(void);

/* Process control block - 
 * holding all process relevant informations 
*/
struct pcb{
	int pid;                   /* ID of the proces */
	int prio;                  /* process priority */
	int attached;              /* 1 if attached to processlist, else 0 */
	ProcessFunc func;             /* pointer to the process function */
	char name[MAX_NAME_LEN];   /* Name of the process */
};

static struct pcb processlist[MAX_PROCESSES];

int process_attach(char *name, int prio, ProcessFunc function)
{
	int i = 0;
	int ret = -1;
	printf("[dbg] process_attach\n");
	while(i < MAX_PROCESSES)
	{
		if(strlen(name) > MAX_NAME_LEN)
		{
			printf("[err] wrong stringlen\n");
			return ret;
		}
			
		if(processlist[i].attached != 1)
		{
			printf("attach process at %d\n", i);
			processlist[i].pid = i;
			strcpy(processlist[i].name, name);
			processlist[i].prio = prio;
			processlist[i].func = function;
			processlist[i].attached = 1;
			ret = i;
			break;
		}
		printf("\n");
		i++;
	}
	return ret; 
	

}

int process_detach(int pid)
{
	printf("detach process at %d\n", pid);
	processlist[pid].attached = 0;
	return 0;
}

/*
 * basic implementation of a RR scheduler 
 */
int scheduler()
{
	int i = 0;
	ProcessFunc func;
	
	while(1)
	{
		for(i = 0; i < MAX_PROCESSES; i++)
		{
			if(processlist[i].attached == 1)
			{
				func = processlist[i].func;
				func();
				//sleep(1);
			}
		}
	}
	return 0;
}

/*** Testdriver ***/
int process0()
{
	printf("0\n");
	return 0;
}

int process1()
{
	printf("1\n");
	return 0;
}

int main()
{
	/*
	 * test run here
	 * */
	printf("basic_scheduler Demo\n");
	int pid = process_attach("process0", 100, process0);
	process_attach("process1", 50,  process1);
	process_detach(pid);
	scheduler();
	return 0;
}
