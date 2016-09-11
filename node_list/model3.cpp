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

void DLR(node * cur){
	ResetIndex();
	Pre(cur);
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

map<int, int> arec;
map<tuple<int, int>, int> aset2;
map<tuple<int, int, int, int>, int> aset4;
map<tuple<int, int, int, int, int, int>, int> aset6;
tuple<int, int> cur2;
tuple<int, int, int, int> cur4;
tuple<int, int, int, int, int, int> cur6;
short f2[1000][2];
short f4[1000][4];
short f6[1000][6];
short u2[1000][2];
short u4[1000][4];
short u6[1000][6];
short r2[1000][2];
short r4[1000][4];
short r6[1000][6];

void Gen2(int count, int* ori, int left, int right, short*pos,){
	//Gen2:生成两个不重复的随机数，存入pos指向的数组中
	//count:已经成功生成的随机数
	//left：左边界，可以取到
	//right：右边界，取不到
	//pos:存放2个随机数的数组起始位置
	if (count == 2){
		int i = 0;
		for (auto elem : arec){
			*(pos + i) = *(ori + elem.first);
			i++;
		}
		arec.clear();
		cur2 = make_tuple(*pos, *(pos + 1));
		return;
	}
	int cur = rand() % (right - left) + left;
	if (arec.find(cur) == arec.end()){
		count++;
		arec[cur] = 1;
	}
	Gen2(count, ori, left, right, pos);
	return;
}
void Gen4(int count, int *ori, int left, int right, short*pos){
	//Gen4:生成四个不重复的随机数，存入pos指向的数组中
	//count:已经成功生成的随机数
	//left：左边界，可以取到
	//right：右边界，取不到
	//pos:存放4个随机数的数组起始位置
	if (count == 4){
		int i = 0;
		for (auto elem : arec){
			*(pos + i) = *(ori + elem.first);
			i++;
		}
		arec.clear();
		cur4 = make_tuple(*pos, *(pos + 1), *(pos + 2), *(pos + 3));
		return;
	}
	int cur = rand() % (right - left) + left;
	if (arec.find(cur) == arec.end()){
		count++;
		arec[cur] = 1;
	}
	Gen4(count, left, right, pos);
	return;
}
void Gen6(int count, int *ori, int left, int right, short*pos){
	//Gen6:生成六个不重复的随机数，存入pos指向的数组中
	//count:已经成功生成的随机数
	//left：左边界，可以取到
	//right：右边界，取不到
	//pos:存放2个随机数的数组起始位置
	if (count == 6){
		int i = 0;
		for (auto elem : arec){
			*(pos + i) = *(ori + elem.first);
			i++;
		}
		arec.clear();
		cur6 = make_tuple(*pos, *(pos + 1), *(pos + 2), *(pos + 3), *(pos + 4), *(pos + 5));
		return;
	}
	int cur = rand() % (right - left) + left;
	if (arec.find(cur) == arec.end()){
		count++;
		arec[cur] = 1;
	}
	Gen6(count, left, right, pos);
	return;
}

void GenFre(int n,int *ori, int left, int right){
	//GenFre：从频繁项中生成n个不相同的数串，数串长度分别为2，4，6
	//left：左边界，可以取到
	//right:右边界，取不到
	for (int i = 0; i < n;){
		Gen2(0,ori, left, right, f2[i]);
		if (aset2.find(cur2) == aset2.end()){
			i++;
			aset2[cur2] = 1;
		}
	}
	aset2.clear();
	for (int i = 0; i < n;){
		Gen4(0, ori, left, right, f4[i]);
		if (aset4.find(cur4) == aset4.end()){
			i++;
			aset4[cur4] = 1;
		}
	}
	aset4.clear();
	for (int i = 0; i < n;){
		Gen6(0, ori, left, right, f6[i]);
		if (aset6.find(cur6) == aset6.end()){
			i++;
			aset6[cur6] = 1;
		}
	}
	aset6.clear();
	return;
}
void GenUnfre(int n, int *ori, int left, int right){
	for (int i = 0; i < n;){
		Gen2(0, ori, left, right, u2[i]);
		if (aset2.find(cur2) == aset2.end()){
			i++;
			aset2[cur2] = 1;
		}
	}
	aset2.clear();
	for (int i = 0; i < n;){
		Gen4(0, ori, left, right, u4[i]);
		if (aset4.find(cur4) == aset4.end()){
			i++;
			aset4[cur4] = 1;
		}
	}
	aset4.clear();
	for (int i = 0; i < n;){
		Gen6(0, ori, left, right, u6[i]);
		if (aset6.find(cur6) == aset6.end()){
			i++;
			aset6[cur6] = 1;
		}
	}
	aset6.clear();
	return;
}
void GenRan(int n, int *ori, int left, int right){
	for (int i = 0; i < n;){
		Gen2(0, ori, left, right, r2[i]);
		if (aset2.find(cur2) == aset2.end()){
			i++;
			aset2[cur2] = 1;
		}
	}
	aset2.clear();
	for (int i = 0; i < n;){
		Gen4(0, ori, left, right, r4[i]);
		if (aset4.find(cur4) == aset4.end()){
			i++;
			aset4[cur4] = 1;
		}
	}
	aset4.clear();
	for (int i = 0; i < n;){
		Gen6(0, ori, left, right, r6[i]);
		if (aset6.find(cur6) == aset6.end()){
			i++;
			aset6[cur6] = 1;
		}
	}
	aset6.clear();
	return;
}
void CheckGen()
{
	for (int ii = 0; ii < 1000; ii++){
		f[ii] = ii;
	}
	//GenFre(0, 1000, 0, 1000);
	srand((unsigned)time(NULL));
	GenFre(1000, 0, 1000);

	for (int i = 0; i < 1000; i++){

		for (int j = 0; j < 2; j++){
			cout << f2[i][j] << " ";
		}
		cout << endl;

	}
	for (int i = 0; i < 1000; i++){

		for (int j = 0; j < 4; j++){
			cout << f4[i][j] << " ";
		}
		cout << endl;

	}
	for (int i = 0; i < 1000; i++){

		for (int j = 0; j < 6; j++){
			cout << f6[i][j] << " ";
		}
		cout << endl;

	}
	return;

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
	SepFreUnfre(baseFre);
	cout << "fi:" << fi << ",ui" << ui << ",ri" << ri << endl;
	GenFre(1000,f, 0, fi);
	GenUnfre(1000,u, 0, ui);
	GenRan(1000,r, 0, ri);
	CheckGen();
	system("pause");
	return 0;
}