#include "heap.h"
#include <iostream>

using namespace std;

int main()
{
	Heap<int> h1(4);

	int arr[11] = {5,10,15,20,25,30,35,40,45,50,55};

	for(int i=0;i<11;i++)
	{
		h1.push(arr[i]);
	}

	for(int i=0;i<h1.size();i++)
	{
		cout<<"result: "<<h1.get(i)<<endl;
	}

	//new test file

	string cmd;
	int input;
	while(true)
	{

		cout<<"cmd input"<<endl;
		cin>>cmd;
		if(cmd == "quit")
		{
			break;
		}else if(cmd == "push")
		{
			cin>>input;
			h1.push(input);
		}else if (cmd == "pop")
		{
			h1.pop();
		}

		for(int i=0;i<h1.size();i++)
		{
			cout<<"result: "<<h1.get(i)<<endl;
		}

		cout<<endl;
	}

	// ObjAComparator c1( );
	// Heap<ObjA, ObjAComparator> h2<2, c1>;
}