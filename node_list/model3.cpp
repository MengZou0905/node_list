#include<vector>
#include<map>
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
#include<stdlib.h>
#include<algorithm>
using namespace std;

map<int, vector<int>> setData;

int ReadFile(string path)
{
	ifstream in(path);
	string line;
	int setId;
	int maxnum = 0;
	for (setId = 0; getline(in, line) && setId<5; setId++){
		istringstream li(line);
		vector<int> temp;
		int data;
		while (li >> data) {
			setData[setId].push_back(data);
		}

	}
	return setId;
}

int itemCount[1000] = { 0 };
void CountData()
{
	int total = 0;
	memset(itemCount, 0, 1000 * sizeof(int));
	for (int i = 0; i < setData.size(); i++)
		for (int j = 0; j < setData[i].size(); j++)
			itemCount[setData[i][j]] += 1;
	//cout << "CountData Done--------" << endl;
	return;
}

bool compare(int a, int b){
	return ((itemCount[a] > itemCount[b]));
}

void RearrangeData()
{
	for (int i = 0; i < setData.size(); i++)
		sort(setData[i].begin(), setData[i].end(), compare);
	cout << "RearrangeData Done--------" << endl;
	
	for (int i = 0; i < setData.size(); i++){
		cout << "after:";
		for (int j = 0; j < setData[i].size(); j++)
			cout << setData[i][j] << "(" << itemCount[setData[i][j]] << ") ";
		cout << endl;
	}
	
}

class node
{
public:
	bool leaf;
	int DLR, LRD;
	map<int, vector<int>> nodeRec;
	vector<node *> son;
	node *par;
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
		for (int i = 0; i < aset.second.size(); i++){//���ڵ��������
			if (nodePre->son.empty() == true){//��ǰ·����ͷ��,�½��ڵ㲢����
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
			else{//��ǰ·�����»��нڵ�
				int find = 0;
				vector<node *>::iterator vi;
				for (vi = nodePre->son.begin(); vi != nodePre->son.end();++vi){
					if ((**vi).nodeRec.find(aset.second[i])!=(**vi).nodeRec.end()){//id��ͬ�Ľڵ��Ѿ��������У������½��ڵ㣬ֻ����ԭ�нڵ�������set id
						(**vi).nodeRec[aset.second[i]].push_back(aset.first);
						find = 1;
						if (itemCount[aset.second[i]] >= fre){
							nodePre = (*vi);
						}
						break;
					}
				}
				if (find == 0){//��id�ڵ㻹û�в��뵽����,��Ҫ�жϴ�id�ǲ��Ƿ�Ƶ���ġ�
					int findLeaf = 0;

					if (itemCount[aset.second[i]] < fre){//�����ǰid�Ƿ�Ƶ���Ҫ�ҵ�ǰ��son����û��Ҷ�ӽڵ㣬û�о��½����о�ֱ�����id
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
void DLR(node *cur)
{
	if (cur == NULL){
		return;
	}
	cur->DLR = index;
	index = index + 1;
	vector<node *>::iterator vi;
	for (vi = cur->son.begin(); vi != cur->son.end(); ++vi){
		DLR(*vi);
	}
	return;
}

void LRD(node *cur)
{
	if (cur->son.empty() == true){
		cur->LRD = index;
		index = index + 1;
		return;
	}
	vector<node *>::iterator vi;
	for (vi = cur->son.begin(); vi != cur->son.end(); ++vi){
		LRD(*vi);
	}
	cur->LRD = index;
	index = index + 1;
	return;
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
	return;
}

int main(){
	
	map<string, float> data_fre = { { ".//data//mushroom.dat", 0.25 }, { ".//data//accidents.dat", 0.5 }, { ".//data//T10I4D100K.dat", 0.005 } };
	string path = ".//data//mushroom.dat";
	float fre = 0.25;
	float baseFre;
	int setNum;
	
	setNum = ReadFile(path);
	CountData();
	RearrangeData();
	baseFre = setNum * fre;
	cout << "Ƶ�������֧�ֶȣ�" <<baseFre<< endl;
	node * root = BuildTree(baseFre);
	ResetIndex();
	DLR(root);
	ResetIndex(); 
	LRD(root);
	CheckTree(root);
	system("pause");
	return 0;
}