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
	cout << "���ű�ƽ�����ȣ�" << evaLen << endl;
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
		for (int i = 0; i < aset.second.size(); i++){//���ڵ��������
			if (nodePre->son.empty() == true){//��ǰ·����ͷ��,�½��ڵ㲢����
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
			else{//��ǰ·�����»��нڵ�
				int find = 0;
				vector<node *>::iterator vi;
				for (vi = nodePre->son.begin(); vi != nodePre->son.end();++vi){
					if ((**vi).nodeRec.find(aset.second[i])!=(**vi).nodeRec.end()){//id��ͬ�Ľڵ��Ѿ��������У������½��ڵ㣬ֻ����ԭ�нڵ�������set id
						(**vi).nodeRec[aset.second[i]].push_back(aset.first);
						find = 1;
						if (itemCount[aset.second[i]] >= baseFre){
							nodePre = (*vi);
						}
						break;
					}
				}
				if (find == 0){//��id�ڵ㻹û�в��뵽����,��Ҫ�жϴ�id�ǲ��Ƿ�Ƶ���ġ�
					int findLeaf = 0;

					if (itemCount[aset.second[i]] < baseFre){//�����ǰid�Ƿ�Ƶ���Ҫ�ҵ�ǰ��son����û��Ҷ�ӽڵ㣬û�о��½����о�ֱ�����id
						vector<node *>::iterator vii;
						for (vii = nodePre->son.begin(); vii != nodePre->son.end(); ++vii){
							if ((**vii).leaf == 1){
								findLeaf = 1;
								break;
							}
						}
						if (findLeaf == 1){//�Ѿ�����Ҷ�ӽڵ㣬ֱ����Ҷ�ӽڵ�������µļ�¼
							(**vii).nodeRec[aset.second[i]].push_back(aset.first);
						}
						else{//û��Ҷ�ӽڵ㣬��Ҫ�½�Ҷ�ӽڵ�
							nodeCur = new node;
							nodeCur->leaf = 1;
							nodeCur->nodeRec[aset.second[i]].push_back(aset.first);
							nodePre->son.push_back(nodeCur);
						}

					}
					else{//��ǰid��Ƶ����½��ڵ��������
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
	cout << "nodeListƽ������nodes��" << evaLen << endl;
	cout << "ÿ��nodeƽ������records:" << evaNodeLen << endl;
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
	//Gen2:�����������ظ��������������posָ���������
	//count:�Ѿ��ɹ����ɵ������
	//left����߽磬����ȡ��
	//right���ұ߽磬ȡ����
	//pos:���2���������������ʼλ��
	if (count == 2){
		int i = 0;
		//cout << "gen2:";
		for (auto elem : arec){
			*(pos + i) = *(ori + elem.first);
			//cout << *(pos + i) << ",";
			i++;
		}
		//����node-list�ɶ̵�������
		//sort(pos, pos+2,CompareOnNodeList);
		//����support�ɶൽ������
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
	//Gen4:�����ĸ����ظ��������������posָ���������
	//count:�Ѿ��ɹ����ɵ������
	//left����߽磬����ȡ��
	//right���ұ߽磬ȡ����
	//pos:���4���������������ʼλ��
	if (count == 4){
		int i = 0;
		for (auto elem : arec){
			*(pos + i) = *(ori + elem.first);
			i++;
		}
		//����node-list�ɶ̵�������
		//sort(pos, pos+4,CompareOnNodeList);
		//����support�ɶൽ������
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
	//Gen6:�����������ظ��������������posָ���������
	//count:�Ѿ��ɹ����ɵ������
	//left����߽磬����ȡ��
	//right���ұ߽磬ȡ����
	//pos:���2���������������ʼλ��
	if (count == 6){
		int i = 0;
		for (auto elem : arec){
			*(pos + i) = *(ori + elem.first);
			i++;
		}
		//����node-list�ɶ̵�������
		//sort(pos, pos+6,CompareOnNodeList);
		//����support�ɶൽ������
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
	//GenFre����Ƶ����������n������ͬ���������������ȷֱ�Ϊ2��4��6
	//left����߽磬����ȡ��
	//right:�ұ߽磬ȡ����
	
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
		Gen2(0, ori, left, right, r2[i]);//�Ѿ�����׼��node-list����support���ź���
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
			//��fre������ǰ�棬unfre�����ں��棬�����ֶ���support�����ǰ
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
	//for (int i = 0; i<2 && result.empty() == true; i++){//�����ڲ�ѯ���node-list�ɶ̵������е��������ֹ��ѯ��Ӹ���˳���²�ѯ���Ϊ��
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
		//swap(a, b);//�����ڲ�ѯ���node-list�ɶ̵������е��������ֹ��ѯ��Ӹ���˳���²�ѯ���Ϊ��
	//}//�����ڲ�ѯ���node-list�ɶ̵������е��������ֹ��ѯ��Ӹ���˳���²�ѯ���Ϊ��
	
	
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
			if ((*ai).first.first == (*bi).first.first){//DLR��ͬ
				//������
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
	//sep == size��ȫ��frequent id
	//sep == 0��ȫ��unfre id
	//else,��������fre��Ҳ��unfre
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
	
	//��ʱ����
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
	cout << "��ǰ���ݼ�: " << path << endl << "frequent creterior: " << fre << endl;
	cout << "��ǰ���ݼ�Ƶ�������֧�ֶȣ�" << baseFre << endl;
	cout << "Ƶ����������:" << fi << endl;
	cout << "��Ƶ����������:" << ui << endl;
	cout << "���Լ���С:" << TESTNUM << endl;
	
	GenFre(TESTNUM, f, 0, fi);
	GenUnfre(TESTNUM, u, 0, ui);
	GenRan(TESTNUM, r, 0, ri);
	//CheckGen();
	//cout << "GenTestData Done----------" << endl;
	
	cout <<endl<< "���Ų���" ;
	//��ʱ��ʼ
	start = clock();
	for (int i = 0; i < TESTNUM; i++){
		vector<int> ir2 = QueryInvertedList(r2[i], 2);//���Ե��ű���ʱ
		//anl nr2 = Query(r2[i], 2);//����node-list�㷨��ʱ
	}
	//��ʱ����
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "\nir2������ʱ��Ϊ" << totaltime << "�룡" << endl;

	//��ʱ��ʼ
	start = clock();
	for (int i = 0; i < TESTNUM; i++){
		vector<int> ir4 = QueryInvertedList(r4[i], 4);//���Ե��ű���ʱ
		//anl nr4 = Query(r4[i], 4);//����node-list�㷨��ʱ
	}
	//��ʱ����
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "ir4������ʱ��Ϊ" << totaltime << "�룡" << endl;

	//��ʱ��ʼ
	start = clock();
	for (int i = 0; i < TESTNUM; i++){
		vector<int> ir6 = QueryInvertedList(r6[i], 6);//���Ե��ű���ʱ
		//anl nr6 = Query(r6[i], 6);//����node-list�㷨��ʱ
	}
	//��ʱ����
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "ir6������ʱ��Ϊ" << totaltime << "�룡" << endl;

	//��ʱ��ʼ
	start = clock();
	for (int i = 0; i < TESTNUM; i++){
		vector<int> if2 = QueryInvertedList(f2[i], 2);//���Ե��ű���ʱ
		//anl nf2 = QueryFre(f2[i], 2);//����node-list�㷨��ʱ
	}
	//��ʱ����
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "\nif2������ʱ��Ϊ" << totaltime << "�룡" << endl;

	//��ʱ��ʼ
	start = clock();
	for (int i = 0; i < TESTNUM; i++){
		vector<int> if4 = QueryInvertedList(f4[i], 4);//���Ե��ű���ʱ
		//anl nf4 = QueryFre(f4[i], 4);//����node-list�㷨��ʱ
	}
	//��ʱ����
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "if4������ʱ��Ϊ" << totaltime << "�룡" << endl;

	//��ʱ��ʼ
	start = clock();
	for (int i = 0; i < TESTNUM; i++){
		vector<int> if6 = QueryInvertedList(f6[i], 6);//���Ե��ű���ʱ
		//anl nf6 = QueryFre(f6[i], 6);//����node-list�㷨��ʱ
	}
	//��ʱ����
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "if6������ʱ��Ϊ" << totaltime << "�룡" << endl;

	//��ʱ��ʼ
	start = clock();
	for (int i = 0; i < TESTNUM; i++){
		vector<int> iu2 = QueryInvertedList(u2[i], 2);//���Ե��ű���ʱ
		//anl nu2 = QueryUnfre(u2[i], 2);//����node-list�㷨��ʱ
	}
	//��ʱ����
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "\niu2������ʱ��Ϊ" << totaltime << "�룡" << endl;

	//��ʱ��ʼ
	start = clock();
	for (int i = 0; i < TESTNUM; i++){
		vector<int> iu4 = QueryInvertedList(u4[i], 4);//���Ե��ű���ʱ
		//anl nu4 = QueryUnfre(u4[i], 4);//����node-list�㷨��ʱ
	}
	//��ʱ����
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "iu4������ʱ��Ϊ" << totaltime << "�룡" << endl;

	//��ʱ��ʼ
	start = clock();
	for (int i = 0; i < TESTNUM; i++){
		vector<int> iu6 = QueryInvertedList(u6[i], 6);//���Ե��ű���ʱ
		//anl nu6 = QueryUnfre(u6[i], 6);//����node-list�㷨��ʱ
	}
	//��ʱ����
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "iu6������ʱ��Ϊ" << totaltime << "�룡" << endl;


	cout << endl<<"node-list���ԣ�";
	//��ʱ��ʼ
	start = clock();
	for (int i = 0; i < TESTNUM; i++){
		//vector<int> ir2 = QueryInvertedList(r2[i], 2);//���Ե��ű���ʱ
		anl nr2 = Query(r2[i], 2);//����node-list�㷨��ʱ
	}
	//��ʱ����
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "\nnr2������ʱ��Ϊ" << totaltime << "�룡" << endl;

	//��ʱ��ʼ
	start = clock();
	for (int i = 0; i < TESTNUM; i++){
		//vector<int> ir4 = QueryInvertedList(r4[i], 4);//���Ե��ű���ʱ
		anl nr4 = Query(r4[i], 4);//����node-list�㷨��ʱ
	}
	//��ʱ����
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "nr4������ʱ��Ϊ" << totaltime << "�룡" << endl;

	//��ʱ��ʼ
	start = clock();
	for (int i = 0; i < TESTNUM; i++){
		//vector<int> ir6 = QueryInvertedList(r6[i], 6);//���Ե��ű���ʱ
		anl nr6 = Query(r6[i], 6);//����node-list�㷨��ʱ
	}
	//��ʱ����
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "nr6������ʱ��Ϊ" << totaltime << "�룡" << endl;

	//��ʱ��ʼ
	start = clock();
	for (int i = 0; i < TESTNUM; i++){
		//vector<int> if2 = QueryInvertedList(f2[i], 2);//���Ե��ű���ʱ
		anl nf2 = QueryFre(f2[i], 2);//����node-list�㷨��ʱ
	}
	//��ʱ����
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "\nnf2������ʱ��Ϊ" << totaltime << "�룡" << endl;

	//��ʱ��ʼ
	start = clock();
	for (int i = 0; i < TESTNUM; i++){
		//vector<int> if4 = QueryInvertedList(f4[i], 4);//���Ե��ű���ʱ
		anl nf4 = QueryFre(f4[i], 4);//����node-list�㷨��ʱ
	}
	//��ʱ����
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "nf4������ʱ��Ϊ" << totaltime << "�룡" << endl;

	//��ʱ��ʼ
	start = clock();
	for (int i = 0; i < TESTNUM; i++){
		//vector<int> if6 = QueryInvertedList(f6[i], 6);//���Ե��ű���ʱ
		anl nf6 = QueryFre(f6[i], 6);//����node-list�㷨��ʱ
	}
	//��ʱ����
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "nf6������ʱ��Ϊ" << totaltime << "�룡" << endl;

	//��ʱ��ʼ
	start = clock();
	for (int i = 0; i < TESTNUM; i++){
		//vector<int> iu2 = QueryInvertedList(u2[i], 2);//���Ե��ű���ʱ
		anl nu2 = QueryUnfre(u2[i], 2);//����node-list�㷨��ʱ
	}
	//��ʱ����
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "\nnu2������ʱ��Ϊ" << totaltime << "�룡" << endl;

	//��ʱ��ʼ
	start = clock();
	for (int i = 0; i < TESTNUM; i++){
		//vector<int> iu4 = QueryInvertedList(u4[i], 4);//���Ե��ű���ʱ
		anl nu4 = QueryUnfre(u4[i], 4);//����node-list�㷨��ʱ
	}
	//��ʱ����
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "nu4������ʱ��Ϊ" << totaltime << "�룡" << endl;

	//��ʱ��ʼ
	start = clock();
	for (int i = 0; i < TESTNUM; i++){
		//vector<int> iu6 = QueryInvertedList(u6[i], 6);//���Ե��ű���ʱ
		anl nu6 = QueryUnfre(u6[i], 6);//����node-list�㷨��ʱ
	}
	//��ʱ����
	finish = clock();
	totaltime = (double)(finish - start) / CLOCKS_PER_SEC;
	cout << "nu6������ʱ��Ϊ" << totaltime << "�룡" << endl;
	
	system("pause");
	return 0;
}