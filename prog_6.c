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

int Field_Length = Field_Size * Field_Size;
int goods_left = Number_of_Goods;
int max_shot = Number_of_Goods * Good_Price / Shell_Price;

key_t key;
key_t sem_key;

int id;
int sem_id;

char* enemy_field_addr;
char* field_addr;

char* q_addr;

int shot = 0;

void SigHndlr(int s){
	printf("Anchuaria goods left:%d\n" , goods_left);
	printf("Taranteria shoted %d times\n" , shot);	
	shmdt(enemy_field_addr);
	printf("Taranteria terminated\n");
	kill(getpid() , SIGTERM);
}

int main(int argc , char* argv[]){

	key = ftok("file.txt" , 3);
	sem_key = ftok("file.txt" , 4);

	id = shmget(key , Queue_Size + 2 * Field_Length , 0666);
	sem_id = semget(sem_key , 1 , 0666);
	
	enemy_field_addr = shmat(id , NULL , 0);
	field_addr = enemy_field_addr + Field_Length;
	q_addr = enemy_field_addr + 2 * Field_Length;

	struct sembuf sops;
	int target;
	int number_of_shots;
	int number = 1;
	int i;
	
	semctl(sem_id , 0 , SETVAL , 0);
	sops.sem_num = 0;
	sops.sem_flg = 0;

	signal(SIGUSR1 , SigHndlr);

	while ((shot < max_shot) && goods_left){

		sops.sem_op = 2;
		semop(sem_id , &sops , 1);
		sops.sem_op = 0;
		semop(sem_id , &sops , 1);
		
		printf("Taranteria is shoting:\n");
		number_of_shots = q_addr[number];
		for (i = 0 ; i < number_of_shots ; i++){
			target = rand() % Field_Length;
			shot++;
			if (enemy_field_addr[target] == 1){
				printf("Taranteria hits the target!\n");
				goods_left--;
				enemy_field_addr[target] = 0;
			}
			else{
				printf("Taranteria missed!\n");
			}
			if (goods_left == 0){
//				printf("Anchuaria goods left:%d\n" , goods_left);
//				printf("Taranteria shoted %d times\n" , shot);
				printf("Taranteria wins!\n");
				kill(getppid() , SIGUSR1);
				kill(getpid() , SIGUSR1);
				break;
			}
			else 
				if (shot == max_shot){
//					printf("Anchuaria goods left:%d\n" , goods_left);
//					printf("Taranteria shoted %d times\n" , shot);
					printf("Anchuaria wins!\n");
					kill(getppid() , SIGUSR1);
					kill(getpid() , SIGUSR1);
				break;
	
				}
		}
	       number += 2;
	}
	return 0;
}

