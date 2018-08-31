#include "hash.h"
using namespace std;

unsigned long long power(unsigned long long base, int n)
{
	if(n==0)
		return 1;
	if(n==1)
	    return base;
	else
		return base*power(base, n-1);
}

unsigned long long calculateHash(string mystring)
{
  /* add your code here */
  unsigned long long hashResult = 0;
  int n = mystring.length();
	for(int i=0; i<n; i++)
	{
		// pn + (128 * pn-1) + (128^2 * pn-2) + ... + (128^(n-1) * p1)
		// hashResult += ( (128)^(n-1-i) )*(int)mystring[i];
		hashResult += power(128, n-1-i)*(int)mystring[i];
	}

	unsigned int w[4];


	for(int i=0;i<4;i++)
	{
			// w[3-i] = (hashResult/(65521^i))%65521;
			w[3-i] = (hashResult/power(65521,i))%65521;
		//check order
	}

  hashResult = (45912 * w[0] + 35511 * w[1] + 65169 * w[2] + 4625 * w[3]) % 65521;

	// secondStep[0] = (hashResult%65521);
 	//  secondStep[1] = (hashResult/65521)%65521;
 	//  secondStep[2] = (hashResult/(65521^2))%65521;
 	//  secondStep[3] = (hashResult/(65521^3))%65521;
  return hashResult;

}
