#include<vector>
#include<map>
#include<iostream>
#include<fstream>
#include<string>
using namespace std;

void ReadData(string fileName)
{
	ifstream in(fileName);
	int item;
	/*
	while (in >> item){
	cout << item << endl;
	}
	*/
	string line;
	while (getline(in, line)){
		cout << line << endl;
	}
	
	
}

int main()
{
	string fileName = ".//data//mushroom.dat";
	ReadData(fileName);
	//CountData();
	//RearrangeData();
	//BuildTree();
	//DLR();
	//LRD();
	//GenNodeList();
	//Query();

}