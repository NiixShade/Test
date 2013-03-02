#include<stdio.h>
#include<stdlib.h>
#include<semaphore.h>
#include<sys/types.h>
#include<sys/sem.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <sched.h>

#define FULL 0
#define EMPTY 1
#define MUTEX 2
#define GEN 3000
#define TATKAL 500
#define WAIT 20


struct sched_param schedparam1,schedparam2,schedparam3;
int policy=SCHED_FIFO;
//int policy=SCHED_RR;
//int policy=SCHED_OTHER; //default scheduling (Realtime RR)

typedef struct booking
{
	int reqNo;
	int nos;
	int status[6];
	int berthNos[6];
} req;

sem_t waiting[2][3], gen[2][2], tat[2][2], berth[2][3520], print;
int waitlist[20];

req reqTr[2][5600];
int head[2] = {3500, 3500};
int tail[2] = {3500, 3500};

void *cancellation_function( void *ptr );
void *tatkal_function( void *ptr );
void* booking_function(void* ptr);

main()
{
pthread_t booking[8000], cancellation[1600], tatkal[1500];
pthread_attr_t at1,at2,at3;
int message[2];
int iret1, iret2, iret3, hh, d, reqph[6], days = 120, bookReqNo[2] = {0, 4000}, cancelReqNo[2] = {8000, 9000}, tatkalReqNo[2] = {10000, 11000};
FILE *file;
int i,j;
file = fopen("daily.txt","w");
fprintf(file," ");
fclose(file);
struct timespec tp, slp;
	slp.tv_nsec = 1333333;

if(sem_init(&print, 0, 1)){
	printf("Semaphore print error");
	exit(1);
}
if(sem_init(&gen[0][EMPTY], 0, GEN)){
	printf("Semaphore gen0 error");
	exit(1);
}
if(sem_init(&gen[1][EMPTY], 0, GEN)){
	printf("Semaphore gen1 error");
	exit(1);
}
if(sem_init(&tat[0][EMPTY], 0, TATKAL)){
	printf("Semaphore tat0 error");
	exit(1);
}
if(sem_init(&tat[0][EMPTY], 0, TATKAL)){
	printf("Semaphore tat1 error");
	exit(1);
}
if(sem_init(&waiting[0][EMPTY], 0, WAIT)){
	printf("Semaphore wait0 error");
	exit(1);
}
if(sem_init(&waiting[1][EMPTY], 0, WAIT)){
	printf("Semaphore wait1 error");
	exit(1);
}

if(sem_init(&gen[0][FULL], 0, 0)){
	printf("Semaphore gen0 error");
	exit(1);
}
if(sem_init(&gen[1][FULL], 0, 0)){
	printf("Semaphore gen1 error");
	exit(1);
}
if(sem_init(&tat[0][FULL], 0, 0)){
	printf("Semaphore tat0 error");
	exit(1);
}
if(sem_init(&tat[0][FULL], 0, 0)){
	printf("Semaphore tat1 error");
	exit(1);
}
if(sem_init(&waiting[0][FULL], 0, 0)){
	printf("Semaphore wait0 error");
	exit(1);
}
if(sem_init(&waiting[1][FULL], 0, 0)){
	printf("Semaphore wait1 error");
	exit(1);
}
if(sem_init(&waiting[0][MUTEX], 0, 1)){
	printf("Semaphore wait0 error");
	exit(1);
}
if(sem_init(&waiting[1][MUTEX], 0, 1)){
	printf("Semaphore wait1 error");
	exit(1);
}
for(i = 0; i < 3500; i++)
if(sem_init(&berth[0][i], 0, 1)){
	printf("Semaphore train 0 berth %d error", i);
	exit(1);
}
for(i = 0; i < 3500; i++)
if(sem_init(&berth[1][i], 0, 1)){
	printf("Semaphore train 1 berth %d error", i);
	exit(1);
}

schedparam1.sched_priority = 10;
sched_setscheduler(getpid(),policy,&schedparam1);
sched_rr_get_interval(getpid(),&tp);

printf("\nTime Interval %ld\n",tp.tv_nsec);
schedparam1.sched_priority = 10; //Set Nice value of Thread
schedparam2.sched_priority = 10;
schedparam3.sched_priority = 10;

/* Set attributes*/

pthread_attr_init(&at1);
pthread_attr_setinheritsched(&at1, PTHREAD_EXPLICIT_SCHED); // To inherit parent thread properties use PTHREAD_INHERIT_SCHED
pthread_attr_setschedpolicy(&at1, policy);
pthread_attr_setschedparam(&at1, &schedparam1);

pthread_attr_init(&at2);
pthread_attr_setinheritsched(&at2, PTHREAD_EXPLICIT_SCHED);
pthread_attr_setschedpolicy(&at2,policy);
pthread_attr_setschedparam(&at2, &schedparam2);

pthread_attr_init(&at3);
pthread_attr_setinheritsched(&at3, PTHREAD_EXPLICIT_SCHED);
pthread_attr_setschedpolicy(&at3, policy);
pthread_attr_setschedparam(&at3, &schedparam3);

for(d = 0; d < days; d++)
{
	for(hh = 0; hh < 48; hh++)
	{
		
		//if(!(d == days - 2 && hh == 0))
		//{
			reqph[0] = (rand()%2+rand()%3)/2;
			reqph[1] = (rand()%2+rand()%3)/2;
			reqph[2] = rand()%2;
			reqph[3] = rand()%2;
			reqph[4] = rand()%15;
			reqph[5] = rand()%14;
		//}
		//else
		//{
			
		//}
		if (!(days == 120 && hh >= 47))
		{
			// Printing chart
			
		}
		if (!(days == 120 && hh >= 46))
		{
			message[0] = 0;
		for(i = 0; i < reqph[0]; i++)
		{
			if(bookReqNo[0] < 4000)
			{
				message[1] = bookReqNo[0];
				//printf("Train BOOKing 1: %d %d", bookReqNo[0], message[0]);
				iret1 = pthread_create( &booking[bookReqNo[0]++], &at1, booking_function, (void*) message);
			}
		}
			message[0] = 1;
		for(i = 0; i < reqph[1]; i++)
		{
			if(bookReqNo[1] < 8000)
			{
				message[1] = bookReqNo[1];
				//printf("Train BOOKing 2: %d", bookReqNo[1]);
				iret1 = pthread_create( &booking[bookReqNo[1]++], &at1, booking_function, (void*) message);
			}
		}
		}
		if(d >= days - 7 && !(days == 120 && hh >= 46))
		{
			message[0] = 0;
			for(i = 0; i <= reqph[2]; i++)
			{
				if(cancelReqNo[0] < 8800)
				{
					message[1] = cancelReqNo[0];
					//printf("Train Cancel 1: %d", cancelReqNo[0]+ 1 - 8000);
					iret2 = pthread_create( &cancellation[cancelReqNo[0]++ - 8000], &at2, cancellation_function, (void*) message);
				}
			}
			message[0] = 1;
			for(i = 0; i <= reqph[3]; i++)
			{
				if(cancelReqNo[1] < 9800)
				{
					message[1] = cancelReqNo[1];
					//printf("Train Cancel 2: %d", cancelReqNo[1]+ 1 - 9000 + 800);
					iret2 = pthread_create( &cancellation[cancelReqNo[1]++ - 9000 + 800], &at2, cancellation_function, (void*) message);
				}
			}
			if(d >= days - 2 && !(days == 120 && hh >= 46))
			{
				message[0] = 0;
				for(i = 0; i <= reqph[4]; i++)
				{
					if(tatkalReqNo[0] < 10750)
					{
						message[1] = tatkalReqNo[0];
						//printf("Train TATKAL 1: %d", tatkalReqNo[0] + 1 - 10000);
						iret3 = pthread_create( &tatkal[tatkalReqNo[0]++ - 10000], &at3, tatkal_function, (void*) message);
					}
				}
				message[0] = 1;
				for(i = 0; i <= reqph[5]; i++)
				{
					if(tatkalReqNo[1] < 11750)
					{
						message[1] = tatkalReqNo[1];
						//printf("Train TATKAL 2: %d", tatkalReqNo[1] + 1 - 11000 + 750);
						iret3 = pthread_create( &tatkal[tatkalReqNo[1]++ - 11000 + 750], &at3, tatkal_function, (void*) message);
					}
				}
			}
		}
		nanosleep(&slp, NULL);
		//pthread_cancel();
	}
	file = fopen("daily.txt","a");
	fprintf(file,"\nDay %d\nreqNo\tbooking\tstatus\n################################\n",d);
	fprintf(file,"\nTrain 1\n");	
	for( i=0; i<5600; i++)
	{		
		for( j=0; j<reqTr[1][i].nos; j++)
		{		
			if(reqTr[1][i].status[j] == 1)
				fprintf(file,"%d\t%d\tconf\n",reqTr[1][i].reqNo,reqTr[1][i].berthNos[j]);
		}
	}
	fprintf(file,"\nTrain 2\n");	
	for( i=0; i<5600; i++)
	{		
		for( j=0; j<reqTr[1][i].nos; j++)
		{		
			if(reqTr[1][i].status[j] == 1)
				fprintf(file,"%d\t%d\tconf\n",reqTr[1][i].reqNo,reqTr[1][i].berthNos[j]);
		}
	}
	fclose(file);
	
}
printf("\n\n\nFinal Reservation Chart\nDay %d\nreqNo\tbooking\tstatus\n################################\n",d);
printf("\nTrain 1\n");	
for( i=0; i<5600; i++)
{		
	for( j=0; j<reqTr[1][i].nos; j++)
	{		
		if(reqTr[1][i].status[j] == 1)
			printf("%d\t%d\tconf\n",reqTr[1][i].reqNo,reqTr[1][i].berthNos[j]);
	}
}
printf("\nTrain 2\n");	
for( i=0; i<5600; i++)
{		
	for( j=0; j<reqTr[1][i].nos; j++)
	{		
		if(reqTr[1][i].status[j] == 1)
			printf("%d\t%d\tconf\n",reqTr[1][i].reqNo,reqTr[1][i].berthNos[j]);
	}
}
//for(i = 0; i < 8000; i++)	pthread_join( booking[i], NULL);
//pthread_join( cancellation[0], NULL);
//pthread_join( tatkal[0], NULL);

printf("\nThread create 1 returns: %d\n", iret1);
printf("Thread create 2 returns: %d\n", iret2);
printf("Thread create 3 returns: %d\n", iret3);
exit(0);

}

