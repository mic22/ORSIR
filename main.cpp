#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

using namespace std;

#define PROJECTID 0xABCDEF

int main(int argc, char **argv)
{
	key_t key;
	int id,size,flag;
	struct shmid_ds buffer;
	//Inicjujemy wartości aktualne dla przyszłego wywołania shmget()
	key = ftok( "/tmp",PROJECTID );
	flag = IPC_CREAT | 0x100 | 0x80;
	size = 1024*64;
	//Tworzymy segment pamięci
	id = shmget( key,size,flag );
	
	if( id>0 )
	{
		//Wyświetlenie informacji diagnostycznych
		printf( "SEGMENT [key=0x%x,id=%u],process=%u\n",
		id,(unsigned)key,(unsigned)getpid() );
		//Informacje o segmencie 
		(void)shmctl( id,IPC_STAT,&buffer );
		printf( "właściciel %u\n",(unsigned)(buffer.shm_cpid) );
		printf( "rozmiar %u [B]\n",(unsigned)(buffer.shm_segsz) );
		printf( "%s\n",ctime( &(buffer.shm_ctime) ) );
		//Zwalniamy i kończymy
		(void)shmctl( id,IPC_RMID,&buffer );
	}
	else
	{
		perror( "!.!.!..shmget()..." ); exit( 1 );
	}
	return 0;

}

