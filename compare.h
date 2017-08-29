#include <opencv\cv.h>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <algorithm>
#include <map>
#include "fragment.h"
#include "food.h"

# define PI 3.1415926

using namespace std;
using namespace cv;

#ifndef compare_H
#define compare_H

class comp
{
public:

	comp();
	comp(int index, bool isReverse, double lLimit, Size inputSize, vector<Mat> descri1Seq, vector<Mat> des2Seq, vector<Point> pointSeq1, vector<Point> pointSeq2, int contourIndex, int foodIndex, vector<food> totalFood, map<string, Mat> totalFoodImg);
	vector<map<string, int> > fragList();
	vector<frag> fragList2();
	vector<frag> fragList3();
	bool isGroup();

private:
	void setInitial();
	void setScoreThreshold();
	void findCombination();
	void compareDesN(Mat input1, Mat input2, int index, bool cLarge);
	bool isExist(Point idx, vector<Point> &vec);
	void localMaxOfRQMap();
	vector<Point> subPointSeq(vector<Point> inputSeq, int startIndex, int matchL);
	void imageOverlap(vector<Point> &contourPoint);
	void gridCutFood();


	string _foodDir;
	int _state;
	int _lLimit;
	int _startIndex1;
	int _startIndex2;
	int _range;
	int _fIndex;
	int _cIndex;
	bool _isReverse;
	bool _isGroup;
	double _thresholdScore;
	double _ratio1;
	double _ratio2;
	double _iError;
	int _rDesSize;
	int _qDesSize;
	double _minScore;
	Size _inputSize;

	Mat _mapRQ;
	Mat _mapScore;
	Mat _warpResult;
	vector<map<string, int> > _frag;
	vector<frag> _frag2;
	vector<frag> _frag3;
	vector<map<string, int> > _totalFrag;
	vector< map<string, int> > _clearResult;
	vector<Point> _pointSeq1;
	vector<Point> _pointSeq2;
	vector<food> _totalFood;
	map<string, Mat> _totalFoodImg;
};
#endif
