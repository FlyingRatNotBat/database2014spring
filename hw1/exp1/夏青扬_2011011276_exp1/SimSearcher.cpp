#include "SimSearcher.h"
#include <fstream>
#include <cstring>
#include <algorithm>
#include <cstdlib>
#include <iostream>

using namespace std;

SimSearcher::SimSearcher()
{
    space = new unsigned * [4000];
    for(int i = 0; i < 4000; i++)
        space[i] = new unsigned[4000];
}

SimSearcher::~SimSearcher()
{
    for(int i = 0; i < 4000; i++){
        if(!space[i])
            delete[] space[i];
    }
    delete[] space;
}

bool gotosort(pair<unsigned, unsigned> a, pair<unsigned , unsigned> b){
    return (a.first < b.first);
}

int SimSearcher::createIndex(const char *filename , unsigned q)
{
	ifstream fin(filename);
	 string word , index;
	q_gram = q;
	MinLength = -1;
	double Id = 0;
	while(getline(fin , word))
	{
		dictionary.push_back(word);//记录每一行的内容

		MinLength = min_2(word.length() , MinLength);

		unsigned num = 0;
		while (num < word.length() + 1 - q)
		{
			index = word.substr(num , q);
			List[index].push_back(Id);
			num++;
		}//while
		
		Id++;
	}//while	    
	return SUCCESS;
}

int SimSearcher::searchJaccard(const char *query, double threshold, vector<pair<unsigned, double> > &result)
{
	result.clear();
  	string query_str(query);//

	vector<int> filter;//

	int tmp = 0;
	int tmp1 = 0;

    	int * temp1 = new int[dictionary.size()];
	int * temp2 = new int[dictionary.size()];

    	memset(temp1 , 0 , sizeof(unsigned) * dictionary.size());//记录index的frequence.
    	memset(temp2 , 0 , sizeof(int)* dictionary.size());//记录当前index是否出现过

    	double t = ((query_str.length() + (int)MinLength - 2 * q_gram + 2) * threshold) / (threshold + 1);//第一个阈值

	while(tmp < query_str.length() - (int)q_gram + 1)
	{
		string gram = query_str.substr(tmp , q_gram);//get q-gram of  quray

		vector<int>gramlist = List[gram];//根据q-gram获得倒排列表中的ID序列。

		for (int i = 0; i < (int)gramlist.size(); i++)
		{
			tmp1 = gramlist[i];//获取ID序列中的第i个位置的值
			if ((t <= temp1[tmp1]++) && temp2[tmp1] == 0)
			{
				filter.push_back(tmp1);
				temp2[tmp1] = 1;
			}//if
		}//for
		tmp++;
	}//while

	for(int i = 0; i < (int)filter.size(); i++)
		result.push_back(pair<unsigned , double>(filter[i] , (double)temp1[filter[i]] / (double)(query_str.length() + dictionary[filter[i]].length() - temp1[filter[i]] -2 * q_gram  + 2)));
	bool check;
	sort(result.begin(), result.end(), gotosort);

	delete[] temp1;
	delete[] temp2;
	/*********************************/
	cout << "*****************************************"<< endl;
	cout << "jaccard  SUCCESS!" << endl;
	
	cout << "results :" << endl;
		for(std::vector<pair<unsigned,double> > :: iterator it2 = result.begin(); it2 != result.end(); it2++)
		{
			cout << it2->first << "     "  << it2->second << endl;
		}
		//double a;
		//cin >> a;
	cout << "*****************************************"<< endl;
	/*********************************/
    	return SUCCESS;
}

int SimSearcher::searchED(const char *query, unsigned threshold, vector<pair<unsigned, unsigned> > &result)
{
	result.clear();
    string query_str(query);
	vector<int> filter;
    int * temp1 = new int[dictionary.size()];
    memset(temp1 , 0 , sizeof(unsigned) * dictionary.size());
    int t = query_str.length() - (int)q_gram + 1 - (int)threshold * (int)q_gram;

	if(t <= 0){
		int tmp = 0;
		while (tmp < (int)dictionary.size()){
			unsigned Ed = verify(query_str , dictionary[tmp] , threshold);
			if (Ed <= threshold)
				result.push_back(pair<unsigned , unsigned>(tmp , Ed));
			tmp++;
		}//while
    }//if
    else{
		int tmp = 0;
		while (tmp < query_str.length() - (int)q_gram + 1){
			string gram = query_str.substr(tmp , q_gram);
			vector<int> gramList = List[gram];
			for(int i = 0; i < (int)gramList.size(); i++){
                if(temp1[gramList[i]] == -1)
                    continue;
                if(temp1[gramList[i]] < t)
                    temp1[gramList[i]]++;
                else{
                    temp1[gramList[i]] = -1;
                    filter.push_back(gramList[i]);
                }//else
            }//for
			tmp++;
		}//while
		tmp = 0;
		while (tmp < (int)filter.size()){
			string tempStr = dictionary[filter[tmp]];
            unsigned ed = verify(query_str , tempStr , threshold);
            if(ed <= threshold)
                result.push_back(pair<unsigned , unsigned>(filter[tmp] , ed));
			tmp++;
		}//while
    }//else
    sort(result.begin() , result.end() , gotosort);
    delete[] temp1;
    	
    	/*********************************/
	cout << "ED  SUCCESS!" << endl;
	cout << "results :" << endl;
		for(std::vector<pair<unsigned,unsigned> > :: iterator it2 = result.begin(); it2 != result.end(); it2++)
		{
			cout << it2->first << "     "  << it2->second << endl;
		}
	/*********************************/
	
    return SUCCESS;
    
}

int SimSearcher::min_2(int a , int b){
	int res = a;
	if (b < res)
		res = b;
	return res;
}

int SimSearcher::min_3(int a , int b , int c){
	int res = a;
	if (b < res)
		res = b;
	if (c < res)
		res = c;
	return res;
}

unsigned SimSearcher::verify(string a , string b , int c){
	int len_a = a.length();
	int len_b = b.length();
    if((len_a - len_b > c) || (len_b - len_a > c))
        return -1;
    space[0][0] = 0;
    for(int i = 1; i <= c; i++){
        space[i][0] = space[i - 1][0] + 1;
        space[0][i] = space[0][i - 1] + 1;
	}//for
    for(int i = 1; i <= len_a; i++){
		for(int j = 1; j <= len_b; j++){
			if( j - i > c ) break;
			if( i - j > c) continue;
			if( j - i + 1 > c){
				space[i][j] = min_2(space[i][j - 1] + 1, space[i - 1][j - 1] + (a[i - 1] != b[j - 1]));
				continue;
			}//if
			if( i - j + 1 > c){
				space[i][j] = min_2(space[i - 1][j] + 1, space[i - 1][j - 1] + (a[i - 1] != b[j - 1]));
				continue;
			}//if
            space[i][j] = min_3(space[i - 1][j] + 1, space[i][j - 1] + 1 , space[i - 1][j - 1] + (a[i - 1] != b[j - 1]));
		}//for
	}//for
    return space[a.length()][b.length()];
}
