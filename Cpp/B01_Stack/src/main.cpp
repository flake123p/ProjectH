
#include <setjmp.h>
#include <iostream>
#include <My_Basics.hpp>

using namespace std;

#define N 6

class node{
	friend class list;
	int data;
	node *link;
};

class list{
	private:
		node *top;
	public:
		list();
		~list();
		bool empty();
		void push(int key);
		int pop();
};

list::list(void)
{
	top = new node;
	top->link = NULL;
}

list::~list(void)
{
	node *link_node, *temp_node;
	
	if(top->link != NULL)
	{
		link_node = top->link;
		while(link_node->link != NULL)
		{
			temp_node = link_node;
			link_node = link_node->link;
			delete temp_node;
		}
		delete link_node;
	}
	delete top;
}

bool list::empty(void)
{
	if(top->link == NULL)
		return true;
	else
		return false;
}

void list::push(int key)
{
	node *new_node;
	new_node = new node;
	new_node->data = key;
	new_node->link = top->link;
	top->link = new_node;
}

int list::pop(void)
{
	int key;
	node *temp_node;
	temp_node = top->link;
	top->link = temp_node->link;
	key = temp_node->data;
	delete temp_node;
	return key;
}

int main(int argc, char *argv[])
{
	int i;
	int arr[N] = {11, 12, 13, 14, 15, 16};
	list stack;
	cout << "put data into stack..." << endl;
	for(i=0; i<N; i++)
	{
		cout << "move <" << arr[i] << "> to stack " << i << endl;
		stack.push(arr[i]);
	}
	
	cout << "get data from top side..." << endl;
	i = 5;
	
	while(!stack.empty())
	{
		cout << "get <" << stack.pop() << "> from stack " << i-- << endl;
	}
	
	return 0;
}
