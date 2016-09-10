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

void ReadFile(string path)
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
}

int itemCount[1000] = { 0 };
int CountData()
{
	int total = 0;
	memset(itemCount, 0, 1000 * sizeof(int));
	for (int i = 0; i < setData.size(); i++)
		for (int j = 0; j < setData[i].size(); j++)
			itemCount[setData[i][j]] += 1;
	//cout << "CountData Done--------" << endl;
	return total;
}

bool compare(int a, int b){
	return ((itemCount[a] > itemCount[b]) && (a < b));
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
			}
			else{//��ǰ·�����»��нڵ�
				int find = 0;
				vector<node *>::iterator vi;
				for (vi = nodePre->son.begin(); vi != nodePre->son.end();++vi){
					if ((**vi).nodeRec.find(aset.second[i])!=(**vi).nodeRec.end()){//id��ͬ�Ľڵ��Ѿ��������У������½��ڵ㣬ֻ����ԭ�нڵ�������set id
						(**vi).nodeRec[aset.second[i]].push_back(aset.first);
						find = 1;
						break;
					}
				}
				if (find == 0){//��id�ڵ㻹û�в��뵽���У��½��ڵ㲢����
					//�����ǰid�Ƿ�Ƶ���Ҫ�ҵ�ǰ��son����û��Ҷ�ӽڵ㣬û�о��½����о�ֱ�����id
					nodeCur = new node;
					nodeCur->nodeRec[aset.second[i]].push_back(aset.first);
					nodePre->son.push_back(nodeCur);
				}
			}
		}
	}
	return root;
}

int main(){
	
	string path = ".//data//mushroom.dat";
	ReadFile(path);
	node * root = BuildTree();
	system("pause");
	return 0;
}