void* booking_function(void* ptr)
{
	req *data;
	int bookReqNo = ((int*)ptr)[1];
	int trNo = (bookReqNo >= 4000)?1:0; //((int*)ptr)[0];
	int i, b, r = bookReqNo - (trNo*4000);
struct timespec slp;
	slp.tv_nsec = 43333333;
	reqTr[trNo][r].reqNo = bookReqNo;

	if(rand()%2)
		reqTr[trNo][r].nos = 2;
	else
		reqTr[trNo][r].nos = rand()%7;

	b = 0;
	for(i = 0; i < reqTr[trNo][r].nos; i++)
	{
		if(sem_trywait(&gen[trNo][EMPTY]) == 0)
		{
			for(;b < 3000; b = (b+1)%3000)
			{
				if(sem_trywait(&berth[trNo][b]) == 0)
				{
					reqTr[trNo][r].berthNos[i] = b;
					break;
				}
			}
			sem_post(&gen[trNo][FULL]);
			b=(b+1)%3000;
			reqTr[trNo][r].status[i] = 1;
		}
		else if(sem_timedwait(&waiting[trNo][EMPTY], &slp) == 0)
		{
			sem_wait(&waiting[trNo][MUTEX]);
			reqTr[trNo][r].berthNos[i] = tail[trNo];
			sem_wait(&berth[trNo][tail[trNo]]);
			waitlist[tail[trNo]%20] = bookReqNo;
			tail[trNo] = 3500 + (tail[trNo] + 1)%20;
			sem_post(&waiting[trNo][MUTEX]);
			reqTr[trNo][r].status[i] = 0;
			sem_post(&waiting[trNo][FULL]);
		}
		else
		{
			for(; i < reqTr[trNo][r].nos; i++)
				reqTr[trNo][r].status[i] = 3;
			break;
		}
	}
	sem_wait(&print);
				//	printf("\nafter%d %d %d %d %d %d\n",reqTr[trNo][r].berthNos[0], reqTr[trNo][r].berthNos[1],reqTr[trNo][r].berthNos[2],reqTr[trNo][r].berthNos[3],reqTr[trNo][r].berthNos[4],b);
	printf("Book %5d\t%d\t%d\t", reqTr[trNo][r].reqNo, trNo+1, reqTr[trNo][r].nos);
	for(i = 0; i < reqTr[trNo][r].nos; i++)
		if(reqTr[trNo][r].status[i] == 3) printf("Fail "); else if(reqTr[trNo][r].status[i] == 0) printf("Waitlisted "); else printf("Success ");
	for(i = 0; i < reqTr[trNo][r].nos; i++)
		if(reqTr[trNo][r].status[i] == 1) printf("%d ", reqTr[trNo][r].berthNos[i]); else printf("NA ");

	 printf("\n");
	sem_post(&print);
	return;
}

