#include <iostream>
#include <string>
#include <vector>

#ifndef food_H
#define food_H

using namespace std;

class food
{
public:
	food(string _name, string _fileName, string _category, int _season, int _period);

	void setHealth(double _cal, double _protein, double _calcium, double _iron, double _vitaminB1, double _vitaminB2, double _vitaminC, double _vitaminE, double _weight);
	void printInfo();
	void printHealth();

	string name;
	string fileName;
	string category;
	int season;
	int period;
	int orderIdx;

	double cal;
	double protein;
	double calcium;
	double iron;
	double vitaminB1;
	double vitaminB2;
	double vitaminC;
	double vitaminE;
	double weight;

	friend bool operator == (const food &a, const food &b);
	friend bool operator != (const food &a, const food &b);

};

#endif