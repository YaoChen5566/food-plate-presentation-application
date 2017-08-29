#include <opencv\cv.h>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>
#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <vector>
#include <numeric>
#include <algorithm>
#include <map>
#include <fstream>
#include <dirent.h>
#include <thread>

#include "compare.h"
#include "fragment.h"
#include "food.h"
#include "GridCut\GridGraph_2D_4C.h"

# define PI 3.1415926

using namespace std;
using namespace cv;

bool compareWithLength(map<string, int> input1, map<string, int> input2)
{
	int i = input1["l"];
	int j = input2["l"];
	return(i<j);
}

vector<double> norVec(Point pre, Point tgt, Point nxt)
{
	vector<double> nor1;
	vector<double> nor2;

	nor1.push_back(pre.y - tgt.y);
	nor1.push_back(tgt.x - pre.x);
	nor2.push_back(tgt.y - nxt.y);
	nor2.push_back(nxt.x - tgt.x);

	vector<double> norT;

	norT.push_back(nor1[0] + nor2[0]);
	norT.push_back(nor1[1] + nor2[1]);
	return norT;
}

void icp(vector<Point> contourPoint, vector<Point> foodPoint, double &dX, double &dY, double &dTheta)
{
	// contour points: Q
	// food points: P, P', P''...
	double minDist = -1;
	int minPointIndex;

	double meanCX = 0.0;
	double meanCY = 0.0;
	double meanFX = 0.0;
	double meanFY = 0.0;

	vector<Point> contourPointPair;
	vector<Point> foodPointPair;
	vector<double> distVec;

	for (int i = 0; i < contourPoint.size(); i++)
	{
		meanCX += contourPoint[i].x;
		meanCY += contourPoint[i].y;
	}
	for (int i = 0; i < foodPoint.size(); i++)
	{
		meanFX += foodPoint[i].x;
		meanFY += foodPoint[i].y;
	}
	meanCX /= contourPoint.size();
	meanCY /= contourPoint.size();
	meanFX /= foodPoint.size();
	meanFY /= foodPoint.size();

	for (int i = 0; i < foodPoint.size(); i++)
	{
		vector<double> normVec = norVec(foodPoint[(i - 1) % foodPoint.size()], foodPoint[i], foodPoint[(i + 1) % foodPoint.size()]);
		double lengthN = sqrt(normVec[0] * normVec[0] + normVec[1] * normVec[1]);
		normVec[0] /= lengthN;
		normVec[1] /= lengthN;
		for (int j = 0; j < contourPoint.size(); j++)
		{
			double dist = sqrt(pow(contourPoint[j].x - foodPoint[i].x, 2) + pow(contourPoint[j].y - foodPoint[i].y, 2));

			vector<double> normFC;
			normFC.push_back(contourPoint[j].x - foodPoint[i].x);
			normFC.push_back(contourPoint[j].y - foodPoint[i].y);

			double lengthFC = sqrt(normFC[0] * normFC[0] + normFC[1] * normFC[1]);

			normFC[0] /= lengthFC;
			normFC[0] /= lengthFC;

			//dist*(1-A dot B)
			double value = dist * (1 - (normVec[0] * normFC[0] + normVec[1] * normFC[1]));

			if (minDist == -1)
			{
				minDist = value;
				minPointIndex = j;
			}
			else
			{
				if (value < minDist)
				{
					minDist = value;
					minPointIndex = j;
				}
			}

		}

		contourPointPair.push_back(contourPoint[minPointIndex]);
		foodPointPair.push_back(foodPoint[i]);
	}

	double sumFXCY = 0;
	double sumFYCX = 0;
	double sumFXCX = 0;
	double sumFYCY = 0;

	for (int i = 0; i < contourPointPair.size(); i++) //contourPointPair.size == foodPointPair.size
	{
		sumFXCY += (foodPointPair[i].x - meanFX) * (contourPointPair[i].y - meanCY);
		sumFYCX += (foodPointPair[i].y - meanFY) * (contourPointPair[i].x - meanCX);
		sumFXCX += (foodPointPair[i].x - meanFX) * (contourPointPair[i].x - meanCX);
		sumFYCY += (foodPointPair[i].y - meanFY) * (contourPointPair[i].y - meanCY);
	}

	Mat H = Mat::zeros(Size(2, 2), CV_64FC1);
	H.at<double>(0, 0) = sumFXCX;
	H.at<double>(0, 1) = sumFYCX;
	H.at<double>(1, 0) = sumFXCY;
	H.at<double>(1, 1) = sumFYCY;

	SVD s(H);

	Mat R = s.vt.t()*s.u.t();

	dTheta = acos(R.at<double>(0, 0));
	dX = meanCX - (meanFX*cos(dTheta) - meanFY*sin(dTheta));
	dY = meanCY - (meanFX*sin(dTheta) + meanFY*cos(dTheta));
}

