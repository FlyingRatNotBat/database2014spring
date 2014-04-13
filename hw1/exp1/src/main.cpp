#include "SimSearcher.h"
#include <fstream>
#include <string>
using namespace std;

int main(int argc, char **argv)
{
	SimSearcher searcher;

	vector<pair<unsigned, unsigned> > resultED;
	vector<pair<unsigned, double> > resultJaccard;

	unsigned q = 3, edThreshold = 2;
	double jaccardThreshold = 0.75;

	searcher.createIndex(argv[1], q);

	ifstream fin;
	fin.open(argv[2]);
	string quary;
	while(getline(fin, quary))
	{
		searcher.searchJaccard(quary.c_str(), jaccardThreshold, resultJaccard);
		searcher.searchED(quary.c_str(), edThreshold, resultED);
	}
	fin.close();
	return 0;
}

