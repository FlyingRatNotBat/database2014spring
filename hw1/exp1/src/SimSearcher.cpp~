#include "SimSearcher.h"
#include <fstream>
#include <map>
#include <string>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

SimSearcher::SimSearcher()
{
	MYmap.clear();
	MYLength.clear();
	MYdoc.clear();
	MYq = 0;
	ShortestLength = -1;
}

SimSearcher::~SimSearcher()
{
}

int SimSearcher::createIndex(const char *filename, unsigned q)
{
	MYq = (double )q;
	MYmap.clear();
	MYLength.clear();
	MYdoc.clear();

	ifstream fin;
	fin.open(filename, ifstream::in);
	string str;//store the line
	double index = 0;
	string gram;
	while(getline(fin, str))
	{
		//creat index for current data
		MYdoc[index] = str;//记录当前行的信息，用于ED计算以及检索后的输出。
		double line_length = str.length();
		if(line_length < ShortestLength)
		{
			ShortestLength = line_length;
		}
		MYLength[index] = line_length;//记录当前行的长度
		for(double i = 0; i < (line_length-(double)q+1); i++)
		{
			gram = str.substr(i, q);//获取子串
			if(MYmap.find(gram) != MYmap.end())//保证倒排列表中有这个q-gram
			{
				vector<double>::iterator findit = std::find(MYmap[gram].begin(), MYmap[gram].end(), index);
				if(findit == MYmap[gram].end())//表示在MYmap中gram词条下没有index这个元素
				{
					MYmap[gram].push_back(index);//将index压入倒排列表,并去重。
				}
			}
			else
			{
				MYmap[gram].push_back(index);//倒排列表中没有这个q-gram，则建立。
			}
		}
		index++;
	}
	fin.close();
	
	return SUCCESS;
}

double ED(string s1, string s2)
{
	int i = s1.length();
	int j = s2.length();
	double ed[i+1][j+1];
	for(int k = 0; k <= i; k++)
		ed[k][0] = k;
	for(int k = 0; k <= j; k++)
		ed[0][k] = k;
	
	for(int k = 1; k <= i; k++)
	{
		for(int l = 1; l <= j; l++)
		{
			double tmp1 = ed[k-1][l]+1, tmp2 =ed[k][l-1]+1;
			double tmp3;
			double min;
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

			//cout << ed[k][l] << " ";
		}
		//cout << endl;
	}
	return ed[i][j];
}
int SimSearcher::searchJaccard(const char *query, double threshold, vector<pair<unsigned, double> > &result)
{
	result.clear();

	std::map<double, double> rsltFrequence;//记录某个index对应的str的q-gram集合与quary的q-gram集合的交集size。
	rsltFrequence.clear();
	/*
	G_quary and G_data >= max( t*G_quary, (G_quary + G_Smin)*t/(1+t))
	*/
	string str = query;
	double len = str.length();
	string gram;
	
	for(double i = 0; i < (len-MYq+1); i++)
	{
		gram = str.substr(i, MYq);

		double size = MYmap[gram].size();//从倒排列表中获得该q-gram对应的index序列；

		for(double j = 0; j < size; j++)
		{
			double index = MYmap[gram].at(j);

			if(rsltFrequence.find(index) != rsltFrequence.end())
			{
				rsltFrequence[index]++;
			}
			else
			{
				rsltFrequence[index] = 1;
			}
		}
	}
	//到此为止，已经获得了所有的q-gram对应的quary和lines的交集大小。存储在rsltFrequence中，是index～|Q-quary U L-lines|
	std::map<double, double> :: iterator it = rsltFrequence.begin();
	for(; it != rsltFrequence.end(); it++)
	{
		double jaccard = (double)(it->second)/(double)(MYLength[it->first] + len - MYq*2 +2 - it->second);
		cout << "jaccard: " << jaccard << endl;
		if(jaccard >= threshold)
		{
			cout << "push_back: " << it->first << endl;
			result.push_back(pair<unsigned, double> (it->first, jaccard));
		}
		//在遍历map rsltFrequence时，由于是按照index（也就是key值排序），所以在result中的结果也是按照index排序的。
	}

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
	std::map<double, double> rsltFrequence;
	rsltFrequence.clear();
	/*
	Qr and Qs >= max(R,S)-q+1-t*q;
	*/
	string str = query;
	string gram;
	double len = str.length();
	for(double i = 0; i < (len-MYq+1); i++)
	{
		gram = str.substr(i, MYq);
		
		double size = MYmap[gram].size();
		
		for(double j = 0; j < size; j++)
		{
			double index = MYmap[gram].at(j);

			if(rsltFrequence.find(index) != rsltFrequence.end())//当index已经在rsltFrequence中
			{
				rsltFrequence[index]++;
			}
			else
			{
				rsltFrequence[index] = 1;
			}
		}
	}

	double max_R_S = str.length()-MYq+1;
	std::map<double, double> :: iterator it = rsltFrequence.begin();
	for(; it != rsltFrequence.end(); it++)
	{
		
		if(max_R_S < (MYLength[it->first]-MYq+1))
		{
			max_R_S = MYLength[it->first]-MYq+1;
		}
		double bouder = (max_R_S - (double)MYq + 1.0 - ((double)threshold)*MYq);//获得一个检测边界
		if(it->second >= bouder)
		{
			double currentED = ED(str, MYdoc[it->first]);
			if(currentED <= threshold)
			{
				cout << "push_back: " << it->first << endl;
				cout << "ED:" << currentED << endl;
				cout << "doc: " << MYdoc[it->first] << endl;
				result.push_back(pair<unsigned, unsigned> (it->first, currentED));
			}
			
		}
	}
	/*********************************/
	cout << "ED  SUCCESS!" << endl;
	
	cout << "results :" << endl;
		for(std::vector<pair<unsigned,unsigned> > :: iterator it2 = result.begin(); it2 != result.end(); it2++)
		{
			cout << it2->first << "     "  << it2->second << endl;
		}
		//double a;
		//cin >> a;
	/*********************************/
	return SUCCESS;
}
