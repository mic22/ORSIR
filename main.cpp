#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string>

using namespace std;

#define PROJECTID 0xFF
#define n 50

void write(void);
void show(void);
void remove(void);

int main(int argc, char *argv[])
{
	string param = argv[1];
	
	if(param == "-w" || param == "--write")
		write();
	else if(param == "-s" || param == "--show")
		show();
	else if(param == "-r" || param == "--remove")
		remove();
	return 0;
}

void write(void)
{
	key_t key;
	int id,size,flag;
	unsigned i;
	//I oczywiście zmienna za pośrednictwem,
	//której będziemy się odwoływać do segmentu wspólnego
	unsigned *array;
	printf("GENERATOR [%u]\n",(unsigned)getpid());
	key = ftok("/tmp", PROJECTID);
	flag = IPC_CREAT | 0x1B6; 
	size = n*sizeof(unsigned);
	id = shmget(key, size, flag);
	
	if(id > 0)
	{
		printf("Utworzono segment wspólny [%u][0x%x]\n\t", id, key);
		array = (unsigned*)shmat(id, NULL, 0);
		for(i = 0; i < n; i++)
		{ 
			*(array + i) = (3 * (i + 1)); 
			fflush(stdout); 
		}
		shmdt((void*)array);
		printf("Zakończono inicjację segmentu wspólnego\n");
	}
	else
	{
		perror("Błąd segmantacji\n");
		exit(1);
	}
}


void show(void)
{
	key_t key;
	int id,size,flag;
	unsigned i,*array;
	key = ftok("/tmp", PROJECTID);
	flag = IPC_CREAT | 0x1B6;
	size = n*sizeof(unsigned);
	id = shmget(key, size, flag);
	if(id > 0)
	{
		array = (unsigned*)shmat(id, NULL, 0);
		printf("%u liczb podzielnych przez 3 odczytanych z [%u][0x%x]\n", n, id, key);
		for(i = 0; i < n; i++)
		{
			printf("%10u", *(array+i));
			printf("%c", !((i+1)%5)?('\n'):(' '));
		}
		printf("%c", '\n');
		shmdt((void*)array);
	}
	else
	{
		perror("Błąd segmentacji\n");
		exit(1);
	}
}

void remove(void)
{
	key_t key;
	int id,size,flag;
	struct shmid_ds buffer;
	key = ftok("/tmp", PROJECTID);
	flag = IPC_CREAT | 0x1B6;
	size = n*sizeof(unsigned);
	id = shmget(key, size, flag);
	if(id > 0)
	{
		(void)shmctl(id, IPC_RMID, &buffer);
	}
}
