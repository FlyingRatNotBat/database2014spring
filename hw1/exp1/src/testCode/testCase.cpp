#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <time.h>

//#include <>
using namespace std;
int main()
{
	srand(time(NULL));
	ofstream fout;
	fout.open("input10.txt",ofstream::out | ofstream::app);
	//random_device rd;
	for(int n = 1; n < 10; n++)
	{
		for(int i = 0; i < 10; i++)
		{
			fout << rand() << ' ';
		}	
		fout << endl;
	}
	
	fout.close();
	return 0;
}