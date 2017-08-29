#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <fstream>

#include "recipe.h"

using namespace std;

recipe::recipe()
{
	size = 0;
}

void recipe::addFood(string _name, int _num, int _type)
{
	size++;
	_foodName.push_back(_name);
	_foodNum.push_back(_num);
	_foodType.push_back(_type);
	__foodName.push_back(_name);
	__foodNum.push_back(_num);
	__foodType.push_back(_type);
}

void recipe::reduceFood(string _name, int _num)
{
	vector<string>::iterator iter;
	iter = find(_foodName.begin(), _foodName.end(), _name);

	if (iter == _foodName.end())
		cout << "dont have this food" << endl;
	else
	{
		if (_foodNum[iter - _foodName.begin()] > _num)
			_foodNum[iter - _foodName.begin()] -= _num;
		else
			_foodNum[iter - _foodName.begin()] = 0;
	}
}

void recipe::writeRecipeFile(string _path)
{
	ofstream fileR(_path, ios::binary);
	//cout <<__foodIndex[0]<<" "<<__foodNum[0]<<" "<<__foodType[0]<<endl;
	//outfile << size;
	fileR.write((char*)&size, sizeof(int));
	//cout <<__foodIndex[0]<<" "<<__foodNum[0]<<" "<<__foodType[0]<<endl;

	for (int i = 0; i < size; i++)
	{
		int nameSize = __foodName[i].size();
		string fName = __foodName[i];
		int fNum = __foodNum[i];
		int fType = __foodType[i];
		/*write food name size, type int*/
		fileR.write((char*)&nameSize, sizeof(int));
		/*write food name, type string*/
		fileR.write(fName.c_str(), fName.size());
		/*write food num, type int*/
		fileR.write((char*)&fNum, sizeof(int));
		/*write food type, type int*/
		fileR.write((char*)&fType, sizeof(int));
	}



	fileR.close();
}

void recipe::readRecipeFile(string _path)
{
	ifstream fileR(_path, ios::binary);
	//int loc;
	int nameSize;
	string fName;

	int fNum;
	int fType;

	if (fileR.good())
	{
		fileR.read((char*)&size, sizeof(int));
		for (int i = 0; i < size; i++)
		{

			fileR.read((char*)&nameSize, sizeof(int));
			char* tempN = new char[nameSize + 1];
			fileR.read(tempN, nameSize);
			tempN[nameSize] = '\0';
			fileR.read((char*)&fNum, sizeof(int));
			fileR.read((char*)&fType, sizeof(int));
			//cout << fIdx<<" "<<fNum<<" "<<fType<<endl;
			_foodName.push_back(tempN);
			__foodName.push_back(tempN);
			_foodNum.push_back(fNum);
			__foodNum.push_back(fNum);
			_foodType.push_back(fType);
			__foodType.push_back(fType);
		}
	}
	else
	{
		cout << "recipe file error" << endl;
	}

}

void recipe::print()
{
	cout << "size: " << size << endl;

	cout << "food Name:";
	for (vector<string>::iterator iter = _foodName.begin(); iter != _foodName.end(); iter++)
		cout << " " << *iter;
	cout << endl;

	cout << "food Num:";
	for (vector<int>::iterator iter = _foodNum.begin(); iter != _foodNum.end(); iter++)
		cout << " " << *iter;
	cout << endl;

	cout << "food Type:";
	for (vector<int>::iterator iter = _foodType.begin(); iter != _foodType.end(); iter++)
		cout << " " << *iter;
	cout << endl;

}

void recipe::reset()
{
	_foodName.clear();
	_foodNum.clear();
	_foodType.clear();

	_foodName.assign(__foodName.begin(), __foodName.end());
	_foodNum.assign(__foodNum.begin(), __foodNum.end());
	_foodType.assign(__foodType.begin(), __foodType.end());

}

vector<string> recipe::foodName()
{
	return _foodName;
}

vector<int> recipe::foodNum()
{
	return _foodNum;
}

vector<int> recipe::foodType()
{
	return _foodType;
}