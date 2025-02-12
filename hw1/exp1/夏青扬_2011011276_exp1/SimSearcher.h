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
public:
	SimSearcher();
	~SimSearcher();

	int createIndex(const char *filename , unsigned q);
	int searchJaccard(const char *query , double threshold , std::vector<std::pair<unsigned, double> > &result);
	int searchED(const char *query , unsigned threshold , std::vector<std::pair<unsigned, unsigned> > &result);
	unsigned verify(string , string , int);
	int min_2(int , int);
	int min_3(int , int , int);

private:
	unsigned q_gram;
    	unsigned MinLength;//无符号数
  	 unsigned ** space;
   	 map<string , vector<int> > List;
   	 vector<string> dictionary;
};