// constructor
comp::comp()
{
	setInitial();
}

comp::comp(int state, bool isReverse, double lLimit, Size inputSize, vector<Mat> descri1Seq, vector<Mat> descri2Seq, vector<Point> pointSeq1, vector<Point> pointSeq2, int contourIndex, int foodIndex, vector<food> totalFood, map<string, Mat>totalFoodImg)
{
	_state = state;
	_isReverse = isReverse;
	_lLimit = lLimit;
	_inputSize = inputSize;
	_pointSeq1 = pointSeq1;
	_pointSeq2 = pointSeq2;
	_rDesSize = pointSeq1.size();
	_qDesSize = pointSeq2.size();
	_cIndex = contourIndex;
	_fIndex = foodIndex;
	_totalFood.assign(totalFood.begin(), totalFood.end());
	_totalFoodImg = totalFoodImg;
	setInitial();

	clock_t s = clock();
	//cout <<"!: " <<_fIndex << endl;
	if ((_fIndex == 5 || _fIndex == 95 || _fIndex == 128 || _fIndex == 134) && _rDesSize > 10)
	{
		gridCutFood();
	}
	else
	{
		if (_rDesSize > 0 && _qDesSize > 0 && _qDesSize < 2 * _rDesSize) {
			if (descri1Seq.size() >= descri2Seq.size())
			{
				for (int i = 0; i < descri1Seq.size(); i++)
					compareDesN(descri1Seq[i], descri2Seq[0], i, true);
			}
			else
			{
				for (int i = 0; i < descri2Seq.size(); i++)
					compareDesN(descri1Seq[0], descri2Seq[i], i, false);
			}
			if (state == 1) {
				findCombination();
			}
			else{
				localMaxOfRQMap();
			}


		}
	}
}

//set initial
void comp::setInitial()
{
	_thresholdScore = 50.0;
	_minScore = _thresholdScore;
	_startIndex1 = 0;
	_startIndex2 = 0;
	_range = 0;
	_mapRQ = Mat::zeros(Size(_qDesSize, _rDesSize), CV_32S); //Size(q, r): x is q, y is r
	_mapScore = Mat::zeros(Size(_qDesSize, _rDesSize), CV_64F); //Size(q, r): x is q, y is r
	_warpResult = Mat::zeros(_inputSize, CV_8UC4);
	_isGroup = false;
}

