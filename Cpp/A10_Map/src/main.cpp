
#include <setjmp.h>
#include <iostream>
#include <My_Basics.hpp>
#include <map>
#include <cstring>

using namespace std;

class name {
	char str[40];
public:
	name() {strcpy(str, "");}
	name(char *s) {strcpy(str, s);}
	char *get() {return str;}
};

bool operator<(name a, name b)
{
	return strcmp(a.get(), b.get()) < 0;
}

class phoneNum {
	char str[80];
public:
	phoneNum() {strcmp(str, "");}
	phoneNum(char *s) {strcpy(str, s);}
	char *get(){return str;}
};

int main(int argc, char *argv[])
{
	map<name, phoneNum>directory;
	
	directory.insert(pair<name, phoneNum>("Tom", "123-4567"));
	directory.insert(pair<name, phoneNum>("Chris", "765-4321"));
	
	map<name, phoneNum>::iterator p;
	
	//char str[40] = "Tom";
	char str[40] = "Chris";
	
	p = directory.find(name(str));
	if(p != directory.end()){
		cout << "Phone number : " << p->second.get() << endl;
	}else{
		cout << "Name not in directory" << endl;
	}
	
	return 0;
}
