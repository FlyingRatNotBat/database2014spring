#pragma once
#include <vector>
#include <utility>
#include <map>
#include <string>

using namespace std;

const int SUCCESS = 0;
const int FAILURE = 1;

class SimSearcher
{
	std::map< std::string , std::vector<double> > MYmap;//record the g-gram map
	std::map<double, double> MYLength;//record the length of each line
	std::map<double, std::string> MYdoc;//record the content of each line
	int MYq;
	unsigned ShortestLength;
public:
	SimSearcher();
	~SimSearcher();

	int createIndex(const char *filename, unsigned q);
	int searchJaccard(const char *query, double threshold, std::vector<std::pair<unsigned, double> > &result);
	int searchED(const char *query, unsigned threshold, std::vector<std::pair<unsigned, unsigned> > &result);
private:
	
};


