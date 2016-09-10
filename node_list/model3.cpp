#include<vector>
#include<map>
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<time.h> 
#include<stdlib.h>
#include<algorithm>
using namespace std;

map<int, vector<int>> setData;

float ReadFile(string path,float fre)
{
	ifstream in(path);
	string line;
	int setId;
	int maxnum = 0;
	for (setId = 0; getline(in, line); setId++){
		istringstream li(line);
		vector<int> temp;
		int data;
		while (li >> data) {
			setData[setId].push_back(data);
		}

	}
	return setId*fre;
}

int itemCount[1000] = { 0 };
void CountData()
{
	int total = 0;
	memset(itemCount, 0, 1000 * sizeof(int));
	for (int i = 0; i < setData.size(); i++)
		for (int j = 0; j < setData[i].size(); j++)
			itemCount[setData[i][j]] += 1;
	
	return;
}

bool compare(int a, int b){
	return ((itemCount[a] > itemCount[b]));
}

void RearrangeData()
{
	for (int i = 0; i < setData.size(); i++)
		sort(setData[i].begin(), setData[i].end(), compare);
	
	/*
	for (int i = 0; i < setData.size(); i++){
		cout << "after:";
		for (int j = 0; j < setData[i].size(); j++)
			cout << setData[i][j] << "(" << itemCount[setData[i][j]] << ") ";
		cout << endl;
	}
	*/
	
	
}

class node
{
public:
	bool leaf;
	int DLR, LRD;
	map<int, vector<int>> nodeRec;
	vector<node *> son;
	node(){
		leaf = 0;
	}
};

node* BuildTree(float fre)
{
	node *root = new node;
	node *nodeCur = NULL;
	node *nodePre = NULL;
	for (auto aset : setData){
		nodePre = root;
		for (int i = 0; i < aset.second.size(); i++){//将节点加入树中
			if (nodePre->son.empty() == true){//当前路径到头了,新建节点并插入
				nodeCur = new node;
				nodeCur->nodeRec[aset.second[i]].push_back(aset.first);
				nodePre->son.push_back(nodeCur);
				if (itemCount[aset.second[i]] < fre){
					nodeCur->leaf = 1;
				}
				else{
					nodePre = nodeCur;
				}
			}
			else{//当前路径往下还有节点
				int find = 0;
				vector<node *>::iterator vi;
				for (vi = nodePre->son.begin(); vi != nodePre->son.end();++vi){
					if ((**vi).nodeRec.find(aset.second[i])!=(**vi).nodeRec.end()){//id相同的节点已经插入树中，不需新建节点，只需在原有节点上添上set id
						(**vi).nodeRec[aset.second[i]].push_back(aset.first);
						find = 1;
						if (itemCount[aset.second[i]] >= fre){
							nodePre = (*vi);
						}
						break;
					}
				}
				if (find == 0){//此id节点还没有插入到树中,需要判断此id是不是非频繁的。
					int findLeaf = 0;

					if (itemCount[aset.second[i]] < fre){//如果当前id是非频繁项，要找当前的son中有没有叶子节点，没有就新建，有就直接添加id
						vector<node *>::iterator vii;
						for (vii = nodePre->son.begin(); vii != nodePre->son.end(); ++vii){
							if ((**vii).leaf == 1){
								findLeaf = 1;
								break;
							}
						}
						if (findLeaf == 1){//已经建好叶子节点，直接在叶子节点里添加新的纪录
							(**vii).nodeRec[aset.second[i]].push_back(aset.first);
						}
						else{//没有叶子节点，需要新建叶子节点
							nodeCur = new node;
							nodeCur->leaf = 1;
							nodeCur->nodeRec[aset.second[i]].push_back(aset.first);
							nodePre->son.push_back(nodeCur);
						}

					}
					else{//当前id是频繁项，新建节点插入树中
						nodeCur = new node;
						nodeCur->nodeRec[aset.second[i]].push_back(aset.first);
						nodePre->son.push_back(nodeCur);
						nodePre = nodeCur;
					}
				}
			}
		}
	}
	return root;
}

int index = 0;
void ResetIndex(){
	index = 0;
}
void Pre(node *cur)
{
	if (cur == NULL){
		return;
	}
	cur->DLR = index;
	index = index + 1;
	vector<node *>::iterator vi;
	for (vi = cur->son.begin(); vi != cur->son.end(); ++vi){
		Pre(*vi);
	}
	return;
}
void DLR(node * cur){
	ResetIndex();
	Pre(cur);
}
void Post(node *cur)
{
	if (cur->son.empty() == true){
		cur->LRD = index;
		index = index + 1;
		return;
	}
	vector<node *>::iterator vi;
	for (vi = cur->son.begin(); vi != cur->son.end(); ++vi){
		Post(*vi);
	}
	cur->LRD = index;
	index = index + 1;
	return;
}
void LRD(node * cur){
	ResetIndex();
	Post(cur);
}
void CheckTree(node * cur){
	if (cur == NULL){
		return;
	}
	cout << "(" << cur->DLR <<","<< cur->LRD << ")";
	if (cur->leaf == 0){
		cout << "node";
	}
	else{
		cout << "leaf";
	}
	for (auto elem : cur->nodeRec){
		cout << elem.first << ":";
		for (int i = 0; i < elem.second.size(); i++){
			cout << elem.second[i] << " ";
		}
		cout << endl;
	}
	vector<node *>::iterator vi;
	for (vi = cur->son.begin(); vi != cur->son.end(); ++vi){
		CheckTree(*vi);
	}
	return;
}

map< int, map< pair<int, int>, vector<int> > > nodeList;

