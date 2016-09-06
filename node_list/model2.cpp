#include<vector>
#include<map>
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<stdlib.h>
#include<algorithm>
using namespace std;

map<int, vector<int>> Set = {};
int itemCount[3000] = { 0 };

void ReadData(string fileName)
{
	ifstream in(fileName);
	string line;
	int i = 0;
	for (int setId = 0; getline(in, line) && i<7; setId++){
		istringstream li(line);
		vector<int> temp;
		int data;
		cout << line<<endl;
		while (li >> data) {
			Set[setId].push_back(data);
		}
		i++;
	}
	cout << "ReadData Done--------" << endl;
}

int CountData()
{
	int total = 0;
	for (int i = 0; i < Set.size(); i++)
		for (int j = 0; j < Set[i].size(); j++)
			itemCount[Set[i][j]] += 1;
	for (int i = 0; i < 3000; i++)
		total += itemCount[i];	
	return total;
}

bool compare(int a, int b){
	return itemCount[a] > itemCount[b];
}

void RearrangeData()
{
	for (int i = 0; i < Set.size(); i++)
		sort(Set[i].begin(), Set[i].end(), compare);
	/*
	for (int i = 0; i < Set.size(); i++){
	cout << "after:";
	for (int j = 0; j < Set[i].size(); j++)
	cout << Set[i][j] << "(" << itemCount[Set[i][j]]<<") ";
	cout << endl;
	}
	*/
}
typedef struct node{
	bool leaf;
	map<int, vector<int>> nodeRec;
	node *child;
	node *par;
	node *bro;
}node;
node * root = NULL;
void BuildTree(float fre, int total)
{
	node *nodeCur = NULL;
	node *nodePre = NULL;
	node *nodeLeftBro = NULL;
	for (auto elem : Set){
		for (int i = 0; i < elem.second.size(); i++){//对每一个set里的每一个item建节点。
			nodeCur = (node *)malloc(sizeof(node));
			nodeCur->bro = NULL;
			nodeCur->child = NULL;
			nodeCur->par = NULL;
			//TO DO
		}
		
	}
}

int main()
{
	string fileName = ".//data//mushroom.dat";
	float fre = 0.25;
	int total;
	ReadData(fileName);
	total = CountData();
	RearrangeData();
	BuildTree(fre, total);
	//DLR();
	//LRD();
	//GenNodeList();
	//Query();
	system("pause");
	return 0;
}