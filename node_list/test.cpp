#include<iostream>
#include<map>
#include<stdlib.h>
#include<time.h>
using namespace std;

map<int, int> arec;
map<tuple<int,int>, int> aset2;
map<tuple<int, int, int, int>, int> aset4;
map<tuple<int, int, int, int, int, int>, int> aset6;
int f[1000];
int fi = 1000;
short f2[1000][2];
short f4[1000][4];
short f6[1000][6];
tuple<int, int> cur2;
tuple<int, int, int, int> cur4;
tuple<int, int, int, int, int, int> cur6;
void GenFre2(int count, int left, int right, short*pos){
	//cout <<"count:"<< count << endl;
	if (count == 2){
		//cout << "count == n" << endl;
		//cout << "cur gen:";
		int i = 0;
		for (auto elem : arec){
			*(pos+i) = elem.first;
			//cout << elem.first << " ";
			i++;
		}
		//cout << endl;
		arec.clear();
		cur2 = make_tuple(*pos, *(pos+1));
		return;
	}
		
	
	int cur = rand() % (right - left) + left;
	//cout << "cur random:" << cur << endl;
	if (arec.find(cur) == arec.end()){
		//cout << "arec.find(cur) == arec.end()" << endl;
		count++;
		arec[cur] = 1;
	}
	GenFre2(count, left, right, pos);
	return;
}
void GenFre4(int count, int left, int right, short*pos){
	//cout <<"count:"<< count << endl;
	if (count == 4){
		//cout << "count == n" << endl;
		//cout << "cur gen:";
		int i = 0;
		for (auto elem : arec){
			*(pos + i) = elem.first;
			//cout << elem.first << " ";
			i++;
		}
		//cout << endl;
		arec.clear();
		cur4 = make_tuple(*pos, *(pos + 1), *(pos + 2), *(pos + 3));
		return;
	}


	int cur = rand() % (right - left) + left;
	//cout << "cur random:" << cur << endl;
	if (arec.find(cur) == arec.end()){
		//cout << "arec.find(cur) == arec.end()" << endl;
		count++;
		arec[cur] = 1;
	}
	GenFre4(count, left, right, pos);
	return;
}
void GenFre6(int count, int left, int right, short*pos){
	//cout <<"count:"<< count << endl;
	if (count == 6){
		//cout << "count == n" << endl;
		//cout << "cur gen:";
		int i = 0;
		for (auto elem : arec){
			*(pos + i) = elem.first;
			//cout << elem.first << " ";
			i++;
		}
		//cout << endl;
		arec.clear();
		cur6 = make_tuple(*pos, *(pos + 1), *(pos + 2), *(pos + 3), *(pos + 4), *(pos + 5));
		return;
	}


	int cur = rand() % (right - left) + left;
	//cout << "cur random:" << cur << endl;
	if (arec.find(cur) == arec.end()){
		//cout << "arec.find(cur) == arec.end()" << endl;
		count++;
		arec[cur] = 1;
	}
	GenFre6(count, left, right, pos);
	return;
}
void GenFre(int n, int left, int right){
	for (int i = 0; i < n;){
		GenFre2(0, left, right, f2[i]);
		if (aset2.find(cur2) == aset2.end()){
			i++;
			aset2[cur2] = 1;
		}
	}
	for (int i = 0; i < n;){
		GenFre4(0, left, right, f4[i]);
		if (aset4.find(cur4) == aset4.end()){
			i++;
			aset4[cur4] = 1;
		}
	}
	for (int i = 0; i < n;){
		GenFre6(0, left, right, f6[i]);
		if (aset6.find(cur6) == aset6.end()){
			i++;
			aset6[cur6] = 1;
		}
	}
	return;
}
int main(){
	
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
	system("pause");
	return 0;
}