#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <signal.h>

#define Field_Size 10
#define Queue_Size 500

#define Shell_Price 1
#define Good_Price 10

#define Number_of_Goods 50

int id;
int sem_id;

int max_shot;
int Field_Length;
int goods_left;

key_t key;
key_t sem_key;

char* field_addr;
char* enemy_field_addr;
char* q_addr;
	
int shot = 0;

void SigHndlr(int s){
	printf("Taranteria's goods left:%d\n" , goods_left);
	printf("Anchuaria shooted %d times\n" , shot);	
	shmdt(field_addr);
	shmctl(id , IPC_RMID , NULL);
	semctl(sem_id , IPC_RMID , 0);
	printf("Anchuaria terminated\n");
	kill(getpid() , SIGTERM);
}


int main(int argc , char* argv[]){

	max_shot = (Number_of_Goods * Good_Price)/Shell_Price;
	Field_Length = Field_Size * Field_Size;
	goods_left = Number_of_Goods;

	key = ftok("file.txt" , 3);
	sem_key = ftok("file.txt" , 4);
	
	id = shmget(key , Queue_Size + 2 * Field_Length , IPC_CREAT|0666);
	sem_id = semget(sem_key , 1 , IPC_CREAT|0666);
	
	field_addr = shmat(id , NULL , 0);
	enemy_field_addr = field_addr + Field_Length;
	q_addr = field_addr + 2 * Field_Length;


	int son_pid;
	struct sembuf sops;	
	int number_of_shots;
	int number = 0;
	int target;
	int i;
	int j;

	srand(getpid());

	for (i = 0 ; i < 2 * Field_Length ; i++)
		field_addr[i] = 0;

	for (i = 0 ; i < Number_of_Goods ; i++){
		target = rand() % Field_Length;
		while (field_addr[target]){
			target = rand() % Field_Length;
		}
		field_addr[target] = 1;
	}

	for (i = 0 ; i < Number_of_Goods ; i++){
		target = rand() % (Field_Length) + Field_Length;
		while (field_addr[target]){
			target = (rand() % Field_Length) + Field_Length;
		}
		field_addr[target] = 1;
	}

	printf("Anchuaria Field:\n");
        for (i = 0 ; i <  Field_Size ; i++){
		for (j = 0 ; j < Field_Size ; j++)
			printf("%d " , field_addr[(Field_Size * i) + j]);
		printf("\n");
	}
	printf("Taranteria Field:\n");
        for (i = Field_Size ; i < 2 * Field_Size ; i++){
		for (j = 0 ; j < Field_Size ; j++)
			printf("%d " , field_addr[(Field_Size * i) + j]);
		printf("\n");
	}

	for (i = 0 ; i < Queue_Size ; i++)
		q_addr[i] = (rand() % 4) + 1;

	printf("Queue of shooting:\n");
	for (i = 0 ; i < Queue_Size ; i++)
		printf ("%d " ,q_addr[i]);
	printf("\n");

	semctl(sem_id , 0 , SETVAL , 0);
	sops.sem_num = 0;
	sops.sem_flg = 0;
	sops.sem_op = -1;

	if (son_pid = fork() == 0){

		execlp("./prog_6" ,"./prog_6" , NULL);

	}

	signal(SIGUSR1 , SigHndlr);

	while ((shot < max_shot) && goods_left){
		semop(sem_id , &sops , 1);
		printf("Anchuaria is shooting:\n");
		number_of_shots = q_addr[number];
		for (i = 0 ; i < number_of_shots ; i++){
			target = rand() % Field_Length;
			shot++;
			if (enemy_field_addr[target] == 1){
				printf("Anchuaria hits the target!\n");
				goods_left--;
				enemy_field_addr[target] = 0;
			}
			else{
				printf("Anchuaria missed!\n");
			}
			if (goods_left == 0){
//				printf("Taranteria's goods left:%d\n" , goods_left);
//				printf("Anchuaria shooted %d times\n" , shot);
				printf("Anchuaria wins!\n");
				kill(son_pid , SIGUSR1);
				kill(getpid() , SIGUSR1);
				break;
			}
			else 
				if (shot == max_shot){
//					printf("Taranteria's goods left:%d\n" , goods_left);
//					printf("Anchuaria shooted %d times\n" , shot);
					printf("Taranteria wins!\n");
					kill(son_pid , SIGUSR1);
					kill(getpid() , SIGUSR1);
				break;
			}
		}
		number = number + 2;
		semop(sem_id , &sops , 1);
	}
return 0;
}
