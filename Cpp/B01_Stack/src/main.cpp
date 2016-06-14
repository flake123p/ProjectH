
#include <setjmp.h>
#include <iostream>
#include <My_Basics.hpp>

using namespace std;

#define N 6

void create_empty_stack(void);
void push(int key);
int pop(void);

struct node{
	int data;
	struct node *link;
};

node *top, *rear;

int main(int argc, char *argv[])
{
	int i;
	int arr[N] = {10, 11, 12, 13, 14, 15};
	
	create_empty_stack();
	
	cout << endl << "put data to stack ...  " << endl;
	for(i=0; i<N; i++){
		cout << "sequence <" << i << "> put " << i << endl;
		push(arr[i]);
	}
	
	cout << endl << "get data from top side...  " << endl;
	i = 0;
	
	while(top->link){
		cout << "sequence <" << i++ << "> get " << pop() << endl;
	}
	delete top;
	
	return 0;
}

void create_empty_stack(void)
{
	top = new (struct node);
	top->link = NULL;
}

void push(int key)
{
	struct node *new_node;
	
	new_node = new (struct node);
	new_node->data = data;
	new_node->link = top->link;
	top->link = new_node;
}

int pop(void)
{
	int key;
	struct node *temp_node;
	
	temp_node = top->link;
	top->link = temp_node->link;
	key = temp_node->data;
	delete temp_node;
	
	return(key);
}
