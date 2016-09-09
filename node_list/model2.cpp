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

int ReadData(string fileName)
{
	ifstream in(fileName);
	string line;
	int setId ;
	int maxnum = 0;
	for (setId = 0; getline(in, line); setId++){
		istringstream li(line);
		vector<int> temp;
		int data;
		//cout << line<<endl;
		int i = 0;
		while (li >> data) {
			Set[setId].push_back(data);
			i++;
		}
		if (i > maxnum)
			maxnum = i;
	}
	cout << fileName <<":"<< setId << endl;
	cout << "maxnum:" << maxnum << endl;
	cout << "ReadData Done--------" << endl;
	return maxnum;
}

int CountData(int maxnum)
{
	int total = 0;
	memset(itemCount, 0, 3000 * sizeof(int));
	for (int i = 0; i < Set.size(); i++)
		for (int j = 0; j < Set[i].size(); j++)
			itemCount[Set[i][j]] += 1;
	for (int i = 0; i < maxnum; i++)
		total += itemCount[i];

	cout << "CountData Done--------" << endl;
	return total;
}

bool compare(int a, int b){
	return itemCount[a] > itemCount[b];
}
 
void RearrangeData()
{
	for (int i = 0; i < Set.size(); i++)
		sort(Set[i].begin(), Set[i].end(), compare);
	cout << "RearrangeData Done--------" << endl;
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
	int DLR;
	int LRD;
	map<int, vector<int>> nodeRec;
	node *son;//如果有叶子节点，那么son（第一个孩子节点）一定指向叶子节点，叶子节点的bro指针再指向其他非叶节点。
	node *par;
	node *bro;
}node;
node * root = NULL;
void BuildTree(float fre, int total)
{
	node *nodeCur = NULL;
	node *nodePre = NULL;
	node *nodeLeftBro = NULL;
	
	root = (node *)malloc(sizeof(node));
	root->bro = NULL;
	root->son = NULL;
	root->par = NULL;
	root->leaf = 0;
	
	for (auto elem : Set){
		nodePre = root;
		nodeCur = NULL;
		for (int i = 0; i < elem.second.size(); i++){//对每一个set里的每一个item建节点。
			int infrequent;
			if (itemCount[elem.second[i]] < fre * total)
				infrequent = 1;
			else
				infrequent = 0;
			if (infrequent == 0){//频繁项
				if (nodeCur == NULL){//当前路径到头了
					nodeCur = (node *)malloc(sizeof(node));
					nodeCur->bro = NULL;
					nodeCur->leaf = 0;
					nodeCur->par = nodePre;
					nodeCur->son = NULL;
					cout <<"new data("<< elem.second[i] << ":" << elem.first << endl;
					nodeCur->nodeRec[elem.second[i]].push_back(elem.first);
					nodePre->son = nodeCur;
					nodePre = nodeCur;
				}
				else{//当前路径没到头,遍历整层
					for (; nodeCur != NULL && nodeCur->nodeRec.find(elem.second[i]) != nodeCur->nodeRec.end(); nodeLeftBro = nodeCur, nodeCur = nodeCur->bro);
					if (nodeCur == NULL){//整层都没有，新建
						nodeCur = (node *)malloc(sizeof(node));
						nodeCur->bro = NULL;
						nodeCur->leaf = 0;
						nodeCur->par = nodePre;
						nodeCur->son = NULL; 
						cout << "new data(" << elem.second[i] << ":" << elem.first << endl;
						nodeCur->nodeRec[elem.second[i]].push_back(elem.first);
						nodeLeftBro->bro = nodeCur;
					}
					else{//有，插入
						nodeCur->nodeRec[elem.second[i]].push_back(elem.first);
					}
					nodePre = nodeCur;
				}
				nodeCur = nodePre->son;
			}
			else{//非频繁项
				if (nodeCur != NULL && nodeCur->leaf == 1){//当前路径已经建有叶子结点
					nodeCur->nodeRec[elem.second[i]].push_back(elem.first);
				}
				else{//当前路径到头了或者当前路径没有叶子节点
					if (nodeCur == NULL){//当前路径到头了
						nodeCur = (node *)malloc(sizeof(node));
						nodeCur->bro = NULL;
						nodeCur->leaf = 1;
						cout << "new data(" << elem.second[i] << ":" << elem.first << endl;
						nodeCur->nodeRec[elem.second[i]].push_back(elem.first);
						nodeCur->par = nodePre;
						nodeCur->son = NULL;
						nodePre->son = nodeCur;
					}
					else{//当前路径没有叶子节点
						nodeLeftBro = nodeCur;
						nodeCur = (node *)malloc(sizeof(node));
						nodeCur->bro = NULL;
						nodeCur->leaf = 1;
						cout << "new data(" << elem.second[i] << ":" << elem.first << endl;
						nodeCur->nodeRec[elem.second[i]].push_back(elem.first);
						nodeCur->par = nodePre;
						nodeCur->son = NULL;
						nodePre->son = nodeCur;
						nodeCur->bro = nodeLeftBro;
					}
				}
			}	
		}
	}
	cout << "BuildTree Done--------" << endl;
}

int DLR(int index, node *nodePre, node *nodeCur){
	if (nodeCur != NULL && nodeCur->leaf == 1){
		nodeCur->DLR == index;
		//DEBUG
		cout << index << ":";
		for (auto elem : nodeCur->nodeRec){
			cout << "(" << elem.first << ")";
		}
		cout << endl;
		//DEBUG
		DLR(index + 1, nodePre, nodeCur->bro);
	}
	else{
		if (nodeCur != NULL && nodeCur->leaf == 0){
			nodeCur->DLR = index;
			//DEBUG
			cout << index << ":";
			for (auto elem : nodeCur->nodeRec){
				cout << "(" << elem.first << ")";
			}
			cout << endl;
			//DEBUG
			if (nodeCur->son != NULL)
				DLR(index + 1, nodeCur,nodeCur->son);
		}
		else{
			if (nodeCur == NULL){
				if (nodePre == root)
					return 0;
				else{
					DLR(index, nodePre->par, nodePre->bro);
				}
			}
		}
	}
	
}

int LRD(int index, node *nodePre, node *nodeCur){
	int temp = 0;
	if (nodeCur != NULL && nodeCur->leaf == 1){
		nodeCur->LRD = index;
		//DEBUG
		cout << index << ":";
		for (auto elem : nodeCur->nodeRec){
			cout << "(" << elem.first << ")";
		}
		cout << endl;
		//DEBUG
		LRD(index + 1, nodePre, nodeCur->bro);
	}
	else{
		if (nodeCur != NULL && nodeCur->leaf == 0){
			if (nodeCur->son != NULL)
				LRD(index, nodeCur, nodeCur->son);
		}
		else{
			if (nodeCur == NULL){
				nodePre->LRD == index;
				//DEBUG
				cout << index << ":";
				for (auto elem : nodePre->nodeRec){
					cout << "(" << elem.first << ")";
				}
				cout << endl;
				//DEBUG
				if (nodePre == root)
					return 0;
				else
					LRD(index + 1, nodePre->par, nodePre->bro);
			}
		}
	}
}
int main()
{
	string fileName[] = { ".//data//mushroom.dat",".//data//accidents.dat",".//data//T10I4D100K.dat" };
	float fre = 0.25;
	int total;
	for (int i = 0; i < 3;i++)
		ReadData(fileName[i]);
		total = CountData();
	//RearrangeData();
	//BuildTree(fre, total);
	//DLR(0, NULL, root);
	//LRD(0, NULL, root);
	//GenNodeList();
	//Query();
	system("pause");
	return 0;
}