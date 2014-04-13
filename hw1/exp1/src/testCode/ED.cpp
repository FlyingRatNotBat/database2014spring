#include <iostream>
#include <string>
using namespace std;

int main(int argc, char const *argv[])
{
	/* code */
	string s1 = "writers";
	string s2 = "vintner";
	/*
	string s1 = argv[1];
	string s2 = argv[2];
	*/
	int i = s1.length();
	int j = s2.length();
	int ed[i+1][j+1];
	for(int k = 0; k <= i; k++)
		ed[k][0] = k;
	for(int k = 0; k <= j; k++)
		ed[0][k] = k;
	
	for(int k = 1; k <= i; k++)
	{
		for(int l = 1; l <= j; l++)
		{
			int tmp1 = ed[k-1][l]+1, tmp2 =ed[k][l-1]+1;
			int tmp3;
			int min;
			if(s1[k-1] == s2[l-1])
				tmp3 = ed[k-1][l-1];
			else
				tmp3 = ed[k-1][l-1]+1;
			if(tmp1 <= tmp2)
				min = tmp1;
			else
				min= tmp2;
			if(min <= tmp3)
				ed[k][l] = min;
			else
				ed[k][l] = tmp3;

			cout << ed[k][l] << " ";
		}
		cout << endl;
	}

	return 0;
}