void* tatkal_function(void* ptr)
{
	req *data;
	//int trNo = ((int*)ptr)[0];
	int tatkalReqNo = ((int*)ptr)[1];
	int trNo = (tatkalReqNo >= 11000)?1:0;
	int i, b, r = tatkalReqNo - (trNo*1000) - 10000 + 4000;
struct timespec slp;
	slp.tv_nsec = 43333333;
	reqTr[trNo][r].reqNo = tatkalReqNo;

	
	if(rand()%2)
		reqTr[trNo][r].nos = 2;
	else
		reqTr[trNo][r].nos = rand()%7;

	b = 3000;
	for(i = 0; i < reqTr[trNo][r].nos; i++)
	{
		if(sem_trywait(&tat[trNo][EMPTY]) == 0)
		{
			for(;b < 3500; b = 3000 + (b+1)%500)
			{
				if(sem_trywait(&berth[trNo][b]) == 0)
				{
					reqTr[trNo][r].berthNos[i] = b;
					break;
				}
			}
			sem_post(&tat[trNo][FULL]);
			b=(b+1)%3000;
			reqTr[trNo][r].status[i] = 1;
		}
		else if(sem_timedwait(&waiting[trNo][EMPTY], &slp) == 0)
		{
			sem_wait(&waiting[trNo][MUTEX]);
			reqTr[trNo][r].berthNos[i] = tail[trNo];
			sem_wait(&berth[trNo][tail[trNo]]);
			waitlist[tail[trNo]%20] = tatkalReqNo;
			tail[trNo] = 3500 + (tail[trNo] + 1)%20;
			sem_post(&waiting[trNo][MUTEX]);
			reqTr[trNo][r].status[i] = 0;
			sem_post(&waiting[trNo][FULL]);
		}
		else
		{
			for(; i < reqTr[trNo][r].nos; i++)
				reqTr[trNo][r].status[i] = 3;
			break;
		}
	}
	sem_wait(&print);
				//	printf("\nafter%d %d %d %d %d %d\n",reqTr[trNo][r].berthNos[0], reqTr[trNo][r].berthNos[1],reqTr[trNo][r].berthNos[2],reqTr[trNo][r].berthNos[3],reqTr[trNo][r].berthNos[4],b);
	printf("Tatkal %5d\t%d\t%d\t", reqTr[trNo][r].reqNo, trNo+1, reqTr[trNo][r].nos);
	for(i = 0; i < reqTr[trNo][r].nos; i++)
		if(reqTr[trNo][r].status[i] == 3) printf("Fail "); else if(reqTr[trNo][r].status[i] == 0) printf("Waitlisted "); else printf("Success ");
	for(i = 0; i < reqTr[trNo][r].nos; i++)
		if(reqTr[trNo][r].status[i] == 1) printf("%d ", reqTr[trNo][r].berthNos[i]); else printf("NA ");

	 printf("\n");
	sem_post(&print);
	//printf("Thread TATKAL: %d exiting\n", tatkalReqNo);
	return;
}

