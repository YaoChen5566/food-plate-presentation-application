#pragma once
#include <opencv\cv.h>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <dirent.h>
#include <errno.h>
#include <fstream>
#include <string>
#include <sstream>
#include <map>
#include <windows.h>

#include <QtWidgets/QMainWindow>
#include "ui_QtGuiApplication1.h"
#include "fragment.h"
#include "recipe.h"
#include "tree.hh"
#include "food.h"

using namespace std;
using namespace cv;


#ifndef QtGuiApplication1_H
#define QtGuiApplication1_H

struct fragList
{
	vector<frag> Element;
};

class QtGuiApplication1 : public QMainWindow
{
    Q_OBJECT

public:
	Ui::QtGuiApplication1Class ui;
    QtGuiApplication1(QWidget *parent = Q_NULLPTR);
	
private:
	void openInputImage(string filePath);
	void updateShowTable(QTableWidget *table);
	void compareWithRecipe(Mat& userDraw, recipe& rec2);
	void buildTree(tree<string>& tr, recipe recipeTmp, string str, int currentIndex, int fragIndex, vector<fragList>& sortedFragList, Mat userDraw, double iErrorValue, vector<int>& groupIdxOfContour);
	void updateResultTable();
	void updateNutrientSlider(std::vector<int> tmpNutrient);
	void bestResult(vector<fragList> sortedFragList);


	private slots:
	void QTreeWidgetClicked_Recipe(QTreeWidgetItem* item, int column);
	void QTreeWidgetClicked_Food(QTreeWidgetItem* item, int column);
	void ResultIndexClicked(QTableWidgetItem* item);
	void removeFoodItem2(QTableWidgetItem* item);
	void changeAmount_Food(int currentRow);
	void changeAmount_Nutrient(int currentRow);
	void sliderRatioChange(int value);
	// button function
	void applyClicked();
	void runAllReipeClicked();
	void runAllImageClicked();
	void openImageClicked();
};
#endif