void comp::findCombination() {

	int limitLength = 0;
	vector<bool> rLength(_rDesSize, false);
	vector<Point> maxIdxOfCols;
	vector<Point> foodIdx;
	// Find all the contour length that correspoding to the candidate
	int maxL = 0;
	Point maxIdx;
	for (int i = 0; i < _mapRQ.rows; i++) //r
	{
		// find the local maximum length in cols
		int maxIdx_col = 0, maxL_col = 0;
		for (int j = 0; j < _mapRQ.cols; j++)
		{
			if (_mapRQ.at<int>(i, j) > maxL_col)
			{
				maxIdx_col = j;
				maxL_col = _mapRQ.at<int>(i, j);
			}
		}

		// count length of limitLength
		if (maxL_col > 0) {
			maxIdxOfCols.push_back(Point(i, maxIdx_col));
			for (int j = 0; j < _mapRQ.at<int>(i, maxIdx_col); j++) {
				int idx = (i + j) % _rDesSize;
				if (rLength[idx] == false) {
					limitLength++;
					rLength[idx] = true;
				}
			}
		}
		// find the max length in RQmap
		if (maxL_col > maxL)
		{
			maxIdx = Point(i, maxIdx_col);
			maxL = maxL_col;
		}
	}

	if (maxL > 0) {
		foodIdx.push_back(maxIdx);

		// Reinitialize rLength
		for (int i = 0; i < _rDesSize; i++)
			rLength[i] = false;
		for (int i = 0; i < maxL; i++) {
			int idx = (maxIdx.x + i) % _rDesSize;
			rLength[idx] = true;
		}

		int whileCount = 0;
		int limitLength_current = maxL;
		// find candidate to satisfy all the contour length (limitLength)
		while (limitLength_current < limitLength) {
			whileCount++;
			maxL = 0;
			for (int i = 0; i < maxIdxOfCols.size(); i++) {
				if (!isExist(maxIdxOfCols[i], foodIdx)) {
					int countL = 0;
					//count unfilled length if use this length starts from r
					for (int j = 0; j < _mapRQ.at<int>(maxIdxOfCols[i].x, maxIdxOfCols[i].y); j++) {
						int idx = (maxIdxOfCols[i].x + j) % _rDesSize;
						if (rLength[idx] == false)
							countL++;
					}
					if (countL > maxL) {
						maxIdx = maxIdxOfCols[i];
						maxL = countL;
					}
				}
			}
			foodIdx.push_back(maxIdx);
			limitLength_current += maxL;
			for (int i = 0; i < maxL; i++) {
				int idx = (maxIdx.x + i) % _rDesSize;
				rLength[idx] = true;
			}
		}

	}


	Mat dood = _totalFoodImg[_totalFood[_fIndex].fileName].clone();

	for (int i = 0; i < foodIdx.size(); i++) {
		_startIndex1 = foodIdx[i].x;
		_startIndex2 = foodIdx[i].y;
		_range = _mapRQ.at<int>(foodIdx[i].x, foodIdx[i].y);

		// calculate the warping matrix
		vector<Point> matchSeqR = subPointSeq(_pointSeq1, _startIndex1, _range);
		vector<Point> matchSeqQ = subPointSeq(_pointSeq2, _startIndex2, _range);
		Mat warpMat = estimateRigidTransform(matchSeqQ, matchSeqR, false); // (src/query, dst/reference)

		vector<Point> newPointSeq;
		for (int p = 0; p < _pointSeq2.size(); p++)
		{
			double newX = warpMat.at<double>(0, 0)*_pointSeq2[p].x + warpMat.at<double>(0, 1)*_pointSeq2[p].y + warpMat.at<double>(0, 2);
			double newY = warpMat.at<double>(1, 0)*_pointSeq2[p].x + warpMat.at<double>(1, 1)*_pointSeq2[p].y + warpMat.at<double>(1, 2);
			newPointSeq.push_back(Point((int)newX, (int)newY));
		}

		imageOverlap(newPointSeq);

		if (_ratio2 > 0.7) {
			Mat resultStack = Mat::zeros(_inputSize, CV_8UC4);
			warpAffine(dood, resultStack, warpMat, _inputSize);

			frag fragTmp;
			fragTmp.setInfo(_startIndex1, _startIndex2, _range, _mapScore.at<double>(_startIndex1, _startIndex2), _cIndex, _fIndex, warpMat, resultStack);
			fragTmp.setError(0, 0, 0, _iError, _ratio1);
			_frag3.push_back(fragTmp);
		}
	}

}

void comp::compareDesN(Mat input1, Mat input2, int index, bool cLarge)
{
	Mat smallerMat;
	Mat sub;
	clock_t c1, c2;
	if (cLarge)
	{
		smallerMat = input1(Rect(0, 0, input2.cols, input2.rows));
		sub = smallerMat - input2;
	}
	else
	{
		smallerMat = input2(Rect(0, 0, input1.cols, input1.rows));
		sub = input1 - smallerMat;
	}

	Mat integral1; // sum
	Mat integral2; // square sum

	int rLim;
	int lefttopPoint1 = 0;
	int lefttopPoint2 = 0;
	double tmpSum = 0;
	double getScore = 0;
	integral(sub, integral1, integral2);
	rLim = _lLimit*integral2.cols; // square size

	for (int i = 0; i < integral2.cols; i++)
	{

		if (cLarge)
		{
			_startIndex1 = (i + index) % input1.cols;
			_startIndex2 = i;
		}
		else
		{
			_startIndex1 = i;
			_startIndex2 = (i + index) % input2.cols;
		}

		if (_startIndex1 < _mapRQ.rows && _startIndex2 < _mapRQ.cols)
		{
			for (int r = integral2.cols; r >= rLim; r--)
			{


				_range = r;

				if ((i + r) <= integral2.cols)
				{
					tmpSum = integral2.at<double>(i, i) + integral2.at<double>(i + r - 1, i + r - 1) - integral2.at<double>(i, i + r - 1) - integral2.at<double>(i + r - 1, i);

					getScore = tmpSum / pow(r, 2);

					if (i == 0 && r == integral2.cols) {
						_minScore = getScore;
					}

					if (getScore <= _minScore && getScore >= 0 && _mapRQ.at<int>(_startIndex1, _startIndex2) == 0) {

						_minScore = getScore;
						if (_state == 1)
						{
							vector<Point> matchSeqR = subPointSeq(_pointSeq1, _startIndex1, _range);
							vector<Point> matchSeqQ = subPointSeq(_pointSeq2, _startIndex2, _range);
							Mat warpMat = estimateRigidTransform(matchSeqQ, matchSeqR, false); // (src/query, dst/reference)
							if (warpMat.size() != cv::Size(0, 0))
							{
								_mapRQ.at<int>(_startIndex1, _startIndex2) = _range;
								_mapScore.at<double>(_startIndex1, _startIndex2) = getScore;
								break;
							}
						}
						else
						{
							_mapRQ.at<int>(_startIndex1, _startIndex2) = _range;
							_mapScore.at<double>(_startIndex1, _startIndex2) = getScore;
							break;
						}

					}
				}

			}
		}
	}

}

