#include<vector>
#include<map>
#include<iostream>
#include<fstream>
#include<sstream>
#include<string>
using namespace std;


void ReadData(string fileName)
{
	ifstream in(fileName);
	/*
	while (in >> item){
	cout << item << endl;
	}
	*/
	string line;
	while (getline(in, line)){
		//cout << line << endl;
		int data;
		istringstream li(line);
		cout << "line: ";
		while (li >> data)
			cout << data <<" ";
		cout <<endl;
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