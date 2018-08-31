#include <iostream>
#include <string>
#include "hash.h"

using namespace std;

int main()
{
	string test = "Cs4";
	unsigned long long result = calculateHash(test);
	cout<<result<<endl;
}