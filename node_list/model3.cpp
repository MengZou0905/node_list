#include<vector>
#include<map>
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<time.h> 
#include<stdlib.h>
#include<tuple>
#include<algorithm>
#include<time.h>
using namespace std;

map<int, vector<int>> setData;
#define TESTNUM 250
float ReadFile(string path,float fre)
{
	ifstream in(path);
	string line;
	int setId;
	int maxnum = 0;
	for (setId = 0; getline(in, line); setId++){
		//cout << setId << ":";
		istringstream li(line);
		vector<int> temp;
		int data;
		while (li >> data) {
			setData[setId].push_back(data);
			//cout << data << " ";
		}
		//cout << endl;
	}
	return setId*fre;
}
map<int, vector<int>> invertedList;
void BuildInvertedList(){
	for (auto elem : setData){
		vector<int>::iterator vi;
		for (vi = elem.second.begin(); vi != elem.second.end(); vi++){
			invertedList[(*vi)].push_back(elem.first);
		}
	}
}
void CheckInvertedList(){
	float evaLen = 0;
	for (auto elem : invertedList){
		evaLen += elem.second.size();
	}
	evaLen = (float)evaLen/invertedList.size();
	cout << "倒排表平均长度：" << evaLen << endl;
	/*
	for (auto elem : invertedList){
		cout << elem.first << ":";
		vector<int>::iterator vi;
		for (vi = elem.second.begin(); vi != elem.second.end(); vi++){
			cout<<(*vi)<<" ";
		}
		cout << endl;
	}
	*/
	
}