// find the local maximum of RQ map
void comp::localMaxOfRQMap()
{
	int iterIcp = 0;
	int maxVal = -1;
	int maxR = -1;
	int maxQ = -1;
	double iError = 0.0;
	double preIcpErr = -1.0;
	double curIcpErr = 0.0;
	double dX, dY, dTheta;
	double minDist = 0.0;
	double dist = 0.0;
	int minDistIdx = 0;
	Mat warpMat;
	double minIcpErr;
	int maxIcpTime = 5;

	vector<Point> newPointSeq;
	vector<Mat> warpMatSeq;
	vector<double> errSeq;

	//food and contour pair
	vector<Point> foodPair;
	vector<Point> contourPair;
	Mat dood;
	int iterLocal = 0;
	while (maxVal != 0)
	{
		iterLocal++;
		maxVal = -1;
		// get local maximum
		for (int i = 0; i < _mapRQ.rows; i++) //r
		{
			for (int j = 0; j < _mapRQ.cols; j++) //q
			{
				if (_mapRQ.at<int>(i, j) > maxVal)
				{
					maxVal = _mapRQ.at<int>(i, j);
					maxR = i;
					maxQ = j;
				}
			}
		}
		if (maxVal > 0 && maxR != -1 && maxQ != -1)
		{
			vector<Point> matchSeqR = subPointSeq(_pointSeq1, maxR, maxVal);
			vector<Point> matchSeqQ = subPointSeq(_pointSeq2, maxQ, maxVal);

			Mat warpMat;

			if (_state == 2)
			{
				warpMat = estimateRigidTransform(matchSeqQ, matchSeqR, false);// src, dst
			}
			else
			{
				warpMat = Mat::zeros(Size(3, 3), CV_64F);
				icp(matchSeqR, matchSeqQ, dX, dY, dTheta);//dst, src

				warpMat.at<double>(0, 0) = cos(dTheta);
				warpMat.at<double>(0, 1) = -sin(dTheta);
				warpMat.at<double>(1, 0) = sin(dTheta);
				warpMat.at<double>(1, 1) = cos(dTheta);
				warpMat.at<double>(0, 2) = dX;
				warpMat.at<double>(1, 2) = dY;
			}
			if (warpMat.size() != Size(0, 0))
			{
				for (int p = 0; p < _pointSeq2.size(); p++)
				{
					double newX = warpMat.at<double>(0, 0)*_pointSeq2[p].x + warpMat.at<double>(0, 1)*_pointSeq2[p].y + warpMat.at<double>(0, 2);
					double newY = warpMat.at<double>(1, 0)*_pointSeq2[p].x + warpMat.at<double>(1, 1)*_pointSeq2[p].y + warpMat.at<double>(1, 2);
					newPointSeq.push_back(Point((int)newX, (int)newY));
				}

				//get icp error
				for (int i = 0; i < newPointSeq.size(); i++)
				{
					for (int j = 0; j < _pointSeq1.size(); j++)
					{
						dist = sqrt(pow(_pointSeq1[j].x - newPointSeq[i].x, 2) + pow(_pointSeq1[j].y - newPointSeq[i].y, 2));
						if (j == 0)
						{
							minDist = dist;
							minDistIdx = j;
						}
						else
						{
							if (dist < minDist)
							{
								minDist = dist;
								minDistIdx = j;
							}
						}

					}
					contourPair.push_back(_pointSeq1[minDistIdx]);
					foodPair.push_back(newPointSeq[i]);
					curIcpErr += minDist;
				}

				curIcpErr /= (int)newPointSeq.size();
				minIcpErr = curIcpErr;
				Mat newWarpMat = warpMat.clone();

				do
				{
					iterIcp++;
					errSeq.push_back(curIcpErr);
					warpMatSeq.push_back(newWarpMat.clone());
					preIcpErr = curIcpErr;

					icp(_pointSeq1, newPointSeq, dX, dY, dTheta);

					newWarpMat.at<double>(0, 0) = cos(dTheta);
					newWarpMat.at<double>(0, 1) = -sin(dTheta);
					newWarpMat.at<double>(1, 0) = sin(dTheta);
					newWarpMat.at<double>(1, 1) = cos(dTheta);
					newWarpMat.at<double>(0, 2) = dX;
					newWarpMat.at<double>(1, 2) = dY;

					Mat lastMat = Mat::zeros(Size(3, 3), CV_64F);
					lastMat.at<double>(0, 0) = warpMatSeq[0].at<double>(0, 0);
					lastMat.at<double>(0, 1) = warpMatSeq[0].at<double>(0, 1);
					lastMat.at<double>(1, 0) = warpMatSeq[0].at<double>(1, 0);
					lastMat.at<double>(1, 1) = warpMatSeq[0].at<double>(1, 1);
					lastMat.at<double>(0, 2) = warpMatSeq[0].at<double>(0, 2);
					lastMat.at<double>(1, 2) = warpMatSeq[0].at<double>(1, 2);
					lastMat.at<double>(2, 2) = 1.0;
					for (int w = 1; w < warpMatSeq.size(); w++)
					{
						Mat tmpMat = Mat::zeros(Size(3, 3), CV_64F);
						tmpMat.at<double>(0, 0) = warpMatSeq[w].at<double>(0, 0);
						tmpMat.at<double>(0, 1) = warpMatSeq[w].at<double>(0, 1);
						tmpMat.at<double>(1, 0) = warpMatSeq[w].at<double>(1, 0);
						tmpMat.at<double>(1, 1) = warpMatSeq[w].at<double>(1, 1);
						tmpMat.at<double>(0, 2) = warpMatSeq[w].at<double>(0, 2);
						tmpMat.at<double>(1, 2) = warpMatSeq[w].at<double>(1, 2);
						tmpMat.at<double>(2, 2) = 1.0;

						lastMat = tmpMat*lastMat;
					}
					Mat thisMat = Mat::zeros(Size(3, 3), CV_64F);
					thisMat.at<double>(0, 0) = newWarpMat.at<double>(0, 0);
					thisMat.at<double>(0, 1) = newWarpMat.at<double>(0, 1);
					thisMat.at<double>(1, 0) = newWarpMat.at<double>(1, 0);
					thisMat.at<double>(1, 1) = newWarpMat.at<double>(1, 1);
					thisMat.at<double>(0, 2) = newWarpMat.at<double>(0, 2);
					thisMat.at<double>(1, 2) = newWarpMat.at<double>(1, 2);
					thisMat.at<double>(2, 2) = 1.0;

					lastMat = thisMat*lastMat;

					newPointSeq.clear();
					for (int p = 0; p < _pointSeq2.size(); p++)
					{
						double newX = lastMat.at<double>(0, 0)*_pointSeq2[p].x + lastMat.at<double>(0, 1)*_pointSeq2[p].y + lastMat.at<double>(0, 2);
						double newY = lastMat.at<double>(1, 0)*_pointSeq2[p].x + lastMat.at<double>(1, 1)*_pointSeq2[p].y + lastMat.at<double>(1, 2);
						newPointSeq.push_back(Point((int)newX, (int)newY));
					}

					//get icp error
					for (int i = 0; i < newPointSeq.size(); i++)
					{
						for (int j = 0; j < _pointSeq1.size(); j++)
						{
							dist = sqrt(pow(_pointSeq1[j].x - newPointSeq[i].x, 2) + pow(_pointSeq1[j].y - newPointSeq[i].y, 2));
							if (j == 0)
							{
								minDist = dist;
								minDistIdx = j;
							}
							else
							{
								if (dist < minDist)
								{
									minDist = dist;
									minDistIdx = j;
								}
							}

						}
						curIcpErr += minDist;
					}
					curIcpErr /= (int)newPointSeq.size();
					if (curIcpErr < minIcpErr)
					{
						minIcpErr = curIcpErr;
						//maxIcpTime = iterIcp + 2;
					}

				} while (iterIcp < maxIcpTime);
				//find minimum value in err Seq
				int minErrIdx = 0;

				double minErr;

				minErr = *min_element(errSeq.begin(), errSeq.end());

				for (int e = 0; e < errSeq.size(); e++)
					if (errSeq[e] == minErr)
						minErrIdx = e;

				warpMatSeq.erase(warpMatSeq.begin() + minErrIdx + 1, warpMatSeq.end());

				Mat lastMat(Size(3, 3), CV_64F);
				lastMat.at<double>(0, 0) = warpMatSeq[0].at<double>(0, 0);
				lastMat.at<double>(0, 1) = warpMatSeq[0].at<double>(0, 1);
				lastMat.at<double>(0, 2) = warpMatSeq[0].at<double>(0, 2);
				lastMat.at<double>(1, 0) = warpMatSeq[0].at<double>(1, 0);
				lastMat.at<double>(1, 1) = warpMatSeq[0].at<double>(1, 1);
				lastMat.at<double>(1, 2) = warpMatSeq[0].at<double>(1, 2);
				lastMat.at<double>(2, 0) = 0;
				lastMat.at<double>(2, 1) = 0;
				lastMat.at<double>(2, 2) = 1;

				Mat tmpMat = Mat::zeros(Size(3, 3), CV_64F);
				for (int w = 1; w < warpMatSeq.size(); w++)
				{
					tmpMat.at<double>(0, 0) = warpMatSeq[w].at<double>(0, 0);
					tmpMat.at<double>(0, 1) = warpMatSeq[w].at<double>(0, 1);
					tmpMat.at<double>(0, 2) = warpMatSeq[w].at<double>(0, 2);
					tmpMat.at<double>(1, 0) = warpMatSeq[w].at<double>(1, 0);
					tmpMat.at<double>(1, 1) = warpMatSeq[w].at<double>(1, 1);
					tmpMat.at<double>(1, 2) = warpMatSeq[w].at<double>(1, 2);
					tmpMat.at<double>(2, 0) = 0;
					tmpMat.at<double>(2, 1) = 0;
					tmpMat.at<double>(2, 2) = 1;
					lastMat = tmpMat*lastMat;
				}

				Mat finalMat = Mat::zeros(Size(3, 2), CV_64F);

				finalMat.at<double>(0, 0) = lastMat.at<double>(0, 0);
				finalMat.at<double>(0, 1) = lastMat.at<double>(0, 1);
				finalMat.at<double>(0, 2) = lastMat.at<double>(0, 2);
				finalMat.at<double>(1, 0) = lastMat.at<double>(1, 0);
				finalMat.at<double>(1, 1) = lastMat.at<double>(1, 1);
				finalMat.at<double>(1, 2) = lastMat.at<double>(1, 2);

				// get scale
				//first, get affine matrix with opencv function
				vector<Point> tmpFoodPair;
				vector<Point> tmpContourPair;
				for (int i = 0; i < _pointSeq2.size(); i++)
				{
					for (int j = 0; j < _pointSeq1.size(); j++)
					{
						double tmpDist = sqrt(pow(_pointSeq1[j].x - newPointSeq[i].x, 2) + pow(_pointSeq1[j].y - newPointSeq[i].y, 2));
						if (j == 0)
						{
							minDist = tmpDist;
							minDistIdx = j;
						}
						else
						{
							if (tmpDist < minDist)
							{
								minDist = tmpDist;
								minDistIdx = j;
							}
						}
					}
					tmpFoodPair.push_back(_pointSeq2[i]);
					tmpContourPair.push_back(_pointSeq1[minDistIdx]);
				}

				Mat sWarpMat = estimateRigidTransform(tmpFoodPair, tmpContourPair, false);

				//get scale from sWarpMat
				double sUpperBound = 1.2;
				double sLowerBound = 0.8;
				double scale = 1.0;

				if (sWarpMat.size() != Size(0, 0))
				{
					double scale = pow(sWarpMat.at<double>(0, 0), 2) + pow(sWarpMat.at<double>(1, 0), 2);
					if (scale >= sUpperBound)
						scale = sUpperBound;
					else if (scale <= sLowerBound)
						scale = sLowerBound;
					else
						scale = scale;
				}
				//multiply warping matrix with scale
				finalMat = sqrt(scale)*finalMat;
				if (_state == 0) {
					dood = _totalFoodImg[_totalFood[_fIndex].fileName].clone();
					if (_isReverse)
						flip(dood, dood, 0);
					warpAffine(dood, dood, finalMat, dood.size());
				}

				for (int w = 0; w < _pointSeq2.size(); w++)
				{
					double newX = finalMat.at<double>(0, 0)*_pointSeq2[w].x + finalMat.at<double>(0, 1)*_pointSeq2[w].y + finalMat.at<double>(0, 2);
					double newY = finalMat.at<double>(1, 0)*_pointSeq2[w].x + finalMat.at<double>(1, 1)*_pointSeq2[w].y + finalMat.at<double>(1, 2);
					_pointSeq2[w].x = (int)newX;
					_pointSeq2[w].y = (int)newY;
				}
				imageOverlap(_pointSeq2);
				frag fragMax;
				fragMax.setInfo(maxR, maxQ, maxVal, _mapScore.at<double>(maxR, maxQ), _cIndex, _fIndex, finalMat.clone(), dood.clone());
				fragMax.setError(0, 0, 0, _iError, _ratio1);
				_frag2.push_back(fragMax);
				break;
			}
			else
				_mapRQ.at<int>(maxR, maxQ) = 0;

		}
	}
}