void GenNodeList(node *cur){
	if (cur == NULL){
		return;
	}
	
	for (auto arec: cur->nodeRec){//map<int, vector<int>> nodeRec;
		nodeList[arec.first][make_pair(cur->DLR, cur->LRD)] = arec.second;
	}
	vector<node *>::iterator vi;
	for (vi = cur->son.begin(); vi != cur->son.end(); ++vi){
		GenNodeList(*vi);
	}
	return;
}
void CheckNodeList(){
	//map< int, map< pair<int, int>, vector<int> > > nodeList;
	map< int, map< pair<int, int>, vector<int> > >::iterator mi;
	for (mi = nodeList.begin(); mi != nodeList.end(); ++mi){
		cout << (*mi).first << "->{";
		map< pair<int, int>, vector<int> >::iterator mii;
		for (mii = (*mi).second.begin(); mii != (*mi).second.end(); ++mii){
			cout << "(<" << (*mii).first.first << "," << (*mii).first.second << ">:{";
			for (auto setId : (*mii).second){
				cout << setId << ",";
			}
			cout << "})\n";
		}
		cout << "}" << endl;
	}
}

int f[1000];
int u[1000];
int r[1000];
int fi;
int ui;
int ri;
bool CompareOnNodeList(int a, int b)
{
	return nodeList[a].size() < nodeList[b].size();
}
void SepFreUnfre(float baseFre)
{
	memset(f, 0, sizeof(f));
	memset(u, 0, sizeof(u));
	memset(r, 0, sizeof(r));
	fi = 0;
	ui = 0;
	ri = 0;
	for (int i = 0; i < 1000; i++){
		if (itemCount[i] != 0){
			r[ri] = itemCount[i];
			ri++;
			if (itemCount[i] >= baseFre){
				f[fi] = itemCount[i];
				fi++;
			}
			else{
				u[ui] = itemCount[i];
				ui++;
			}
		}	
	}
}

short f2[1000][2];
short f4[1000][4];
short f6[1000][6];
void GenFre(int num){
	for (int producted = 0; producted < num; producted++){
		for (int j = 0; j < 2; j++){
			srand((unsigned)time(NULL));
			f2[producted][j] = f[rand() % fi];
		}
		for (int j = 0; j < 4; j++){
			srand((unsigned)time(NULL));
			f4[producted][j] = f[rand() % fi];
		}
		for (int j = 0; j < 6; j++){
			srand((unsigned)time(NULL));
			f6[producted][j] = f[rand() % fi];
		}
	}
	for (int producted = 0; producted < num; producted++){
		sort(f2[producted], f2[producted] + 2, CompareOnNodeList);
		sort(f4[producted], f4[producted] + 4, CompareOnNodeList);
		sort(f6[producted], f6[producted] + 6, CompareOnNodeList);
	}
		
}
short u2[1000][2];
short u4[1000][4];
short u6[1000][6];
void GenUnfre(int num){
	for (int producted = 0; producted < num; producted++){
		for (int j = 0; j < 2; j++){
			srand((unsigned)time(NULL));
			u2[producted][j] = u[rand() % ui];
		}
		for (int j = 0; j < 4; j++){
			srand((unsigned)time(NULL));
			u4[producted][j] = u[rand() % ui];
		}
		for (int j = 0; j < 6; j++){
			srand((unsigned)time(NULL));
			u6[producted][j] = u[rand() % ui];
		}
	}
	for (int producted = 0; producted < num; producted++){
		sort(u2[producted], u2[producted] + 2, CompareOnNodeList);
		sort(u4[producted], u4[producted] + 4, CompareOnNodeList);
		sort(u6[producted], u6[producted] + 6, CompareOnNodeList);
	}
}
short r2[1000][2];
short r4[1000][4];
short r6[1000][6];
void GenRan(int num){
	for (int producted = 0; producted < num; producted++){
		vector<int> temp;
		for (int j = 0; j < 2; ){
			srand((unsigned)time(NULL));
			int t = rand() % ri;
			if (find(temp.begin(), temp.end(), t) == temp.end()){
				r2[producted][j] = r[t];
				temp.push_back(t);
				j++;
			}	
		}
		temp.clear();
		for (int j = 0; j < 4; j++){
			srand((unsigned)time(NULL));
			r4[producted][j] = r[rand() % ri];
		}
		for (int j = 0; j < 6; j++){
			srand((unsigned)time(NULL));
			r6[producted][j] = r[rand() % ri];
		}
	}
	for (int producted = 0; producted < num; producted++){
		sort(r2[producted], r2[producted] + 2, CompareOnNodeList);
		sort(r4[producted], r4[producted] + 4, CompareOnNodeList);
		sort(r6[producted], r6[producted] + 6, CompareOnNodeList);
	}
}
int main(){
	
	map<string, float> data_fre = { { ".//data//mushroom.dat", 0.25 }, { ".//data//accidents.dat", 0.5 }, { ".//data//T10I4D100K.dat", 0.005 } };
	string path = ".//data//mushroom.dat";
	float fre = 0.25;
	float baseFre;
	
	baseFre = ReadFile(path, fre);
	cout << "频繁项最低支持度：" << baseFre << endl;
	CountData();
	cout << "CountData Done------------" << endl;
	RearrangeData();
	cout << "RearrangeData Done--------" << endl;
	//node * root = BuildTree(baseFre);
	//cout << "BuildTree Done------------" << endl;
	//DLR(root); 
	//LRD(root);

	//GenNodeList(root);
	//cout << "GenNodeList Done----------" << endl;
	GenFre(baseFre);
	GenUnfre(baseFre);
	GenRan(baseFre);
	system("pause");
	return 0;
}