int itemCount[1000] = { 0 };
float baseFre;
void CountData()
{
	//map<int, vector<int>> setData;
	int total = 0;
	memset(itemCount, 0, 1000 * sizeof(int));
	for (auto i: setData){
		for (auto j:i.second)
			itemCount[j] += 1;
	}
		
	
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

node* BuildTree()
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
				if (itemCount[aset.second[i]] < baseFre){
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
						if (itemCount[aset.second[i]] >= baseFre){
							nodePre = (*vi);
						}
						break;
					}
				}
				if (find == 0){//此id节点还没有插入到树中,需要判断此id是不是非频繁的。
					int findLeaf = 0;

					if (itemCount[aset.second[i]] < baseFre){//如果当前id是非频繁项，要找当前的son中有没有叶子节点，没有就新建，有就直接添加id
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
	float evaLen = 0;
	float evaNodeLen = 0;
	for (mi = nodeList.begin(); mi != nodeList.end(); ++mi){
		evaLen += (*mi).second.size();
		map< pair<int, int>, vector<int> >::iterator mii;
		for (mii = (*mi).second.begin(); mii != (*mi).second.end(); ++mii){
			evaNodeLen += (*mii).second.size();
		}
	}
	evaNodeLen = (float)evaNodeLen / evaLen;
	evaLen = (float)evaLen / nodeList.size();
	cout << "nodeList平均包含nodes：" << evaLen << endl;
	cout << "每个node平均包含records:" << evaNodeLen << endl;
	/*
	
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
	*/
}


int f[1000];
int u[1000];
int r[1000];
int fi;
int ui;
int ri;

void SepFreUnfre(float baseFre)
{
	memset(f, 0, sizeof(f));
	memset(u, 0, sizeof(u));
	memset(r, 0, sizeof(r));
	fi = 0;
	ui = 0;
	ri = 0;
	for (int i = 1; i < 1000; i++){
		if (itemCount[i] != 0){
			r[ri] = i;
			ri++;
			if (itemCount[i] >= baseFre){
				f[fi] = i;
				fi++;
			}
			else{
				u[ui] = i;
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
short f2[TESTNUM][2];
short f4[TESTNUM][4];
short f6[TESTNUM][6];
short u2[TESTNUM][2];
short u4[TESTNUM][4];
short u6[TESTNUM][6];
short r2[TESTNUM][2];
short r4[TESTNUM][4];
short r6[TESTNUM][6];

bool CompareOnNodeList(int a, int b)
{
	return (nodeList[a].size() < nodeList[b].size());
}
void Gen2(int count, int* ori, int left, int right, short*pos){
	//Gen2:生成两个不重复的随机数，存入pos指向的数组中
	//count:已经成功生成的随机数
	//left：左边界，可以取到
	//right：右边界，取不到
	//pos:存放2个随机数的数组起始位置
	if (count == 2){
		int i = 0;
		//cout << "gen2:";
		for (auto elem : arec){
			*(pos + i) = *(ori + elem.first);
			//cout << *(pos + i) << ",";
			i++;
		}
		//按照node-list由短到长排列
		//sort(pos, pos+2,CompareOnNodeList);
		//按照support由多到少排列
		sort(pos, pos + 2, compare);
		//cout << endl;
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
		//按照node-list由短到长排列
		//sort(pos, pos+4,CompareOnNodeList);
		//按照support由多到少排列
		sort(pos, pos + 4, compare);
		arec.clear();
		cur4 = make_tuple(*pos, *(pos + 1), *(pos + 2), *(pos + 3));
		return;
	}
	int cur = rand() % (right - left) + left;
	if (arec.find(cur) == arec.end()){
		count++;
		arec[cur] = 1;
	}
	Gen4(count, ori,left, right, pos);
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
		//按照node-list由短到长排列
		//sort(pos, pos+6,CompareOnNodeList);
		//按照support由多到少排列
		sort(pos, pos + 6, compare);
		arec.clear();
		cur6 = make_tuple(*pos, *(pos + 1), *(pos + 2), *(pos + 3), *(pos + 4), *(pos + 5));
		return;
	}
	int cur = rand() % (right - left) + left;
	if (arec.find(cur) == arec.end()){
		count++;
		arec[cur] = 1;
	}
	Gen6(count, ori,left, right, pos);
	return;
}

void GenFre(int n,int *ori, int left, int right){
	//GenFre：从频繁项中生成n个不相同的数串，数串长度分别为2，4，6
	//left：左边界，可以取到
	//right:右边界，取不到
	
	for (int i = 0; i < n;){
		Gen2(0,ori, left, right, f2[i]);
		if (aset2.find(cur2) == aset2.end()){
			//cout <<"Fre:" <<i << endl;
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
			//cout << "GenUnfre:" << i << endl;
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
		Gen2(0, ori, left, right, r2[i]);//已经按标准（node-list或者support）排好序
		vector<int> tempv;
		for (int k = 0; k < 2; k++){
			if (itemCount[r2[i][k]] >= baseFre){
				tempv.push_back(r2[i][k]);
			}
		}
		for (int k = 0; k < 2; k++){
			if (itemCount[r2[i][k]] < baseFre){
				tempv.push_back(r2[i][k]);
			}
		}
		int kk = 0;
		for (vector<int>::iterator vi = tempv.begin(); vi != tempv.end(); ++vi,++kk){
			r2[i][kk] = (*vi);
		}
		cur2 = make_tuple(r2[i][0],r2[i][1]);
		if (aset2.find(cur2) == aset2.end()){
			//将fre项排在前面，unfre项排在后面，两部分都是support多的在前
			i++;
			aset2[cur2] = 1;
		}
	}
	
	aset2.clear();
	for (int i = 0; i < n;){
		Gen4(0, ori, left, right, r4[i]);
		vector<int> tempv;
		for (int k = 0; k < 4; k++){
			if (itemCount[r4[i][k]] >= baseFre){
				tempv.push_back(r4[i][k]);
			}
		}
		for (int k = 0; k < 4; k++){
			if (itemCount[r4[i][k]] < baseFre){
				tempv.push_back(r4[i][k]);
			}
		}
		int kk = 0;
		for (vector<int>::iterator vi = tempv.begin(); vi != tempv.end(); ++vi, ++kk){
			r4[i][kk] = (*vi);
		}
		cur4 = make_tuple(r4[i][0], r4[i][1], r4[i][2], r4[i][3]);
		if (aset4.find(cur4) == aset4.end()){
			i++;
			aset4[cur4] = 1;
		}
	}
	aset4.clear();
	for (int i = 0; i < n;){
		Gen6(0, ori, left, right, r6[i]);
		vector<int> tempv;
		for (int k = 0; k < 6; k++){
			if (itemCount[r6[i][k]] >= baseFre){
				tempv.push_back(r6[i][k]);
			}
		}
		for (int k = 0; k < 6; k++){
			if (itemCount[r6[i][k]] < baseFre){
				tempv.push_back(r6[i][k]);
			}
		}
		int kk = 0;
		for (vector<int>::iterator vi = tempv.begin(); vi != tempv.end(); ++vi, ++kk){
			r6[i][kk] = (*vi);
		}
		cur6 = make_tuple(r6[i][0], r6[i][1], r6[i][2], r6[i][3], r6[i][4], r6[i][5]);
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
	for (int i = 0; i < TESTNUM; i++){

		for (int j = 0; j < 4; j++){
			cout << r4[i][j] << ":" << itemCount[r4[i][j]] << "(" << nodeList[r4[i][j]].size() << ") ";
		}
		cout << endl;

	}
	
	return;

}
typedef map< pair<int, int>, vector<int> > anl;
anl Combine2Fre(anl a, anl b)
{
	anl result;
	anl::iterator ai, bi;
	//for (int i = 0; i<2 && result.empty() == true; i++){//适用于查询项按照node-list由短到长排列的情况，防止查询项“子父”顺序导致查询结果为空
		bi = b.begin();
		for (ai = a.begin(); ai != a.end();){
			for (; bi != b.end() && (*ai).first.first >(*bi).first.first; ++bi);
			//int dlra = (*ai).first.first;
			//int lrda = (*ai).first.second;
			//int dlrb = (*bi).first.first;
			//int lrdb = (*bi).first.second;
			//vector<int> seta = (*ai).second;
			//vector<int> setb = (*bi).second;
			if (bi != b.end()){
				if ((*ai).first.second > (*bi).first.second){
					result[(*bi).first] = (*bi).second;
					++bi;
				}
				else{
					++ai;
				}
			}
			else{
				break;
			}
		}
		//swap(a, b);//适用于查询项按照node-list由短到长排列的情况，防止查询项“子父”顺序导致查询结果为空
	//}//适用于查询项按照node-list由短到长排列的情况，防止查询项“子父”顺序导致查询结果为空
	
	
	return result;
}
vector<int> Intersection(vector<int> a, vector<int> b)
{
	vector<int> result;
	vector<int>::iterator ai, bi;
	bi = b.begin();
	for (ai = a.begin(); ai != a.end();){
		for (; bi != b.end() && (*bi) < (*ai); ++bi);
		if (bi != b.end()){
			if ((*bi) == (*ai)){
				result.push_back((*bi));
				++ai;
				++bi;
			}
			else{
				if ((*bi) > (*ai)){
					++ai;
				}
			}
		}
		else{
			break;
		}
	}
	return result;
}
anl Combine2Unfre(anl a, anl b){
	anl result;
	anl::iterator ai, bi;
	vector<int> inter;
	bi = b.begin();
	for (ai = a.begin(); ai != a.end();){
		for (; bi != b.end() && (*ai).first.first < (*bi).first.first; ++ bi);
		if (bi != b.end()){
			if ((*ai).first.first == (*bi).first.first){//DLR相同
				//交操作
				inter = Intersection((*ai).second, (*bi).second);
				result[(*ai).first] = inter;
				++bi;
				++ai;
			}
			else{
				if ((*ai).first.first > (*bi).first.first){
					++ai;
				}
			}
		}
		else{
			break;
		}
	}
	return result;
}
anl QueryFre(short *ori, int size){
	anl a,b;
	//int have_son_relation = 1;

		short ida = *(ori);
		a = nodeList[ida];
		//for (int j = 1; j < size && have_son_relation; j++){
		for (int j = 1; j < size && a.empty()!= true; j++){
			short idb = *(ori + j);
			b = nodeList[idb];
			a = Combine2Fre(a, b);
			/*
			if (a.empty() == true){
				have_son_relation = 0;
				break;
			}
			*/
		}
	//}
	return a;
}
anl QueryUnfre(short *ori, int size){
	anl a, b;
	//for (int i = 0; i < TESTNUM && have_son_relation; i++){
	short ida = *(ori);
	a = nodeList[ida];
	for (int j = 1; j < size && a.empty() != true; j++){
		short idb = *(ori + j);
		b = nodeList[idb];
		a = Combine2Unfre(a, b);
	}
	//}
	return a;
}
int FindSep(short *ori, int size){
	int sep ;
	for (sep = 0; sep < size; sep++){
		if (itemCount[*(ori + sep)] < baseFre){
			break;
		}
	}
	//sep == size：全是frequent id
	//sep == 0：全是unfre id
	//else,数组里有fre，也有unfre
	return sep;
}
anl QueryRan(short *fre, int fsize,short *unfre,int usize){
	anl a, b;
	
	short ida = *(fre);
	a = nodeList[ida];
	for (int j = 1; j < fsize && a.empty() != true; j++){
		short idb = *(fre + j);
		b = nodeList[idb];
		a = Combine2Fre(a, b);
	}
	
	if (a.empty() != true){
		short idb = *(unfre);
		b = nodeList[idb];
		a = Combine2Fre(a, b);
		for (int j = 1; j < usize && a.empty() != true; j++){
			idb = *(unfre + j);
			b = nodeList[idb];
			a = Combine2Unfre(a, b);
		}
	}
	
	return a;
}
anl Query(short *ori, int size){
	anl a;
	int sep;

	sep = FindSep(ori, size);
	//cout << "sep:" << sep << endl;
	//cout << "size:" << size << endl;
	if (sep == size){
		//cout << "using QueryFre" << endl;
		a = QueryFre(ori, size);
	}
	else{
		if (sep == 0){
			//cout << "using QueryUnfre" << endl;
			a = QueryUnfre(ori, size);
		}
		else{
			//cout << "QueryRan" << endl;
			a = QueryRan(ori, sep, ori + sep, size - sep);
		}
	}
	return a;
}

vector<int> QueryInvertedList(short *ori, int size){
	vector<int> a, b;
	
	short ida = *(ori);
	a = invertedList[ida];
	for (int j = 1; j < size && a.empty() != true; j++){
		short idb = *(ori + j);
		b = invertedList[idb];
		a = Intersection(a, b);
	}
	return a;

}
void CheckQueryAnswer(anl ans){
	if (ans.empty() == true){
		cout << "no record!" << endl;
		return;
	}
	cout << "answer:" << endl;
	for (auto elem : ans){
		//cout << "<" << elem.first.first << "," << elem.first.second << ">:";
		for (int i = 0; i < elem.second.size(); i++){
			cout << elem.second[i] << " ";
		}
		//cout << endl;
	}
	cout << endl;
}
void PrintFreNode(){
	cout << "fi:" <<fi<< endl;
	for (int i = 0; i < fi; i++){
		cout << f[i] << ",";
	}
	cout << endl;
}
void PrintUnfreNode(){
	cout << "ui:" << ui<< endl;
	for (int i = 0; i < ui; i++){
		cout << u[i] << ",";
	}
	cout << endl;
} 

int main(){
	
	//map<string, float> data_fre = { { ".//data//mushroom.dat", 0.25 }, { ".//data//accidents.dat", 0.5 }, { ".//data//T10I4D100K.dat", 0.005 } };
	string path = ".//data//accidents.dat";
	float fre = 0.5;
	
	//计时声明
	clock_t start, finish;
	double totaltime;

	baseFre = ReadFile(path, fre);
	
	BuildInvertedList();
	//CheckInvertedList();
	
	CountData();
	//cout << "CountData Done------------" << endl;
	RearrangeData();
	//cout << "RearrangeData Done--------" << endl;
	node * root = BuildTree();
	//cout << "BuildTree Done------------" << endl;
	DLR(root); 
	LRD(root);

	GenNodeList(root);
	//cout << "GenNodeList Done----------" << endl;
	//CheckNodeList();

	
	//PrintFreNode();
	//PrintUnfreNode();

	SepFreUnfre(baseFre);
	cout << "----------------------------------------------------------------" << endl;
	cout << "当前数据集: " << path << endl << "frequent creterior: " << fre << endl;
	cout << "当前数据集频繁项最低支持度：" << baseFre << endl;
	cout << "频繁单项数量:" << fi << endl;
	cout << "非频繁单项数量:" << ui << endl;
	cout << "测试集大小:" << TESTNUM << endl;
	
	GenFre(TESTNUM, f, 0, fi);
	GenUnfre(TESTNUM, u, 0, ui);
	GenRan(TESTNUM, r, 0, ri);
	//CheckGen();
	//cout << "GenTestData Done----------" << endl;
	
	cout <<endl<< "倒排测试" ;
	//计时开始
	start = clock();
	for (int i = 0; i < TESTNUM; i++){
		vector<int> ir2 = QueryInvertedList(r2[i], 2);//测试倒排表用时
		//anl nr2 = Query(r2[i], 2);//测试node-list算法用时
	}
	//计时结束
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "\nir2的运行时间为" << totaltime << "秒！" << endl;

	//计时开始
	start = clock();
	for (int i = 0; i < TESTNUM; i++){
		vector<int> ir4 = QueryInvertedList(r4[i], 4);//测试倒排表用时
		//anl nr4 = Query(r4[i], 4);//测试node-list算法用时
	}
	//计时结束
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "ir4的运行时间为" << totaltime << "秒！" << endl;

	//计时开始
	start = clock();
	for (int i = 0; i < TESTNUM; i++){
		vector<int> ir6 = QueryInvertedList(r6[i], 6);//测试倒排表用时
		//anl nr6 = Query(r6[i], 6);//测试node-list算法用时
	}
	//计时结束
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "ir6的运行时间为" << totaltime << "秒！" << endl;

	//计时开始
	start = clock();
	for (int i = 0; i < TESTNUM; i++){
		vector<int> if2 = QueryInvertedList(f2[i], 2);//测试倒排表用时
		//anl nf2 = QueryFre(f2[i], 2);//测试node-list算法用时
	}
	//计时结束
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "\nif2的运行时间为" << totaltime << "秒！" << endl;

	//计时开始
	start = clock();
	for (int i = 0; i < TESTNUM; i++){
		vector<int> if4 = QueryInvertedList(f4[i], 4);//测试倒排表用时
		//anl nf4 = QueryFre(f4[i], 4);//测试node-list算法用时
	}
	//计时结束
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "if4的运行时间为" << totaltime << "秒！" << endl;

	//计时开始
	start = clock();
	for (int i = 0; i < TESTNUM; i++){
		vector<int> if6 = QueryInvertedList(f6[i], 6);//测试倒排表用时
		//anl nf6 = QueryFre(f6[i], 6);//测试node-list算法用时
	}
	//计时结束
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "if6的运行时间为" << totaltime << "秒！" << endl;

	//计时开始
	start = clock();
	for (int i = 0; i < TESTNUM; i++){
		vector<int> iu2 = QueryInvertedList(u2[i], 2);//测试倒排表用时
		//anl nu2 = QueryUnfre(u2[i], 2);//测试node-list算法用时
	}
	//计时结束
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "\niu2的运行时间为" << totaltime << "秒！" << endl;

	//计时开始
	start = clock();
	for (int i = 0; i < TESTNUM; i++){
		vector<int> iu4 = QueryInvertedList(u4[i], 4);//测试倒排表用时
		//anl nu4 = QueryUnfre(u4[i], 4);//测试node-list算法用时
	}
	//计时结束
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "iu4的运行时间为" << totaltime << "秒！" << endl;

	//计时开始
	start = clock();
	for (int i = 0; i < TESTNUM; i++){
		vector<int> iu6 = QueryInvertedList(u6[i], 6);//测试倒排表用时
		//anl nu6 = QueryUnfre(u6[i], 6);//测试node-list算法用时
	}
	//计时结束
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "iu6的运行时间为" << totaltime << "秒！" << endl;


	cout << endl<<"node-list测试：";
	//计时开始
	start = clock();
	for (int i = 0; i < TESTNUM; i++){
		//vector<int> ir2 = QueryInvertedList(r2[i], 2);//测试倒排表用时
		anl nr2 = Query(r2[i], 2);//测试node-list算法用时
	}
	//计时结束
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "\nnr2的运行时间为" << totaltime << "秒！" << endl;

	//计时开始
	start = clock();
	for (int i = 0; i < TESTNUM; i++){
		//vector<int> ir4 = QueryInvertedList(r4[i], 4);//测试倒排表用时
		anl nr4 = Query(r4[i], 4);//测试node-list算法用时
	}
	//计时结束
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "nr4的运行时间为" << totaltime << "秒！" << endl;

	//计时开始
	start = clock();
	for (int i = 0; i < TESTNUM; i++){
		//vector<int> ir6 = QueryInvertedList(r6[i], 6);//测试倒排表用时
		anl nr6 = Query(r6[i], 6);//测试node-list算法用时
	}
	//计时结束
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "nr6的运行时间为" << totaltime << "秒！" << endl;

	//计时开始
	start = clock();
	for (int i = 0; i < TESTNUM; i++){
		//vector<int> if2 = QueryInvertedList(f2[i], 2);//测试倒排表用时
		anl nf2 = QueryFre(f2[i], 2);//测试node-list算法用时
	}
	//计时结束
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "\nnf2的运行时间为" << totaltime << "秒！" << endl;

	//计时开始
	start = clock();
	for (int i = 0; i < TESTNUM; i++){
		//vector<int> if4 = QueryInvertedList(f4[i], 4);//测试倒排表用时
		anl nf4 = QueryFre(f4[i], 4);//测试node-list算法用时
	}
	//计时结束
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "nf4的运行时间为" << totaltime << "秒！" << endl;

	//计时开始
	start = clock();
	for (int i = 0; i < TESTNUM; i++){
		//vector<int> if6 = QueryInvertedList(f6[i], 6);//测试倒排表用时
		anl nf6 = QueryFre(f6[i], 6);//测试node-list算法用时
	}
	//计时结束
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "nf6的运行时间为" << totaltime << "秒！" << endl;

	//计时开始
	start = clock();
	for (int i = 0; i < TESTNUM; i++){
		//vector<int> iu2 = QueryInvertedList(u2[i], 2);//测试倒排表用时
		anl nu2 = QueryUnfre(u2[i], 2);//测试node-list算法用时
	}
	//计时结束
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "\nnu2的运行时间为" << totaltime << "秒！" << endl;

	//计时开始
	start = clock();
	for (int i = 0; i < TESTNUM; i++){
		//vector<int> iu4 = QueryInvertedList(u4[i], 4);//测试倒排表用时
		anl nu4 = QueryUnfre(u4[i], 4);//测试node-list算法用时
	}
	//计时结束
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "nu4的运行时间为" << totaltime << "秒！" << endl;

	//计时开始
	start = clock();
	for (int i = 0; i < TESTNUM; i++){
		//vector<int> iu6 = QueryInvertedList(u6[i], 6);//测试倒排表用时
		anl nu6 = QueryUnfre(u6[i], 6);//测试node-list算法用时
	}
	//计时结束
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "nu6的运行时间为" << totaltime << "秒！" << endl;
	
	system("pause");
	return 0;
}