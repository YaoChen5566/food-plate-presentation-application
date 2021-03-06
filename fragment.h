#include <opencv\cv.h>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>

#ifndef fragment_H
#define fragment_H

using namespace std;
using namespace cv;

class frag
{
public:

	frag();

	int r; // reference start
	int q; // query start
	int l; // match length
	int fIndex; //file index
	int cIndex; //contour index
	int rIndex; // recipe index
	bool isReverse;
	double score; //fragment score
	vector<int> recipe_num;
	Mat warpMatrix; //warping matrix
	Mat warpImg;
	double eError; //edge error
	double cError; //color error
	double rError; //reference error
	double sError; //sum of three error
	double iError; // intersection error
	double iErrorRatio1; // intersection error
	
	void setInfo(int _r, int _q, int _l, double _score, int _cIndex, int _fIndex, Mat _warpMatrix, Mat _warpImg);
	void setError(double _eError, double _cError, double _rError, double _iError, double _iErrorRatio1);
	bool theSame(frag A);
private:


};

#endif