#include <iostream>
#include <string>

#include "food.h"

using namespace std;

food::food(string _name, string _fileName, string _category, int _season, int _period)
{
	name = _name;
	fileName = _fileName;
	category = _category;
	season = _season;
	period = _period;
}

void food::setHealth(double _cal, double _protein, double _calcium, double _iron, double _vitaminB1, double _vitaminB2, double _vitaminC, double _vitaminE, double _weight)
{
	cal = _cal;
	protein = _protein;
	calcium = _calcium;
	iron = _iron;
	vitaminB1 = _vitaminB1;
	vitaminB2 = _vitaminB2;
	vitaminC = _vitaminC;
	vitaminE = _vitaminE;
	weight = _weight;
}

void food::printInfo()
{
	cout << "name: " << name << endl;
	cout << "fileName: " << fileName << endl;
	cout << "category: " << category << endl;
	cout << "season: " << season << endl;
	cout << "period: " << period << endl;
}

void food::printHealth()
{
	cout << "cal: " << cal << endl;
	cout << "protein: " << protein << endl;
	cout << "calcium: " << calcium << endl;
	cout << "iron: " << iron << endl;
	cout << "vitaminB1: " << vitaminB1 << endl;
	cout << "vitaminB2: " << vitaminB2 << endl;
	cout << "vitaminC: " << vitaminC << endl;
	cout << "vitaminE: " << vitaminE << endl;
}

bool operator == (const food &a, const food &b){
	if ((a.name == b.name) && (a.fileName == b.fileName) && (a.category == b.category) && (a.season == b.season) && (a.period == b.period))
		return true;
	else
		return false;
}

bool operator != (const food &a, const food &b){
	if ((a.name == b.name) && (a.fileName == b.fileName) && (a.category == b.category) && (a.season == b.season) && (a.period == b.period))
		return false;
	else
		return true;
}
