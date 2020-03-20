#include <stdio.h>
#include <stdlib.h>

typedef struct node{
	int value;
	struct node *next;
}node;

node* Add_node(node* list,int num){
node* cur;
node* new;
	new =  malloc(sizeof(node));
	new -> value = num;
	new -> next = NULL;

	if (list == NULL)
		list = new;
	else{
		cur = list;
		while (cur -> next != NULL){
			cur = cur -> next;
		}
			cur -> next = new;
	}

return list;
}

void Print_list(node* list){
node* cur;
	cur = list;
	while (cur != NULL){
		printf("%d\n", cur -> value);
		cur = cur -> next;
	}
}

void Free_list(node* list){
node* cur;
node* fol;
	cur = list;
	while (cur != NULL){
		fol = cur -> next;
		free(cur);
		cur = fol;
	}
}

node* List_sort(node* list){
node* cur;
node* a;
node* b;
	if (list == NULL)
		return list;
	else if (list -> next == NULL)
		return list;
	else{
		cur = list;
		while (cur != NULL && cur -> next != NULL){
			a = cur -> next;
			cur -> next = a -> next;
			b = list;
			if (a -> value < b -> value){
				a -> next = list;
				list = a;
			}
			else{
				while ((b != cur) && (b -> next -> value < a -> value)){
					b = b -> next;
				}
				a -> next = b -> next;
				b -> next = a;
				if (cur == b)
					cur = cur -> next;
			}
		}
		
	}
	return list;
}

int main(void){
node* List;
int num;
	List = NULL;
	printf("Enter the numbers(push Ctrl+D to stop):\n");
	scanf("%d",&num);
	while(!feof(stdin)){
		List = Add_node(List,num);
		scanf("%d", &num);
	}
	printf ("\nThis is your list:\n");
	Print_list(List);
	printf ("\nThis is yout sorted list:\n");
	List = List_sort(List);
	Print_list(List);
	Free_list(List);
return 0;
}