void* cancellation_function(void * ptr)
{
	req *data;
	//int trNo = ((int*)ptr)[0];
	int cancelReqNo = ((int*)ptr)[1];
	int trNo = (cancelReqNo >= 9000)?1:0;

	int c, i, b, n, r, s = cancelReqNo - (trNo*1000) - 8000 + 4750;
struct timespec slp;
	slp.tv_nsec = 43333333;

	c = rand()%4750;
	
	reqTr[trNo][r].reqNo = cancelReqNo;
	reqTr[trNo][c].reqNo;

	reqTr[trNo][r].nos = rand()%((reqTr[trNo][c].nos==0)?2:reqTr[trNo][c].nos);

	for(i = 0; i < reqTr[trNo][r].nos; i++)
	{
		if(reqTr[trNo][c].status[i] == 1)
		{
			if(sem_trywait(&berth[trNo][reqTr[trNo][c].berthNos[i]]) == 0)
			{
				sem_post(&berth[trNo][reqTr[trNo][c].berthNos[i]]);
				reqTr[trNo][r].status[i] = 3;
			}
			else
			{
				if(reqTr[trNo][c].berthNos[i]>=3000) sem_wait(&tat[trNo][FULL]); else sem_wait(&gen[trNo][FULL]);
				reqTr[trNo][c].status[i] = 2;
				reqTr[trNo][r].status[i] = 2;
				if(sem_trywait(&waiting[trNo][FULL]) == 0)
				{					
					sem_wait(&waiting[trNo][MUTEX]);
					n = (head[trNo] >= 10000)?(head[trNo]-60000-trNo*1000):((head[trNo]-trNo*4000));
					for(s = 0; (s < reqTr[trNo][n].nos) && (reqTr[trNo][n].status[s] != 0); s++);
					reqTr[trNo][n].berthNos[s] = reqTr[trNo][c].berthNos[i];
					reqTr[trNo][n].status[s] = 1;
					sem_post(&berth[trNo][head[trNo]]);
					head[trNo] = 3500 + (head[trNo] + 1)%20;
					sem_post(&waiting[trNo][MUTEX]);
					sem_post(&waiting[trNo][EMPTY]);
					reqTr[trNo][n].status[s] = 1;
				}
				else{
				sem_post(&berth[trNo][reqTr[trNo][c].berthNos[i]]);
				}
				if(reqTr[trNo][c].berthNos[i]>=3000) sem_post(&tat[trNo][EMPTY]); else sem_post(&gen[trNo][EMPTY]);
			}
		}
		else if(reqTr[trNo][c].status[i] == 0)
		{
			sem_wait(&waiting[trNo][FULL]);
			sem_wait(&waiting[trNo][MUTEX]);
			reqTr[trNo][r].berthNos[i] = tail[trNo];
			for(b = reqTr[trNo][c].berthNos[i]%20; b > head[trNo]%20; b=(b-1)%20)
			{
				waitlist[b] = waitlist[(b-1)%20];
			}
			sem_post(&berth[trNo][head[trNo]]);
			head[trNo] = 3500 + (head[trNo] + 1)%20;
			sem_post(&waiting[trNo][MUTEX]);
			sem_post(&waiting[trNo][EMPTY]);
			reqTr[trNo][r].status[i] = 2;
		}
		else 
		{
			reqTr[trNo][r].status[i] = 3;
		}
	}
	sem_wait(&print);
	printf("Cancel %5d\t%d\t%d\t", reqTr[trNo][r].reqNo, trNo+1, reqTr[trNo][r].nos);
	for(i = 0; i < reqTr[trNo][r].nos; i++)
		if(reqTr[trNo][r].status[i] == 3) printf("Fail "); else printf("Success ");
	printf("\n");
	sem_post(&print);
	return;
}