void comp::gridCutFood()
{

	typedef GridGraph_2D_4C<short, short, int> Grid;

	Mat food = _totalFoodImg[_totalFood[_fIndex].fileName].clone();
	Mat draw = Mat::zeros(food.size(), CV_8UC4);

	//target contour
	Mat tmp = Mat::zeros(food.size(), CV_8UC4);

	drawContours(draw, vector<vector<Point>>(1, _pointSeq1), 0, Scalar(255, 0, 0, 255), 2, 8);
	drawContours(tmp, vector<vector<Point>>(1, _pointSeq1), 0, Scalar(255, 0, 0, 255), 2, 8);

	//do dilate with first contour
	Mat element = getStructuringElement(MORPH_RECT, Size(30, 30));
	Mat tmp2 = Mat::zeros(tmp.size(), CV_8UC4);
	dilate(tmp, tmp2, element);

	//get contour after dilate
	vector<Mat> sp;

	split(tmp2, sp);

	vector<vector<Point>> tmpC;
	vector<Vec4i> hi;
	findContours(sp[3], tmpC, hi, CV_RETR_TREE, CV_CHAIN_APPROX_NONE);

	drawContours(draw, tmpC, 0, Scalar(0, 0, 255, 255), 5, 8);


	int minX = food.cols;
	int minY = food.rows;
	int maxX = 0;
	int maxY = 0;
	for (int i = 0; i < tmpC.size(); i++)
	{
		for (int j = 0; j < tmpC[i].size(); j++)
		{
			if (tmpC[i][j].x < minX)
				minX = tmpC[i][j].x;
			if (tmpC[i][j].y < minY)
				minY = tmpC[i][j].y;
			if (tmpC[i][j].x > maxX)
				maxX = tmpC[i][j].x;
			if (tmpC[i][j].y > maxY)
				maxY = tmpC[i][j].y;
		}
	}

	cout << minX << " " << minY << " " << maxX << " " << maxY << endl;

	Mat input2(food, Rect(minX, minY, maxX - minX, maxY - minY));
	Mat draw2(draw, Rect(minX, minY, maxX - minX, maxY - minY));
	//build graph

	Grid* grid = new Grid(input2.cols, input2.rows);

	int max = 0;


	for (int i = 0; i < input2.cols; i++)
	{
		for (int j = 0; j < input2.rows; j++)
		{
			grid->set_terminal_cap(grid->node_id(i, j), draw2.at<Vec4b>(j, i) == Vec4b(255, 0, 0, 255) ? 1000 : 0, draw2.at<Vec4b>(j, i) == Vec4b(0, 0, 255, 255) ? 1000 : 0);
			Vec4b s1 = input2.at<Vec4b>(j, i);
			if (i < input2.cols - 1)
			{
				Vec4b s2 = input2.at<Vec4b>(j, i + 1);
				const short cap = (abs(s1[0] - s2[0]) + abs(s1[1] - s2[1]) + abs(s1[2] - s2[2])) / 3;
				grid->set_neighbor_cap(grid->node_id(i, j), 1, 0, cap);
				grid->set_neighbor_cap(grid->node_id(i + 1, j), -1, 0, cap);
			}
			if (j < input2.rows - 1)
			{
				Vec4b s2 = input2.at<Vec4b>(j + 1, i);
				const short cap = (abs(s1[0] - s2[0]) + abs(s1[1] - s2[1]) + abs(s1[2] - s2[2])) / 3;
				grid->set_neighbor_cap(grid->node_id(i, j), 0, +1, cap);
				grid->set_neighbor_cap(grid->node_id(i, j + 1), 0, -1, cap);
			}
		}
	}

	//compute max flow and segmentation
	grid->compute_maxflow();
	Mat output2 = Mat::zeros(input2.size(), CV_8UC4);

	for (int i = 0; i < output2.cols; i++)
	{
		for (int j = 0; j < output2.rows; j++)
		{
			Vec4b o = input2.at<Vec4b>(j, i);
			Vec4b p = Vec4b(0, 0, 0, 0);
			output2.at<Vec4b>(j, i) = (grid->get_segment(grid->node_id(i, j)) ? p : o);
		}
	}

	Mat output = Mat::zeros(food.size(), CV_8UC4);

	for (int i = 0; i < output2.cols; i++)
	{
		for (int j = 0; j < output2.rows; j++)
		{
			output.at<Vec4b>(j + minY, i + minX) = output2.at<Vec4b>(j, i);
		}
	}

	//imwrite("Result/" + to_string(_cIndex) + "s.png", draw);
	//imwrite("Result/" + to_string(_cIndex) + "o.png", output);
	//set fragment
	imageOverlap(tmpC[0]);
	frag fragMax;
	fragMax.setInfo(-1, -1, -1, 0.0, _cIndex, _fIndex, output.clone(), output.clone());
	fragMax.setError(0, 0, 0, _iError, _ratio1);
	_frag2.push_back(fragMax);
}

