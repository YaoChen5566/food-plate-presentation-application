#include <iostream>
#include <vector>

using namespace std;
#ifndef recipe_H
#define recipe_H

class recipe
{
public:

	recipe();

	void addFood(string _name, int _num, int _type);
	void reduceFood(string _name, int _num);
	void writeRecipeFile(string _path);
	void readRecipeFile(string _path);
	void print();
	void reset();
	int size;
	vector<string> foodName();
	vector<int> foodNum();
	vector<int> foodType();

private:
	vector<string> _foodName;
	vector<int> _foodNum;
	vector<int> _foodType;

	vector<string> __foodName;
	vector<int> __foodNum;
	vector<int> __foodType;
};
#endif