bool comp::isGroup()
{
	return _isGroup;
}

// return a set of fragment
vector<map<string, int> > comp::fragList()
{
	return _frag;
}

// return a list of fragment with fragment.h
vector<frag> comp::fragList2()
{
	return _frag2;
}

// return a list of fragment with fragment.h
vector<frag> comp::fragList3()
{
	return _frag3;
}

// get sub point sequence
vector<Point> comp::subPointSeq(vector<Point> inputSeq, int startIndex, int matchL)
{
	vector<Point> result;

	for (int i = 0; i < matchL; i++)
	{
		result.push_back(inputSeq[(startIndex + i) % inputSeq.size()]);
	}
	return result;
}

bool comp::isExist(Point idx, vector<Point> &vec) {
	for (int i = 0; i < vec.size(); i++) {
		if (vec[i].x == idx.x) {
			return true;
		}
	}
	return false;
}

void comp::imageOverlap(vector<Point> &contourPoint)
{
	clock_t t1, t2;
	t1 = clock();

	/// Calculate the distances to the contour
	Mat contour_dist(_inputSize, CV_32FC1);
	Mat food_dist(_inputSize, CV_32FC1);
	Mat drawing0 = Mat::zeros(_inputSize, CV_8UC3);
	int contourArea = 0, foodArea = 0, overlapArea = 0;

	int strideSize = 5;
	for (int i = 0; i < _inputSize.width; i += strideSize) {
		for (int j = 0; j < _inputSize.height; j += strideSize) {
			contour_dist.at<float>(j, i) = pointPolygonTest(_pointSeq1, Point2f(i, j), true);
			food_dist.at<float>(j, i) = pointPolygonTest(contourPoint, Point2f(i, j), true);

			int same = 0;
			//calculate contour area
			if (contour_dist.at<float>(j, i) > 0) {
				same++;
				contourArea++;
				drawing0.at<Vec3b>(j, i)[2] = 255;
			}
			if (food_dist.at<float>(j, i) > 0) {
				same++;
				foodArea++;
				drawing0.at<Vec3b>(j, i)[1] = 255;
			}
			if (same == 2) {
				overlapArea++;
			}
		}
	}

	double ratio = (double)overlapArea / (double)(contourArea + foodArea - overlapArea);
	double ratio1 = (double)overlapArea / (double)contourArea; // contour 
	double ratio2 = (double)overlapArea / (double)foodArea; // food

	_ratio1 = ratio1;
	_ratio2 = ratio2;
	_iError = (1 - ratio);
}