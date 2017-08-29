#include "QtGuiApplication1.h"
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

#include <QApplication>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDirModel>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QStyleOptionViewItem>
#include <QtWidgets/QSlider>
#include <QSignalMapper>
#include <QPixmap>
#include <QPainter>
#include <QToolButton>

#include "tree.hh"
#include "descri.h"
#include "compare.h"
#include "fragment.h"
#include "recipe.h"
#include "tpSort.h"
#include "food.h"
# define PI 3.1415926

using namespace std;
using namespace cv;


struct cfMap
{
	map<int, fragList> Element;
};

struct leafNode
{
	Mat image;
	double originError;
	double totalError;
	vector<int> nutrientVec;
};
vector<leafNode> leafNodeVec;

/* Global Parameters */
vector<vector<Mat> > desOfFood, desOfFoodReverse;
vector<vector<Point> > samplepointsOfFood, samplepointsOfFoodReverse;

Size inputSize(500, 500);
Point topLeft, bottomRight;
vector<Mat> userDrawSet;
Mat selectUserDraw;
bool selectImageOrNot = false;
vector<vector<Point>> disjointContour;
vector<vector<Mat> > desOfDraw, desOfDrawReverse;
vector<vector<Point> > samplepointsOfDraw, samplepointsOfDrawReverse;

int topLabelSize = 300;
int listWidth = 100;
int topAmount = 5;
int vecSeqIndexAmount = 2;

QColor select_Color(255, 253, 234, 200);
QString select_Color_Str("255, 253, 234, 200");
QColor selectDisable_Color(255, 253, 234, 0);
QString selectDisable_Color_Str("255, 253, 234, 0");

QString white_Color_Str("255, 255, 255, 100");
QString blue_Color_Str("127, 230, 255, 220");
QString darkBlue_Color_Str("127, 230, 255, 255");

vector<QString> recipeSelectedIndex;
vector<food> foodSelectedIndex;
int initialAmount = 10;
vector<int> currentFoodTableOrder{ 0, 1, 2, 3 }; // delete, foodMat, amount, slider
vector<int> foodSelectedIndex_Amount;

int nutrient_ratio = 50;
vector<string> nutrient_Str{ "Cal", "Pr", "Ca", "Fe", "B1", "B2", "C", "E" };
vector<int> nutrientValue(nutrient_Str.size(), 50);
vector<int> nutrientValue_show(nutrient_Str.size(), 50);
vector<double> nutrientDay{ 2100, 50, 700, 15, 1.1, 1.2, 50, 10 }; // maxCal maxProtein maxCalcium maxIron maxVitaminB1 maxVitaminB2 maxVitaminC maxVitaminE
QString nutrient_transparent = "200";
int nutrient_transparentI = 255;
vector<QString> nutrient_Color_Str{
	"(208, 16, 76, " + nutrient_transparent + ")",
	"(204, 84, 58, " + nutrient_transparent + ")",
	"(88, 178, 220, " + nutrient_transparent + ")",
	"(148, 122, 109, " + nutrient_transparent + ")",
	"(255, 177, 27, " + nutrient_transparent + ")",
	"(233, 233, 43, " + nutrient_transparent + ")",
	"(134, 193, 102, " + nutrient_transparent + ")",
	"(119, 66, 141, " + nutrient_transparent + ")" };
vector<QColor> nutrient_Color{
	QColor(208, 16, 76, nutrient_transparentI),
	QColor(204, 84, 58, nutrient_transparentI),
	QColor(88, 178, 220, nutrient_transparentI),
	QColor(148, 122, 109, nutrient_transparentI),
	QColor(255, 177, 27, nutrient_transparentI),
	QColor(233, 233, 43, nutrient_transparentI),
	QColor(134, 193, 102, nutrient_transparentI),
	QColor(119, 66, 141, nutrient_transparentI) };

string recipeDir("inputRecipe3/");
vector<string> recipeFileName;

string preDataDir("preData_500/");

string foodPreDir("bin500/");
string categoryImgDir("CategoryImg/");

vector<food> totalFood;

vector<string> category;
map<string, vector<food> > cateFood;
map<string, vector<food> > cateRecipe;
map<string, Mat> totalFoodImg;
map<string, Mat> modifyFoodImg;

// grouping
vector<vector<int> > groupIdx;
vector<int> groupIdxOfContour;
vector<Mat> groupWarpMatOfContour;

/* Main Function */
void prePocessOfUserDraw(Mat& userDraw);

/* Build Tree Function */
vector<string> split_str(string s, char ch);
void appendAllFrag(int state, tree<string>& tr, string c_str, int nextIndex, vector<fragList>& sortedFragList);
bool hasGroup(string str, int currentIndex, int fragIndex, vector<fragList>& sortedFragList, vector<int>& groupIdxOfContour);
Mat stackWithTopo(string str, vector<fragList>& sortedFragList);
double getHealthError(vector<int> nutrient);
vector<int> getStrNutrient(string str, vector<fragList>& sortedFragList);


/* Food Combination Function */
vector<vector<Point>> getNewContour(vector<Point> &contourPoint, Mat warpImg);
int imageOverlap(int a, int b, Mat& resultImg);
bool foodCombination(int contourIdx, int foodImgIdx, int& recipe_num_tmp, int recipe_num_contour, int recipe_num, vector<Point>& c_new_contourPoint, descri& c_descriUser, Mat& resultStack, recipe& rec2);

/* Pre-Processing */
void preProcess(string s_file, vector<vector<Point> >& samplePoint, string p_file, vector<vector<Mat> >& descriptor, string d_file, string d_file2, string d_file3, string d_file4);
vector<Mat> vecmatread(const string& filename); //store food descriptor
vector<Point> vecPointRead(const string& filename); //store food samplePoint
void getAllRecipe(string recipeDir, vector<string> &files, vector<recipe>& RecipeAll);
void readFoodPreData(vector<food>& totalFood);
void readBinaryImg(vector<Mat>& totalImg, string fileName);

/* Image Processing */
void grouping(vector<vector<int> >& groupIdx, vector<int>& groupIdxOfContour, vector<Mat>& groupWarpMatOfContour, vector<Vec4b>& colorOfContour, vector<Point>& centerOfContour);
void reversePoint(Point center, vector<Point>& contourPoint); //reverse contour with center
void getBorder(Mat &image, int& top, int& left, int& bottom, int& right);
Mat addTransparent(Mat &bg, Mat &fg); //add image with transparent background
vector<Point> subPointSeq(vector<Point> inputSeq, int startIndex, int range); //get subPointSeq
Mat cannyThreeCh(Mat input, bool mode); //three channels Canny
int getdir(string dir, vector<string> &files); //get all files in the dir
Mat alphaBinary(Mat input); //segment image with alpha value
void scaleIconImg(vector<Mat> tmpFoodImg); // scale img icon
void vecSeqIndex(fragList& input, int newAmount); //put fragment in order

/* Error Function */
double edgeError(Mat draw, Mat food); //edge error
double colorError(Mat draw, Mat food); //color errorrefError
double refError(Mat draw, Mat food); //reference error
double interError(Mat draw, Mat food); //intersection error
double iContourError(vector<Point> contour1, vector<Point> contour2, Mat food); //iError with two contour

/* Comparison Value Function */
bool compareContourSize(vector<Point> contour1, vector<Point> contour2);
bool compareWithCertainKey(frag input1, frag input2);
bool compareWithTotalError(leafNode input1, leafNode input2);

/* Comparison Contour Function */
void doCompare(bool isReverse, fragList& pairSeq, Mat& userDraw, vector<vector<Mat> >& descriptor, vector<vector<Point> >& sampleP, int contourIdx, int recipeIdx, recipe& rec);

/* --------------------------------UI Function-------------------------------- */
void removeFileExtension(vector<string>& fileName) {
	for (int i = 0; i < fileName.size(); i++) {
		vector<string> strs = split_str(fileName[i], '.');
		fileName[i] = strs[0];
	}
}

void initTab(QTabWidget *tab) {
	//tab->setStyleSheet(QString("QTabBar::tab { height: 45px; width: %1px; } ").arg(s_width / tab->count()) + "QTabWidget{ background-color: transparent; font-size: " + QString::number(18) + "pt; font: bold large 'Times New Roman';}"
	tab->setStyleSheet("QTabBar::tab { height: 45px; } "
		"QTabWidget{ background-color: transparent; font-size: " + QString::number(18) + "pt; font: bold large 'Times New Roman';}"
		"QTabBar::tab:selected { background-color: rgba(" + blue_Color_Str + "); }"
		"QTabBar::tab-bar { border: none; left: 5px; }"
		//"QTabWidget::pane{ border-top: 2px solid; background-color: rgba(" + white_Color_Str + ");}");
		"QTabWidget::pane{ border-top: 1px solid; background-color: rgba(" + white_Color_Str + ");}");
}

void initTree(QTreeWidget *tree) {
	tree->header()->close();
	tree->setIconSize(QSize(70, 70));
	tree->setExpandsOnDoubleClick(false);
	tree->setStyleSheet("QTreeWidget { border: none; font-size: " + QString::number(16) + "pt; font: bold large 'Times New Roman'; background-color: rgba(" + selectDisable_Color_Str + ");  show-decoration-selected: 1;}"
		// https://www.ics.com/files/qtdocs/stylesheet-examples.html
		//"QTreeView::branch:open:has-children{ background-color:rgba(" + white_Color_Str + "); border-image: url(indicator_open.png) 0; }"
		"QTreeView::branch:open:has-children{ border-image: url(indicator_open.png) 0; }"
		"QTreeView::branch:closed:has-children{ border-image: url(indicator_close.png) 0; }"
		);
}

void initLabel(QLabel *label) {
	label->setAlignment(Qt::AlignCenter);
	label->setStyleSheet("background-color: rgba(" + selectDisable_Color_Str + "); QLabelView { border: none; }");
	label->setScaledContents(true);
	label->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void initTable(QTableWidget *table) {
	table->setShowGrid(false);
	table->horizontalHeader()->hide();
	table->verticalHeader()->hide();
	table->resizeRowsToContents();
}

void initNutrientTable(QTableWidget *table) {
	table->setStyleSheet("QTableWidget {border: none; background-color: rgba(" + white_Color_Str + ");}"
		"QTableWidget::item {padding: 5px;}"
		"QHeaderView::section {background-color: transparent;}"
		"QHeaderView {background-color: transparent;}"
		"QTableCornerButton::section {background-color: transparent;} ");
	table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	table->horizontalHeader()->setStretchLastSection(true);
	table->setRowCount(nutrient_Str.size());
	table->resizeRowsToContents();
	table->setColumnCount(3);
	//table->setColumnWidth(0, 50);
	//table->setColumnWidth(1, 50);

	for (int i = 0; i < nutrient_Str.size(); i++) {
		QTableWidgetItem *qItemName = new QTableWidgetItem(QString::fromStdString(nutrient_Str[i]));
		qItemName->setTextAlignment(Qt::AlignCenter);
		table->setItem(i, 0, qItemName);
		//table->item(i, 0)->setTextColor(nutrient_Color[i]);

		QFont fnt;
		fnt.setPointSize(12);
		table->setItem(i, 1, new QTableWidgetItem(QString::fromStdString(to_string(nutrientValue[i]) + "%")));
		table->item(i, 1)->setFont(fnt);
	}
	table->resizeColumnsToContents();
}

/* --------------------------------Slot Function-------------------------------- */
/* Repaint current food table */
void QtGuiApplication1::updateShowTable(QTableWidget *table) {
	table->clearContents();
	table->setRowCount(foodSelectedIndex.size());
	table->setStyleSheet("QTableWidget {border: none; background-color: rgba(" + white_Color_Str + ");}"
		"QTableWidget::item {background-color: rgba(" + select_Color_Str + "); padding: 5px;}"
		"QHeaderView::section {background-color: transparent;}"
		"QHeaderView {background-color: transparent;}"
		"QTableCornerButton::section {background-color: transparent;} ");

	/*add stuff inside the table view*/
	for (int i = 0; i < foodSelectedIndex.size(); i++) {
		QPixmap pixelImage_delete = QPixmap::fromImage(QImage("delete.png"));
		QPixmap resPix_delete = pixelImage_delete.scaled(30, 30, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		//QPixmap pixelImage_delete("delete.png");
		QTableWidgetItem *pCell_delete = new QTableWidgetItem();
		pCell_delete->setData(Qt::DecorationRole, resPix_delete);
		table->setItem(i, currentFoodTableOrder[0], pCell_delete);

		QPixmap pixelImage = QPixmap::fromImage(QImage((uchar*)modifyFoodImg[foodSelectedIndex[i].fileName].data, modifyFoodImg[foodSelectedIndex[i].fileName].cols, modifyFoodImg[foodSelectedIndex[i].fileName].rows, QImage::Format_ARGB32));
		QPixmap resPix = pixelImage.scaled(75, 75, Qt::KeepAspectRatio, Qt::SmoothTransformation);
		QTableWidgetItem *pCell = new QTableWidgetItem();
		pCell->setData(Qt::DecorationRole, resPix);
		table->setItem(i, currentFoodTableOrder[1], pCell);

		QSlider *sliderTmp = new QSlider(Qt::Horizontal, table);
		sliderTmp->setMinimum(0);
		sliderTmp->setMaximum(20);
		sliderTmp->setValue(foodSelectedIndex_Amount[i]);
		sliderTmp->setStyleSheet(
			"QSlider::groove:horizontal { border: 1px solid #999999; height: 8px; background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #B1B1B1, stop:1 #c4c4c4); }"
			"QSlider::handle:horizontal{ width: 5px; background: #69c2ef; border: 2px solid #004370; margin: -8px 0; border-radius: 15px;}"
			"QSlider::sub-page:horizontal{	background: #69c2ef; }"
		);
		table->setCellWidget(i, currentFoodTableOrder[3], sliderTmp);

		QTableWidgetItem *qItem = new QTableWidgetItem(QString::number(sliderTmp->value()));
		qItem->setTextAlignment(Qt::AlignCenter);
		table->setItem(i, currentFoodTableOrder[2], qItem);
		QSignalMapper* signalMapper = new QSignalMapper(this);
		connect(sliderTmp, SIGNAL(valueChanged(int)), signalMapper, SLOT(map()));
		signalMapper->setMapping(sliderTmp, i);
		connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(changeAmount_Food(int)));
	}
}

/* Recipe TreeItem Clicked Function */
void setColor_Recipe(QTreeWidget* tree1, QTreeWidget* tree2, food& tmpF, QBrush color) {
	for (int i = 0; i < tree1->topLevelItemCount(); i++) {
		QTreeWidgetItem *item = tree1->topLevelItem(i);
		for (int j = 0; j < item->childCount(); j++) {
			food tmpF2 = cateRecipe[(item->text(0)).toStdString()][j];
			if (tmpF2 == tmpF) {
				item->child(j)->setBackground(0, color);
			}
		}
	}
	for (int i = 0; i < tree2->topLevelItemCount(); i++) {
		QTreeWidgetItem *item = tree2->topLevelItem(i);
		for (int j = 0; j < item->childCount(); j++) {
			food tmpF2 = cateFood[(item->text(0)).toStdString()][j];
			if (tmpF2 == tmpF) {
				item->child(j)->setBackground(0, color);
			}
		}
	}
}

void QtGuiApplication1::QTreeWidgetClicked_Recipe(QTreeWidgetItem* item, int column) {
	if (item->parent()) {
		int currentIdx = item->parent()->indexOfChild(item);
		bool storeOrNot = true;

		food tmpF = cateRecipe[(item->parent()->text(0)).toStdString()][currentIdx];
		for (int i = 0; i < foodSelectedIndex.size(); i++) {
			if (tmpF == foodSelectedIndex[i]) {
				storeOrNot = false;
				item->setBackground(0, QBrush(selectDisable_Color));
				setColor_Recipe(ui.treeWidget, ui.treeWidget_2, tmpF, QBrush(selectDisable_Color));
				foodSelectedIndex.erase(foodSelectedIndex.begin() + i);
				foodSelectedIndex_Amount.erase(foodSelectedIndex_Amount.begin() + i);
				break;
			}
		}

		if (storeOrNot) {
			item->setBackground(0, QBrush(select_Color));
			setColor_Recipe(ui.treeWidget, ui.treeWidget_2, tmpF, QBrush(select_Color));
			foodSelectedIndex.push_back(tmpF);
			foodSelectedIndex_Amount.push_back(initialAmount);
		}
	}
	else {
		bool hasRecipe = false;
		for (int i = 0; i < recipeSelectedIndex.size(); i++) {
			if (item->text(0) == recipeSelectedIndex[i]) {
				hasRecipe = true;
				recipeSelectedIndex.erase(recipeSelectedIndex.begin() + i);
				break;
			}
		}

		// if user cancel to select the whole recipe
		if (hasRecipe) {
			item->setBackground(0, QBrush(selectDisable_Color));
			item->setExpanded(false);

			for (int i = 0; i < item->childCount(); i++) {
				food tmpF = cateRecipe[(item->text(0)).toStdString()][i];
				for (int j = 0; j < foodSelectedIndex.size(); j++) {
					if (tmpF == foodSelectedIndex[j]) {
						item->child(i)->setBackground(0, QBrush(selectDisable_Color));
						setColor_Recipe(ui.treeWidget, ui.treeWidget_2, tmpF, QBrush(selectDisable_Color));
						foodSelectedIndex.erase(foodSelectedIndex.begin() + j);
						foodSelectedIndex_Amount.erase(foodSelectedIndex_Amount.begin() + j);
						break;
					}
				}
			}
		}
		// if user select the whole recipe
		else {
			item->setBackground(0, QBrush(select_Color));
			item->setExpanded(true);
			recipeSelectedIndex.push_back(item->text(0));

			for (int i = 0; i < item->childCount(); i++) {
				food tmpF = cateRecipe[(item->text(0)).toStdString()][i];
				bool storeOrNot = true;
				for (int j = 0; j < foodSelectedIndex.size(); j++) {
					if (tmpF == foodSelectedIndex[j]) {
						storeOrNot = false;
						break;
					}
				}
				if (storeOrNot) {
					item->child(i)->setBackground(0, QBrush(select_Color));
					setColor_Recipe(ui.treeWidget, ui.treeWidget_2, tmpF, QBrush(select_Color));
					foodSelectedIndex.push_back(tmpF);
					foodSelectedIndex_Amount.push_back(initialAmount);
				}
			}
		}
	}
	updateShowTable(ui.tableWidget);
}

/* Food TreeItem Clicked Function */
void setColor_Food(QTreeWidget* tree1, food& tmpF, QBrush color) {
	for (int i = 0; i < tree1->topLevelItemCount(); i++) {
		QTreeWidgetItem *item = tree1->topLevelItem(i);
		for (int j = 0; j < item->childCount(); j++) {
			food tmpF2 = cateRecipe[(item->text(0)).toStdString()][j];
			if (tmpF2 == tmpF) {
				item->child(j)->setBackground(0, color);
			}
		}
	}
}

void QtGuiApplication1::QTreeWidgetClicked_Food(QTreeWidgetItem* item, int column) {

	if (item->parent()) {
		int currentIdx = item->parent()->indexOfChild(item);
		bool storeOrNot = true;

		food tmpF = cateFood[(item->parent()->text(0)).toStdString()][currentIdx];
		for (int i = 0; i < foodSelectedIndex.size(); i++) {
			if (tmpF == foodSelectedIndex[i]) {
				storeOrNot = false;
				item->setBackground(0, QBrush(selectDisable_Color));
				setColor_Food(ui.treeWidget, tmpF, QBrush(selectDisable_Color));
				foodSelectedIndex.erase(foodSelectedIndex.begin() + i);
				foodSelectedIndex_Amount.erase(foodSelectedIndex_Amount.begin() + i);
				break;
			}
		}

		if (storeOrNot) {
			item->setBackground(0, QBrush(select_Color));
			setColor_Food(ui.treeWidget, tmpF, QBrush(select_Color));
			foodSelectedIndex.push_back(tmpF);
			foodSelectedIndex_Amount.push_back(initialAmount);
			nutrientValue_show[0] = tmpF.cal;
			nutrientValue_show[1] = tmpF.protein;
			nutrientValue_show[2] = tmpF.calcium;
			nutrientValue_show[3] = tmpF.iron;
			nutrientValue_show[4] = tmpF.vitaminB1;
			nutrientValue_show[5] = tmpF.vitaminB2;
			nutrientValue_show[6] = tmpF.vitaminC;
			nutrientValue_show[7] = tmpF.vitaminE;
			showAmount_Nutrient();
		}
	}
	else {
		item->setExpanded(!item->isExpanded());

		///*
		// if user select the whole food
		item->setBackground(0, QBrush(select_Color));
		item->setExpanded(true);
		recipeSelectedIndex.push_back(item->text(0));

		for (int i = 0; i < item->childCount(); i++) {
			food tmpF = cateFood[(item->text(0)).toStdString()][i];
			bool storeOrNot = true;
			for (int j = 0; j < foodSelectedIndex.size(); j++) {
				if (tmpF == foodSelectedIndex[j]) {
					storeOrNot = false;
					break;
				}
			}
			if (storeOrNot) {
				item->child(i)->setBackground(0, QBrush(select_Color));
				setColor_Food(ui.treeWidget, tmpF, QBrush(select_Color));
				foodSelectedIndex.push_back(tmpF);
				foodSelectedIndex_Amount.push_back(initialAmount);
			}
		}
		//*/
	}
	updateShowTable(ui.tableWidget);
}

/* Change the big image on table when click anither index */
void QtGuiApplication1::ResultIndexClicked(QTableWidgetItem* item) {
	int currentColumn = item->column(); 
	Mat currentImage = leafNodeVec[currentColumn].image.clone();
	QPixmap pix = QPixmap::fromImage(QImage((uchar*)currentImage.data, currentImage.cols, currentImage.rows, QImage::Format_ARGB32));
	QPixmap resPix = pix.scaled(topLabelSize, topLabelSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	ui.label_5->setPixmap(resPix);
	updateNutrientSlider(leafNodeVec[currentColumn].nutrientVec);
	cout << "totalError= " << leafNodeVec[currentColumn].totalError << endl;
}

/* Remove new food item into ui.tableWidget */
void QtGuiApplication1::removeFoodItem(int currentRow) {
	food tmpF = foodSelectedIndex[currentRow];
	setColor_Recipe(ui.treeWidget, ui.treeWidget_2, tmpF, QBrush(selectDisable_Color));
	foodSelectedIndex.erase(foodSelectedIndex.begin() + currentRow);
	foodSelectedIndex_Amount.erase(foodSelectedIndex_Amount.begin() + currentRow);
	updateShowTable(ui.tableWidget);
}

/* Remove new food item into ui.tableWidget */
void QtGuiApplication1::removeFoodItem2(QTableWidgetItem* item) {
	int currentRow = item->row();
	food tmpF = foodSelectedIndex[currentRow];
	if (item->column() == currentFoodTableOrder[0]) {
		setColor_Recipe(ui.treeWidget, ui.treeWidget_2, tmpF, QBrush(selectDisable_Color));
		foodSelectedIndex.erase(foodSelectedIndex.begin() + currentRow);
		foodSelectedIndex_Amount.erase(foodSelectedIndex_Amount.begin() + currentRow);
		updateShowTable(ui.tableWidget);
	}
	else {
		nutrientValue_show[0] = tmpF.cal;
		nutrientValue_show[1] = tmpF.protein;
		nutrientValue_show[2] = tmpF.calcium;
		nutrientValue_show[3] = tmpF.iron;
		nutrientValue_show[4] = tmpF.vitaminB1;
		nutrientValue_show[5] = tmpF.vitaminB2;
		nutrientValue_show[6] = tmpF.vitaminC;
		nutrientValue_show[7] = tmpF.vitaminE;
		showAmount_Nutrient();
	}
}

/* Change food amount in ui.tableWidget */
void QtGuiApplication1::changeAmount_Food(int currentRow) {
	QSignalMapper* signalMapper = qobject_cast<QSignalMapper*>(sender());
	QSlider *sliderTmp = qobject_cast<QSlider*>(signalMapper->mapping(currentRow));
	QTableWidgetItem *qItem = new QTableWidgetItem(QString::number(sliderTmp->value()));
	qItem->setTextAlignment(Qt::AlignCenter);
	ui.tableWidget->item(currentRow, currentFoodTableOrder[2])->setText(QString::number(sliderTmp->value()));
	foodSelectedIndex_Amount[currentRow] = sliderTmp->value();
}

/* Change nutrient value in ui.tableWidget_2 */
void QtGuiApplication1::changeAmount_Nutrient(int currentRow) {
	QSignalMapper* signalMapper = qobject_cast<QSignalMapper*>(sender());
	QSlider *sliderTmp = qobject_cast<QSlider*>(signalMapper->mapping(currentRow));
	ui.tableWidget_2->item(currentRow, 1)->setText(QString::fromStdString(to_string(nutrientValue[currentRow]) + "%"));
	nutrientValue[currentRow] = sliderTmp->value();

	if (leafNodeVec.size() > 0) {
		// sort with totalError(originError+hErr)
		for (int i = 0; i < leafNodeVec.size(); i++) {
			double hErr = getHealthError(leafNodeVec[i].nutrientVec);
			leafNodeVec[i].totalError = (leafNodeVec[i].originError*(100 - nutrient_ratio) + hErr*nutrient_ratio)/100;
		}
		updateResultTable();
	}
}

/* Update nutrient value in ui.tableWidget_2 */
void  updateNutrientShowTable(QTableWidget *table) {
	//double percentCal = curCal / maxCal;
	//double percentProtein = curProtein / maxProtein;
	//double percentCalaium = curCalcium / maxCalcium;
	//double percentIron = curIron / maxIron;
	//double percentVitaminB1 = curVitaminB1 / maxVitaminB1;
	//double percentVitaminB2 = curVitaminB2 / maxVitaminB2;
	//double percentVitaminC = curVitaminC / maxVitaminC;
	//double percentVitaminE = curVitaminE / maxVitaminE;

	for (int i = 0; i < nutrientValue_show.size(); i++) {
		int tmpInt = 100 * nutrientValue_show[i] / nutrientDay[i];
		table->item(i, 1)->setText(QString::fromStdString(to_string(tmpInt) + "%"));

		QSlider *sliderTmp = new QSlider(Qt::Horizontal, table);
		sliderTmp->setMinimum(0);
		sliderTmp->setMaximum(100);
		sliderTmp->setValue(100 * nutrientValue_show[i] / nutrientDay[i]);
		sliderTmp->setStyleSheet(
			"QSlider::groove:horizontal { border: 1px solid #999999; height: 15px; background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #B1B1B1, stop:1 #c4c4c4); }"
			"QSlider::handle:horizontal { width: 1px; background: rgba" + nutrient_Color_Str[i] + "; border: 0px solid rgba" + nutrient_Color_Str[i] + ";}"
			"QSlider::handle:vertical { height: 1px; background: rgba" + nutrient_Color_Str[i] + "; border: 0px solid rgba" + nutrient_Color_Str[i] + ";}"
			"QSlider::sub-page:horizontal { background: rgba" + nutrient_Color_Str[i] + "; }"
			);
		//sliderTmp->setValue(100);
		sliderTmp->setDisabled(true);
		table->setCellWidget(i, 2, sliderTmp);
	}
}

/* show nutrient value in ui.tableWidget_3 */
void QtGuiApplication1::showAmount_Nutrient() {
	updateNutrientShowTable(ui.tableWidget_3);
}

/* change ratio */
void QtGuiApplication1::sliderRatioChange(int value) {
	nutrient_ratio = value;
	if (leafNodeVec.size() > 0) {
		// sort with totalError(originError+hErr)
		for (int i = 0; i < leafNodeVec.size(); i++) {
			double hErr = getHealthError(leafNodeVec[i].nutrientVec);
			leafNodeVec[i].totalError = (leafNodeVec[i].originError*(100 - nutrient_ratio) + hErr*nutrient_ratio) / 100;
		}
		updateResultTable();
	}
}

/* Apply Button Function */
void QtGuiApplication1::applyClicked() {
	if (selectImageOrNot) {
		ui.tableWidget_6->clearContents();
		prePocessOfUserDraw(selectUserDraw);

		int tmpIdx;
		if (foodSelectedIndex.size() > 0) {
			recipe rec;
			for (int j = 0; j < foodSelectedIndex.size(); j++) {
				for (int k = 0; k < totalFood.size(); k++)
				{
					if (foodSelectedIndex[j] == totalFood[k])
					{
						tmpIdx = k;
						break;
					}
				}
				rec.addFood(tmpIdx, foodSelectedIndex_Amount[j], 0);
			}
			rec.print();
			compareWithRecipe(selectUserDraw, rec);
		}
	}

	cout << "------------------------------------------------- finish" << endl;
}

/* Add new file Function */
void QtGuiApplication1::openImageFile() {
	selectImageOrNot = true;

	QFileDialog *fileDialog = new QFileDialog(this);
	fileDialog->setWindowTitle(tr("Open Image"));
	fileDialog->setDirectory(".");

	if (fileDialog->exec() == QDialog::Accepted) {
		QString path = fileDialog->selectedFiles()[0];
		string tmpStr = split_str(path.toStdString(), '/').back();
		vector<string> tmpStrVec2 = split_str(tmpStr, '.');

		if (tmpStrVec2[1] == "png") {
			Mat userDraw = imread(path.toStdString(), -1);
			cv::resize(userDraw, userDraw, inputSize);


			int tmp_top, tmp_left, tmp_width, tmp_height;
			getBorder(userDraw, tmp_top, tmp_left, tmp_width, tmp_height);
			Mat cutMat = userDraw(Rect(tmp_left, tmp_top, tmp_width, tmp_height));

			double w_scale = (double)userDraw.cols / (double)tmp_width;
			double h_scale = (double)userDraw.rows / (double)tmp_height;
			double small_scale;
			if (w_scale < h_scale)
				small_scale = w_scale;
			else
				small_scale = h_scale;
			small_scale *= 0.7;

			Size newS = Size(cutMat.cols*small_scale, cutMat.rows*small_scale);
			cv::resize(cutMat, cutMat, newS);

			/*
			Size iconSize = Size(userDraw.cols * 0.8, userDraw.rows * 0.8);
			Mat finalMat = Mat::zeros(iconSize, CV_8UC4);
			cutMat.copyTo(finalMat(cv::Rect((finalMat.cols - cutMat.cols) / 2, (finalMat.rows - cutMat.rows) / 2, cutMat.cols, cutMat.rows)));
			*/

			selectUserDraw = userDraw.clone();
			//selectUserDraw = userDraw;

			QPixmap pix = QPixmap::fromImage(QImage((uchar*)userDraw.data, userDraw.cols, userDraw.rows, QImage::Format_ARGB32));
			QPixmap resPix = pix.scaled(topLabelSize, topLabelSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
			ui.label_6->setPixmap(resPix);
		}
	}
	else {
		//ui.bg_label->setText("You didn't select any files.");
	}
}

/* change background */
void QtGuiApplication1::changeBKG() {
	QFileDialog *fileDialog = new QFileDialog(this);
	fileDialog->setWindowTitle(tr("Change background"));
	fileDialog->setDirectory(".");

	if (fileDialog->exec() == QDialog::Accepted) {
		QString path = fileDialog->selectedFiles()[0];
		string tmpStr = split_str(path.toStdString(), '/').back();
		vector<string> tmpStrVec2 = split_str(tmpStr, '.');
		vector<string> typeName{ "bmp", "dib", "jpeg", "jpg", "png", "pbm", "pgm", "ppm", "sr", "ras", "tiff",
			"BMP", "DIB", "JPEG", "JPE", "PNG", "PBM", "PGM", "PPM", "SR", "RAS", "TIFF" };

		bool typeCorrect = false;
		for (int i = 0; i < typeName.size(); i++) {
			if (tmpStrVec2[1] == typeName[i]) {
				typeCorrect = true;
				break;
			}
		}

		if (typeCorrect) {
			QPixmap bkgnd(path);
			bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
			QPalette palette;
			palette.setBrush(QPalette::Background, bkgnd);
			this->setPalette(palette);
		}
	}
	else {
		cout << "You didn't select any BKG files.";
	}
}

/* --------------------------------UI Main Function-------------------------------- */
QtGuiApplication1::QtGuiApplication1(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->showFullScreen();

	ui.horizontalLayout_3->setStretch(0, 3.5);
	ui.horizontalLayout_3->setStretch(1, 6.5);

	ui.horizontalLayout_2->setStretch(0, 5);
	ui.horizontalLayout_2->setStretch(1, 5);

	ui.horizontalLayout_4->setStretch(0, 7);
	ui.horizontalLayout_4->setStretch(1, 3);

	ui.horizontalLayout_5->setStretch(0, 7);
	ui.horizontalLayout_5->setStretch(1, 3);

	ui.verticalLayout_3->setStretch(0, 7);
	ui.verticalLayout_3->setStretch(1, 3);

	ui.verticalLayout->setStretch(0, 1);
	ui.verticalLayout->setStretch(1, 4);
	ui.verticalLayout->setStretch(2, 1);
	ui.verticalLayout->setStretch(3, 3);
	ui.verticalLayout->setStretch(4, 1);

	/* -----------------PreProcessing----------------- */
	// pre-process for contour descriptor and sample points
	//preProcess("foodDesSize.bin", samplepointsOfFood, "foodSample.bin", desOfFood, "foodDesSeq.bin", "foodDesSeq2.bin", "foodDesSeq3.bin", "foodDesSeq4.bin");
	//preProcess("foodDesSize_reverse.bin", samplepointsOfFoodReverse, "foodSample_reverse.bin", desOfFoodReverse, "foodDesSeq_reverse.bin", "foodDesSeq_reverse2.bin", "foodDesSeq_reverse3.bin", "foodDesSeq_reverse4.bin");
	
	readFoodPreData(totalFood);
	category.push_back("Fruits");
	category.push_back("Vegetables");
	category.push_back("Grains");
	category.push_back("Protein_Foods");
	category.push_back("Dairy");
	category.push_back("Oils");
	cout << "foodPredata" << endl;

	vector<Mat> tmpFoodImg;
	readBinaryImg(tmpFoodImg, foodPreDir + "preFoodImgData1.bin");
	readBinaryImg(tmpFoodImg, foodPreDir + "preFoodImgData2.bin");
	readBinaryImg(tmpFoodImg, foodPreDir + "preFoodImgData3.bin");
	readBinaryImg(tmpFoodImg, foodPreDir + "preFoodImgData4.bin");
	for (int i = 0; i < totalFood.size(); i++) {
		totalFoodImg[totalFood[i].fileName] = tmpFoodImg[i];
		//modifyFoodImg[totalFood[i].fileName] = tmpFoodImg[i].clone();
	}
	cout << "image PreData" << endl;
	scaleIconImg(tmpFoodImg);

	/* set background image
	QPixmap bkg_input("foodBKG6.jpg");
	QImage bkg_image(bkg_input.size(), QImage::Format_ARGB32_Premultiplied);
	bkg_image.fill(Qt::transparent);
	QPainter p(&bkg_image);
	p.setOpacity(100);
	p.drawPixmap(0, 0, bkg_input);
	p.end();
	QPixmap bkg_output = QPixmap::fromImage(bkg_image);
	QPixmap bkg_output("foodBKG6.jpg");
	bkg_output = bkg_output.scaled(this->size(), Qt::IgnoreAspectRatio);
	*/
	
	QPalette palette;
	palette.setBrush(QPalette::Background, QColor(234, 252, 255, 200));
	this->setPalette(palette);

	/* get recipe by dir */
	vector<recipe> RecipeFiles;
	//getdir(recipeDir, recipeFileName);
	//getAllRecipe(recipeDir, recipeFileName, RecipeFiles);
	//removeFileExtension(recipeFileName); //remove ".bin"

	/* -----------------UI----------------- */
	/* recipe treeWidget - left */
	initTree(ui.treeWidget);
	QList<QTreeWidgetItem *> rootList;
	for (int i = 0; i < RecipeFiles.size(); i++) {
		QTreeWidgetItem *root = new QTreeWidgetItem(ui.treeWidget, QStringList() << QString::fromStdString(recipeFileName[i]));
		root->setIcon(0, QIcon(QString::fromStdString("inputImg/bear.png"))); // set icon
		root->setSizeHint(0, QSize(50, 80));

		for (int j = 0; j < RecipeFiles[i].size; j++) {
			int foodImgIdx = RecipeFiles[i].foodIndex()[j];
			cateRecipe[recipeFileName[i]].push_back(totalFood[foodImgIdx]);
			QTreeWidgetItem *leaf = new QTreeWidgetItem(root, QStringList() << QString::fromStdString(totalFood[foodImgIdx].name));
			QPixmap pixelImage = QPixmap::fromImage(QImage((uchar*)modifyFoodImg[totalFood[foodImgIdx].fileName].data, modifyFoodImg[totalFood[foodImgIdx].fileName].cols, modifyFoodImg[totalFood[foodImgIdx].fileName].rows, QImage::Format_ARGB32));
			QPixmap resPix = pixelImage.scaled(80, 80);
			leaf->setIcon(0, QIcon(resPix)); // set icon
		}
		rootList << root;
	}
	ui.treeWidget->insertTopLevelItems(0, rootList);  //設定此容器內容為QTreeWidget的根節點
	connect(ui.treeWidget, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(QTreeWidgetClicked_Recipe(QTreeWidgetItem*, int)));

	/* food treeWidget - left, get food by dir */
	initTree(ui.treeWidget_2);
	QList<QTreeWidgetItem *> rootList2;
	for (int i = 0; i < totalFood.size(); i++)
		for (int j = 0; j < category.size(); j++)
			if (totalFood[i].category == category[j])
				cateFood[category[j]].push_back(totalFood[i]);

	for (int i = 0; i < category.size(); i++) {
		QTreeWidgetItem *root = new QTreeWidgetItem(ui.treeWidget_2, QStringList() << QString::fromStdString(category[i]));
		root->setIcon(0, QIcon(QString::fromStdString(categoryImgDir + category[i] + ".png"))); // set icon
		root->setSizeHint(0, QSize(50, 80));

		for (int j = 0; j < cateFood[category[i]].size(); j++) {
			QTreeWidgetItem *leaf = new QTreeWidgetItem(root, QStringList() << QString::fromStdString(cateFood[category[i]][j].name));
			QPixmap pixelImage = QPixmap::fromImage(QImage((uchar*)modifyFoodImg[cateFood[category[i]][j].fileName].data, modifyFoodImg[cateFood[category[i]][j].fileName].cols, modifyFoodImg[cateFood[category[i]][j].fileName].rows, QImage::Format_ARGB32));
			QPixmap resPix = pixelImage.scaled(80, 80);
			leaf->setIcon(0, QIcon(resPix)); // set icon
		}
		rootList2 << root;
	}
	ui.treeWidget_2->insertTopLevelItems(0, rootList2);  //設定此容器內容為QTreeWidget的根節點
	connect(ui.treeWidget_2, SIGNAL(itemClicked(QTreeWidgetItem*, int)), this, SLOT(QTreeWidgetClicked_Food(QTreeWidgetItem*, int)));

	/* init origin and result label */
	initLabel(ui.label_5);
	initLabel(ui.label_6);
	if (ui.label_6->width() < ui.label_6->height())	{ topLabelSize = ui.label_6->width(); }
	else { topLabelSize = ui.label_6->height(); }

	/* init the show table */
	initTab(ui.tabWidget);
	initTab(ui.tabWidget_2);

	///*
	//setCentralWidget(ui.tabWidget_2);
	QPushButton* addButton = new QPushButton(ui.tabWidget_2);
	addButton->setText("+");
	addButton->setMaximumWidth(25);
	addButton->setMaximumHeight(25);
	addButton->setStyleSheet("QPushButton { font-size: " + QString::number(20) + "pt; font: large 'Times New Roman'; background-color: rgba(255, 255, 255, 220); } QPushButton:hover { background-color: rgba(255, 250, 188); }" );
	ui.tabWidget_2->tabBar()->setTabButton(0, QTabBar::LeftSide, addButton);
	connect(addButton, SIGNAL(clicked()), this, SLOT(openImageFile()));

	/* init table style */
	initTable(ui.tableWidget);
	initTable(ui.tableWidget_6);
	initTable(ui.tableWidget_2);
	initTable(ui.tableWidget_3);

	///* click current food table event */
	ui.tableWidget->setStyleSheet("QTableWidget {border: none; background-color: rgba(" + white_Color_Str + ");}"
		"QTableWidget::item {background-color: rgba(" + select_Color_Str + "); padding: 5px;}"
		"QHeaderView::section {background-color: transparent;}"
		"QHeaderView {background-color: transparent;}"
		"QTableCornerButton::section {background-color: transparent;} ");
	ui.tableWidget->setColumnCount(4);
	ui.tableWidget->horizontalHeader()->setStretchLastSection(true);
	ui.tableWidget->setColumnWidth(0, 35);
	ui.tableWidget->setColumnWidth(1, 80);
	ui.tableWidget->setColumnWidth(2, 50);
	/*QStringList horHeaders_current;
	horHeaders_current << "" << "Food" << "#" << "Slider";
	ui.tableWidget->setHorizontalHeaderLabels(horHeaders_current);*/
	ui.tableWidget->verticalHeader()->setDefaultSectionSize(80);
	connect(ui.tableWidget, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(removeFoodItem2(QTableWidgetItem*)));

	///* init result table and click index event */
	ui.tableWidget_6->setStyleSheet( "QTableWidget {border: none; background-color: rgba(" + white_Color_Str + ");}"
		"QHeaderView::section {background-color: transparent;}"
		"QHeaderView {background-color: transparent;}"
		"QTableCornerButton::section {background-color: transparent;} ");
	ui.tableWidget_6->setRowCount(1);
	ui.tableWidget_6->setColumnCount(topAmount);
	ui.tableWidget_6->resizeColumnsToContents();
	ui.tableWidget_6->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui.tableWidget_6->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	connect(ui.tableWidget_6, SIGNAL(itemClicked(QTableWidgetItem*)), this, SLOT(ResultIndexClicked(QTableWidgetItem*)));
	
	///* init nutrient table */
	initNutrientTable(ui.tableWidget_2);
	for (int i = 0; i < nutrient_Str.size(); i++) {
		QSlider *sliderTmp = new QSlider(Qt::Horizontal, ui.tableWidget_2);
		sliderTmp->setMinimum(0);
		sliderTmp->setMaximum(150);
		sliderTmp->setValue(nutrientValue[i]);
		sliderTmp->setStyleSheet(
			"QSlider::groove:horizontal { border: 1px solid #999999; height: 15px; background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #B1B1B1, stop:1 #c4c4c4); }"
			"QSlider::handle:horizontal { width: 0px; background: rgba" + nutrient_Color_Str[i] + "; border: 5px solid rgba" + nutrient_Color_Str[i] + "; margin: -5px 0; border-radius: 8px;}"
			"QSlider::handle:vertical { height: 0px; background: rgba" + nutrient_Color_Str[i] + "; border: 5px solid rgba" + nutrient_Color_Str[i] + "; margin: -8px 0; border-radius: 10px;}"
			"QSlider::sub-page:horizontal { background: rgba" + nutrient_Color_Str[i] + "; }"
			);
		ui.tableWidget_2->setCellWidget(i, 2, sliderTmp);
		QSignalMapper* signalMapper = new QSignalMapper(this);
		connect(sliderTmp, SIGNAL(valueChanged(int)), signalMapper, SLOT(map()));
		signalMapper->setMapping(sliderTmp, i);
		connect(signalMapper, SIGNAL(mapped(int)), this, SLOT(changeAmount_Nutrient(int)));
	}

	initNutrientTable(ui.tableWidget_3);
	for (int i = 0; i < nutrient_Str.size(); i++) {
		QSlider *sliderTmp = new QSlider(Qt::Horizontal, ui.tableWidget_3);
		sliderTmp->setMinimum(0);
		sliderTmp->setMaximum(100);
		sliderTmp->setValue(nutrientValue_show[i]);
		sliderTmp->setStyleSheet(
			"QSlider::groove:horizontal { border: 1px solid #999999; height: 15px; background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #B1B1B1, stop:1 #c4c4c4); }"
			"QSlider::handle:horizontal { width: 1px; background: rgba" + nutrient_Color_Str[i] + "; border: 0px solid rgba" + nutrient_Color_Str[i] + ";}"
			"QSlider::handle:vertical { height: 1px; background: rgba" + nutrient_Color_Str[i] + "; border: 0px solid rgba" + nutrient_Color_Str[i] + ";}"
			"QSlider::sub-page:horizontal { background: rgba" + nutrient_Color_Str[i] + "; }"
			);
		//sliderTmp->setValue(100);
		sliderTmp->setDisabled(true);
		ui.tableWidget_3->setCellWidget(i, 2, sliderTmp);
	}

	/* set slider */
	QSlider *ratio_slider = new QSlider(Qt::Horizontal, this);;
	ratio_slider->setSingleStep(1);
	ratio_slider->setMinimum(0);
	ratio_slider->setMaximum(100);
	ratio_slider->setValue(50);
	ratio_slider->setStyleSheet(
		"QSlider::groove:horizontal { border: 1px solid #999999; height: 15px; background: qlineargradient(x1: 0, y1: 0, x2: 0, y2: 1, stop: 0 #B1B1B1, stop:1 #c4c4c4); }"
		"QSlider::handle:horizontal{ width: 5px; background: #69c2ef; border: 2px solid #004370; margin: -4px -4px;}"
		"QSlider::sub-page:horizontal{	background: #69c2ef; }"
		);
	connect(ratio_slider, SIGNAL(valueChanged(int)), this, SLOT(sliderRatioChange(int)));
	QFont fnt;
	fnt.setPointSize(15);
	fnt.setFamily("Times New Roman");
	QLabel *label0 = new QLabel(QString::number(nutrient_ratio), this);
	QLabel *label1 = new QLabel(" nutrient", this);
	QLabel *label2 = new QLabel("shape ", this);
	label0->setFont(fnt);
	label1->setFont(fnt);
	label2->setFont(fnt);
	ui.gridLayout_5->addWidget(ratio_slider, 0, 0, 1, 3);
	ui.gridLayout_5->addWidget(label1, 0, 0, 1, 1, Qt::AlignLeft);
	ui.gridLayout_5->addWidget(label2, 0, 2, 1, 1, Qt::AlignRight);

	/* set button */
	ui.ApplyButton->setStyleSheet("QPushButton { font-size: " + QString::number(20) + "pt; font: large 'Times New Roman'; background-color: rgba(170, 240, 255); } QPushButton:hover { background-color: rgba(80, 220, 255); }");
	ui.ExitButton->setStyleSheet("QPushButton { font-size: " + QString::number(20) + "pt; font: large 'Times New Roman'; background-color: rgba( 255, 170, 170); } QPushButton:hover { background-color: rgba(255, 119, 119); }");
	connect(ui.ApplyButton, SIGNAL(clicked()), this, SLOT(applyClicked()));
	connect(ui.ExitButton, SIGNAL(clicked()), QApplication::instance(), SLOT(quit()));

	// image test
	/*
	QPixmap label_pix = QPixmap::fromImage(QImage("label.png"));
	QPixmap label_resPix = label_pix.scaled(ui.label->width(), ui.label->width(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
	initLabel(ui.label);
	ui.label->setPixmap(label_pix);
	ui.label->setAlignment(Qt::AlignCenter);
	ui.label->setScaledContents(true);
	ui.label->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
	*/
}

/* --------------------------------Main Function-------------------------------- */
void prePocessOfUserDraw(Mat& userDraw) {
	disjointContour.clear();
	desOfDraw.clear();
	desOfDrawReverse.clear();
	samplepointsOfDraw.clear();
	samplepointsOfDrawReverse.clear();
	groupIdx.clear();
	groupIdxOfContour.clear();
	groupWarpMatOfContour.clear();
	leafNodeVec.clear();

	vector<vector<Point> > userDrawContours;
	vector<Vec4i> hierarchy;
	Mat cannyColor = cannyThreeCh(userDraw, true);
	findContours(cannyColor.clone(), userDrawContours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, Point(0, 0));

	for (int i = userDrawContours.size() - 1; i >= 0; i--)
	{
		double cArea = contourArea(userDrawContours[i], false);

		if (cArea > 200 && hierarchy[i][3] != -1) {
			vector<int>hull;
			convexHull(userDrawContours[i], hull, true);
			if (hull[0] > hull[1])
				reverse(userDrawContours[i].begin(), userDrawContours[i].end());
			disjointContour.push_back(userDrawContours[i]);
		}
	}

	RNG rng(12345);
	Mat drawing = Mat::zeros(inputSize, CV_8UC3);
	for (int i = 0; i < disjointContour.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
		drawContours(drawing, disjointContour, i, color, 1, 8);

		//imwrite("contour" + to_string(i) + "_" + to_string(testa) + ".png", drawing);
	}
	imwrite("canny.png", cannyColor);
	imwrite("contour.png", drawing);

	vector<Vec4b> colorOfContour(disjointContour.size());
	vector<Point> centerOfContour(disjointContour.size());

	int c_top = userDraw.cols, c_bottom = 0, c_left = userDraw.rows, c_right = 0;
	for (int i = 0; i < disjointContour.size(); i++)
	{
		descri descriUser(disjointContour[i]);
		desOfDraw.push_back(descriUser.seqDescri());
		samplepointsOfDraw.push_back(descriUser.sampleResult());
		int top = userDraw.cols, bottom = 0, left = userDraw.rows, right = 0;
		for (int j = 0; j < descriUser.sampleResult().size(); j++) {
			if (descriUser.sampleResult()[j].x < left) //left
				left = descriUser.sampleResult()[j].x;
			if (descriUser.sampleResult()[j].x > right) //right
				right = descriUser.sampleResult()[j].x;
			if (descriUser.sampleResult()[j].y < top) //top
				top = descriUser.sampleResult()[j].y;
			if (descriUser.sampleResult()[j].y > bottom) //bottom
				bottom = descriUser.sampleResult()[j].y;
		}

		if (left < c_left) c_left = left;
		if (right > c_right) c_right = right;
		if (top < c_top) c_top = top;
		if (bottom > c_bottom) c_bottom = bottom;

		Point center((left + right) / 2, (top + bottom) / 2);
		colorOfContour[i] = userDraw.at<Vec4b>(center.y, center.x);
		centerOfContour[i] = center;
	}
	topLeft = Point(c_left, c_top);
	bottomRight = Point(c_right, c_bottom);

	// store reverse descriptor and samplePoint of contour
	for (int i = 0; i < disjointContour.size(); i++)
	{
		vector<Point> tmpContour = disjointContour[i];
		reversePoint(centerOfContour[i], tmpContour);
		descri descriUserReverse(tmpContour);
		desOfDrawReverse.push_back(descriUserReverse.seqDescri());
		samplepointsOfDrawReverse.push_back(descriUserReverse.sampleResult());
	}

	// grouping
	groupIdx.clear();
	groupIdxOfContour.resize(desOfDraw.size());
	groupWarpMatOfContour.resize(desOfDraw.size(), Mat::zeros(Size(2, 3), CV_64F));
	grouping(groupIdx, groupIdxOfContour, groupWarpMatOfContour, colorOfContour, centerOfContour);
}

void QtGuiApplication1::compareWithRecipe(Mat& userDraw, recipe& rec2) {
	// compare contour with food
	cfMap foodCandidate;
	fragList pairSeq;
	Mat table_resultStack = Mat::zeros(inputSize, CV_8UC4);

	for (int i = 0; i < groupIdx.size(); i++) {
		int contourIdx = groupIdx[i][0];

		// doCompare for the first element in this group
		for (int j = 0; j < rec2.size; j++)	{
			doCompare(false, pairSeq, userDraw, desOfFood, samplepointsOfFood, contourIdx, j, rec2);
			doCompare(true, pairSeq, userDraw, desOfFoodReverse, samplepointsOfFoodReverse, contourIdx, j, rec2); // reverse
		}
		foodCandidate.Element[contourIdx] = pairSeq;

		// for other contour "contourIdx2" that are in the same group("contourIdx)
		for (int g = 1; g < groupIdx[i].size(); g++) {
			int contourIdx2 = groupIdx[i][g];
			foodCandidate.Element[contourIdx2] = pairSeq;
			Mat tmp_resultStack = table_resultStack;

			double smallError = 100;
			for (int j = 0; j < foodCandidate.Element[contourIdx2].Element.size(); j++) {
				Mat tmpWarpImg = Mat::zeros(inputSize, CV_8UC4);
				Mat flipWarpImg = Mat::zeros(inputSize, CV_8UC4);
				warpAffine(foodCandidate.Element[contourIdx2].Element[j].warpImg, tmpWarpImg, groupWarpMatOfContour[contourIdx2], inputSize);
				//get center x
				int minX = flipWarpImg.cols - 1, maxX = 0;
				for (int fc = 0; fc < flipWarpImg.cols; fc++)
					for (int fr = 0; fr < flipWarpImg.rows; fr++)
						if (tmpWarpImg.at<Vec4b>(fr, fc)[3] != 0)
						{
							if (fc < minX)	minX = fc;
							if (fc > maxX)	maxX = fc;
						}
				//do flip
				for (int wc = 0; wc < tmpWarpImg.cols; wc++)
					for (int wr = 0; wr < tmpWarpImg.rows; wr++)
						if (tmpWarpImg.at<Vec4b>(wr, wc)[3] != 0)
							flipWarpImg.at<Vec4b>(wr, (minX + maxX) - wc) = tmpWarpImg.at<Vec4b>(wr, wc);
				foodCandidate.Element[contourIdx2].Element[j].warpImg = flipWarpImg.clone();

				if (foodCandidate.Element[contourIdx2].Element[j].iError < smallError) {
					smallError = foodCandidate.Element[contourIdx2].Element[j].iError;
					tmp_resultStack = addTransparent(table_resultStack, foodCandidate.Element[contourIdx2].Element[j].warpImg);
					tmp_resultStack = addTransparent(tmp_resultStack, flipWarpImg.clone());

					table_resultStack = tmp_resultStack.clone();
				}
			}
		}
		pairSeq.Element.clear();
	}

	// do combination if contour is not satisfy
	vector<fragList> sortedFragList;
	for (int i = 0; i < disjointContour.size(); i++)
	{
		// sort with iError
		vecSeqIndex(foodCandidate.Element[i], rec2.size);

		bool isSatisfy = false;
		for (int j = foodCandidate.Element[i].Element.size() - 1; j >= 0; j--) {
			if (foodCandidate.Element[i].Element[j].iErrorRatio1 >= 0.75) {
				isSatisfy = true;
				if (j == (foodCandidate.Element[i].Element.size() - 1))
					foodCandidate.Element[i].Element.erase(foodCandidate.Element[i].Element.end() - 1);
				else
					foodCandidate.Element[i].Element.erase(foodCandidate.Element[i].Element.begin() + j + 1, foodCandidate.Element[i].Element.end());
				break;
			}
		}

		if (!isSatisfy) {
			vector<int> sameFood;
			for (int j = 0; j < foodCandidate.Element[i].Element.size(); j++) {
				sameFood.push_back(foodCandidate.Element[i].Element[j].fIndex);
				Mat resultStack = foodCandidate.Element[i].Element[j].warpImg;
				vector<vector<Point> > new_contourPoint = getNewContour(disjointContour[i], resultStack);

				bool isIn = false;
				Mat resultStack2 = resultStack;
				int recIdx = foodCandidate.Element[i].Element[j].rIndex;
				int foodImgIdx = foodCandidate.Element[i].Element[j].fIndex;
				vector<int> recipe_num_tmp = foodCandidate.Element[i].Element[j].recipe_num;

				for (int ci = 0; ci < new_contourPoint.size(); ci++) {

					if (new_contourPoint[ci].size() > 0) {
						descri descriUser(new_contourPoint[ci]);
						vector<Point> c_new_contourPoint = new_contourPoint[ci];
						descri c_descriUser = descriUser;
						Mat c_resultStack = resultStack;

						if (foodCombination(i, foodImgIdx, recipe_num_tmp[recIdx], recipe_num_tmp[recIdx], rec2.foodNum()[recIdx], c_new_contourPoint, c_descriUser, c_resultStack, rec2)) {
							isIn = true;
							resultStack2 = addTransparent(resultStack2, c_resultStack);
						}
					}
				}
				if (isIn) {
					frag tmpp = foodCandidate.Element[i].Element[j];
					tmpp.warpImg = resultStack2;
					tmpp.iError = imageOverlap(i, j, tmpp.warpImg);
					tmpp.recipe_num.assign(recipe_num_tmp.begin(), recipe_num_tmp.end());
					tmpp.setError(edgeError(userDraw, tmpp.warpImg), colorError(userDraw, tmpp.warpImg), refError(userDraw, tmpp.warpImg), 0, 0);
					foodCandidate.Element[i].Element.push_back(tmpp);
				}
			}
		}

		// sort with iError
		vecSeqIndex(foodCandidate.Element[i], vecSeqIndexAmount);
		sortedFragList.push_back(foodCandidate.Element[i]);
	}
	foodCandidate.Element.clear();

	/*
	for (int i = 0; i < sortedFragList.size(); i++) {
		for (int j = 0; j < sortedFragList[i].Element.size(); j++) {
			string aa = "";
			for (int k = 0; k < sortedFragList[i].Element[j].recipe_num.size(); k++) {
				aa.append("_");
				aa.append(to_string(sortedFragList[i].Element[j].recipe_num[k]));
			}
			string cc = "Result/" + to_string(i) + "_" + to_string(j) + "*" + aa + ".png";
			imwrite(cc, sortedFragList[i].Element[j].warpImg);
		}
	}
	*/

	// build tree Start ----------------------------------------
	///*
	Mat resultStack;
	tree<string> tr;
	tree<string>::iterator top;
	top = tr.begin();
	tr.insert(top, "top");

	int idx = 0;
	while (idx < sortedFragList.size() && sortedFragList[idx].Element.size() < 1) {
		idx++;
	}
	appendAllFrag(0, tr, "top", idx, sortedFragList);
	for (int i = 0; i < sortedFragList[idx].Element.size(); i++) {
		buildTree(tr, rec2, to_string(idx) + "*" + to_string(i), idx, i, sortedFragList[idx].Element[i].warpImg, sortedFragList, userDraw, 0, groupIdxOfContour);
	}
	//*/
}

/* --------------------------------Build Tree Function-------------------------------- */
vector<string> split_str(string s, char ch) {
	vector<string> tokens;
	istringstream ss(s);
	string token;

	while (std::getline(ss, token, ch)) {
		tokens.push_back(token);
	}
	return tokens;
}

void appendAllFrag(int state, tree<string>& tr, string c_str, int nextIndex, vector<fragList>& sortedFragList) {

	tree<string>::iterator loc = find(tr.begin(), tr.end(), c_str);
	if (state != 0) {
		for (int i = 0; i < sortedFragList[nextIndex].Element.size(); i++) {
			tr.append_child(loc, c_str + "_" + to_string(nextIndex) + "*" + to_string(i));
		}
	}
	else {
		for (int i = 0; i < sortedFragList[nextIndex].Element.size(); i++) {
			tr.append_child(loc, to_string(nextIndex) + "*" + to_string(i));
		}
	}
}

bool hasGroup(string str, int currentIndex, int fragIndex, vector<fragList>& sortedFragList, vector<int>& groupIdxOfContour) {
	int group = -1, group_frag = 0;
	vector<string> vec = split_str(str, '_');

	for (int i = 0; i < vec.size(); i++) {
		vector<string> tmp = split_str(vec[i], '*');
		int last_currentIndex = stoi(tmp[0]);

		if (groupIdxOfContour[currentIndex] == groupIdxOfContour[last_currentIndex]) {
			group = last_currentIndex;
			group_frag = stoi(tmp[1]);
			break;
		}
	}

	if (group != -1) {
		if (sortedFragList[currentIndex].Element[fragIndex].fIndex != sortedFragList[group].Element[group_frag].fIndex)
			return false;
	}
	return true;
}

Mat stackWithTopo(string str, vector<fragList>& sortedFragList) {
	vector<string> vec = split_str(str, '_');
	vector<vector<int> > candidate(vec.size(), vector<int>(2));
	for (int i = 0; i < vec.size(); i++) {
		vector<string> tmp = split_str(vec[i], '*');
		candidate[i][0] = stoi(tmp[0]);
		candidate[i][1] = stoi(tmp[1]);
	}
	vec.clear();

	// topological sort
	topo graph((int)candidate.size());

	for (int i = 0; i < candidate.size(); i++)
	{
		for (int j = i + 1; j < candidate.size(); j++)
		{
			int i_contourIdx = candidate[i][0];
			int i_foodIdx = candidate[i][1];
			int j_contourIdx = candidate[j][0];
			int j_foodIdx = candidate[j][1];

			Mat iContourImg = sortedFragList[i_contourIdx].Element[i_foodIdx].warpImg.clone();
			Mat jContourImg = sortedFragList[j_contourIdx].Element[j_foodIdx].warpImg.clone();

			//draw contour i first fragment area after warping
			for (int ic = 0; ic < iContourImg.cols; ic++)
			{
				for (int ir = 0; ir < iContourImg.rows; ir++)
				{
					Vec4b BGRA = iContourImg.at<Vec4b>(ir, ic);
					Vec4b newBGRA = Vec4b(0, 255, 0, 255);
					if (BGRA[3] != 0)
						iContourImg.at<Vec4b>(ir, ic) = newBGRA;
				}
			}

			//draw contour j first fragment area after warping
			for (int jc = 0; jc < jContourImg.cols; jc++)
			{
				for (int jr = 0; jr < jContourImg.rows; jr++)
				{
					Vec4b BGRA = jContourImg.at<Vec4b>(jr, jc);
					Vec4b newBGRA = Vec4b(255, 0, 0, 255);
					if (BGRA[3] != 0)
						jContourImg.at<Vec4b>(jr, jc) = newBGRA;
				}
			}

			//stack j on i
			Mat cannyStackJI = cannyThreeCh(addTransparent(iContourImg, jContourImg), true);
			//stack i on j
			Mat cannyStackIJ = cannyThreeCh(addTransparent(jContourImg, iContourImg), true);

			double joni = iContourError(samplepointsOfDraw[i_contourIdx], samplepointsOfDraw[j_contourIdx], cannyStackJI);
			double ionj = iContourError(samplepointsOfDraw[i_contourIdx], samplepointsOfDraw[j_contourIdx], cannyStackIJ);

			//smaller cannyStackJI, add edge ij
			if (joni < ionj)
				graph.addEdge(i, j, abs(ionj - joni));
			else if (ionj < joni)
				graph.addEdge(j, i, abs(ionj - joni));

			//graph.printAdjList();
			graph.isCyclic();

			Mat drawConIJ = Mat::zeros(inputSize, CV_8UC3);
			drawContours(drawConIJ, samplepointsOfDraw, i, Scalar(255, 255, 255), 1);
			drawContours(drawConIJ, samplepointsOfDraw, j, Scalar(255, 255, 255), 1);
		}
	}
	//graph.printAdjList();
	graph.topoSort();

	Mat resultStack = Mat::zeros(inputSize, CV_8UC4);
	for (int i = 0; i < graph.sortResult.size(); i++) {
		resultStack = addTransparent(resultStack, sortedFragList[candidate[graph.sortResult[i]][0]].Element[candidate[graph.sortResult[i]][1]].warpImg).clone();
	}

	return resultStack;
}

double getHealthError(vector<int> nutrient) {
	double tmpHErr = 0;
	for (int i = 0; i < nutrient_Str.size(); i++)
		tmpHErr += abs((nutrient[i]/nutrientDay[i]-nutrientValue[i]));
	return tmpHErr / nutrient_Str.size();
}

vector<int> getStrNutrient(string str, vector<fragList>& sortedFragList, recipe& recipeTmp) {
	
	vector<int> tmp_Nutrient(nutrient_Str.size(), 0);
	vector<string> vec = split_str(str, '_');
	for (int i = 0; i < vec.size(); i++) {
		vector<string> tmp = split_str(vec[i], '*');
		int contourIndex = stoi(tmp[0]);
		int candidateIndex = stoi(tmp[1]);

		for (int j = 0; j < recipeTmp.foodIndex().size(); j++) {
			int recipeIdx = recipeTmp.foodIndex()[j];
			int recipeNum = sortedFragList[contourIndex].Element[candidateIndex].recipe_num[j];
			if (recipeNum > 0) {
				//nutrientValue_show
				tmp_Nutrient[0] += totalFood[recipeIdx].cal*recipeNum;
				tmp_Nutrient[1] += totalFood[recipeIdx].protein*recipeNum;
				tmp_Nutrient[2] += totalFood[recipeIdx].calcium*recipeNum;
				tmp_Nutrient[3] += totalFood[recipeIdx].iron*recipeNum;
				tmp_Nutrient[4] += totalFood[recipeIdx].vitaminB1*recipeNum;
				tmp_Nutrient[5] += totalFood[recipeIdx].vitaminB2*recipeNum;
				tmp_Nutrient[6] += totalFood[recipeIdx].vitaminC*recipeNum;
				tmp_Nutrient[7] += totalFood[recipeIdx].vitaminE*recipeNum;
			}
		}
	}
	return tmp_Nutrient;
}

void QtGuiApplication1::updateResultTable() {
	int imgSize = ui.tableWidget_6->width() / topAmount;
	if (ui.tableWidget_6->width() / topAmount > ui.tableWidget_6->height())
		imgSize = ui.tableWidget_6->height();

	int smallSize = topAmount;
	if (leafNodeVec.size() < smallSize) { smallSize = leafNodeVec.size(); }
	sort(leafNodeVec.begin(), leafNodeVec.end(), compareWithTotalError);

	for (int i = 0; i < smallSize; i++) {
		Mat currentImage = leafNodeVec[i].image.clone();

		QImage qImg((uchar*)currentImage.data, currentImage.cols, currentImage.rows, QImage::Format_ARGB32);
		QImage smallImg = (&qImg)->scaled(imgSize, imgSize, Qt::KeepAspectRatio);

		QTableWidgetItem *pCell = new QTableWidgetItem;
		pCell->setData(Qt::DecorationRole, QPixmap::fromImage(smallImg));
		pCell->setTextAlignment(Qt::AlignCenter);
		ui.tableWidget_6->removeCellWidget(0, i);
		ui.tableWidget_6->setItem(0, i, pCell);
	}

	Mat currentTopImage = leafNodeVec[0].image.clone();
	QPixmap pix = QPixmap::fromImage(QImage((uchar*)currentTopImage.data, currentTopImage.cols, currentTopImage.rows, QImage::Format_ARGB32));
	QPixmap resPix = pix.scaled(topLabelSize, topLabelSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
	ui.label_5->setPixmap(resPix);
}

void QtGuiApplication1::updateNutrientSlider(vector<int> tmpNutrient) {
	if (leafNodeVec.size() > 0) {
		for (int i = 0; i < nutrient_Str.size(); i++) {
			nutrientValue_show[i] = tmpNutrient[i];
		}
		updateNutrientShowTable(ui.tableWidget_3);
	}
}

void QtGuiApplication1::buildTree(tree<string>& tr, recipe recipeTmp, string str, int currentIndex, int fragIndex, Mat resultStack, vector<fragList>& sortedFragList, Mat userDraw, double iErrorValue, vector<int>& groupIdxOfContour) {
	int startFrag = 0;
	int idx = currentIndex;
	bool recursive = true, addNode = false;
	double new_iErrorValue = iErrorValue + sortedFragList[currentIndex].Element[fragIndex].iError;

	for (int i = 0; i < sortedFragList[currentIndex].Element[fragIndex].recipe_num.size(); i++) {
		recipeTmp.reduceFoodByIdx(i, sortedFragList[currentIndex].Element[fragIndex].recipe_num[i] - 1);
		if (recipeTmp.foodNum()[i] <= 0) {
			recursive = false;
			addNode = true;
		}
	}

	// if the last contour is satisfy, then stop recursive
	if (sortedFragList.size() - 1 == idx) {
		recursive = false;
	}
	// if the contour is satisfy, then go to next contour -> idx++
	else {
		idx++;

		// if the contour doesn't have any fragment 
		while (idx < sortedFragList.size() && sortedFragList[idx].Element.size() < 1) {
			idx++;
		}
	}

	tree<string>::iterator loc = std::find(tr.begin(), tr.end(), str);
	if (idx < sortedFragList.size() && recursive) {
		addNode = true;

		for (int i = 0; i < sortedFragList[idx].Element.size(); i++) {
			if (hasGroup(str, idx, i, sortedFragList, groupIdxOfContour)) {
				string next_str = to_string(idx) + "*" + to_string(i);
				Mat resultStack_2 = addTransparent(resultStack, sortedFragList[idx].Element[i].warpImg);

				tr.append_child(loc, str + "_" + to_string(idx) + "*" + to_string(i));
				buildTree(tr, recipeTmp, str + "_" + next_str, idx, i, resultStack_2, sortedFragList, userDraw, new_iErrorValue, groupIdxOfContour);
			}
		}
	}

	if (!addNode) {
		cout << str << endl;
		int treeDepth = (tr.depth(loc));
		resultStack = stackWithTopo(str, sortedFragList);
		//imwrite("Result0/" + to_string(totalErr) + ".png", resultStack);

		new_iErrorValue = new_iErrorValue / (double)treeDepth;
		double edgeE = edgeError(userDraw, resultStack);
		double refE = refError(userDraw, resultStack);
		double colE = colorError(userDraw, resultStack);
		double totalErr = edgeE + refE + colE + new_iErrorValue;

		vector<int> tmpNutrient = getStrNutrient(str, sortedFragList, recipeTmp);
		double hErr = getHealthError(tmpNutrient);
		
		leafNode tmpLeaf;
		tmpLeaf.image = resultStack.clone();
		tmpLeaf.originError = totalErr;
		totalErr += hErr;
		tmpLeaf.totalError = (tmpLeaf.originError*(100 - nutrient_ratio) + hErr*nutrient_ratio) / 100;
		tmpLeaf.nutrientVec.assign(tmpNutrient.begin(), tmpNutrient.end());
		leafNodeVec.push_back(tmpLeaf);

		updateResultTable();
		updateNutrientSlider(leafNodeVec[0].nutrientVec);

		string aa = "";
		for (int k = 0; k < sortedFragList[currentIndex].Element[fragIndex].recipe_num.size(); k++) {
			aa.append("_");
			aa.append(to_string(sortedFragList[currentIndex].Element[fragIndex].recipe_num[k]));
		}
		imwrite("ResultAll/" + to_string(totalErr) + "**" + aa + ".png", resultStack);
		
		qApp->processEvents();
	}
}

/* --------------------------------Food Combination Function-------------------------------- */
vector<vector<Point>> getNewContour(vector<Point> &contourPoint, Mat warpImg) {
	Mat contour_dist(warpImg.size(), CV_32FC1);
	Mat foodAlphaBin = alphaBinary(warpImg);
	Mat new_drawClone = Mat::zeros(warpImg.size(), CV_8UC1);

	for (int i = 0; i < warpImg.cols; i += 1) {
		for (int j = 0; j < warpImg.rows; j += 1) { //height
			contour_dist.at<float>(j, i) = pointPolygonTest(contourPoint, Point2f(i, j), true);
			if (contour_dist.at<float>(j, i) > 0 && !(foodAlphaBin.at<int>(j, i))) {
				new_drawClone.at<uchar>(j, i) = 255;
			}
		}
	}
	//morphological opening (removes small objects from the foreground)
	erode(new_drawClone, new_drawClone, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	dilate(new_drawClone, new_drawClone, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	//morphological closing (removes small holes from the foreground)
	dilate(new_drawClone, new_drawClone, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
	erode(new_drawClone, new_drawClone, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

	vector<vector<Point> > userDrawContours;
	vector<Vec4i> hierarchy;
	findContours(new_drawClone.clone(), userDrawContours, hierarchy, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, Point(0, 0));
	sort(userDrawContours.begin(), userDrawContours.end(), compareContourSize);

	if (userDrawContours.size() > 0) {
		vector<int>hull;
		convexHull(userDrawContours[0], hull, true);
		if (hull[0] > hull[1])
			reverse(userDrawContours[0].begin(), userDrawContours[0].end());

		Mat drawing = Mat::zeros(warpImg.size(), CV_8UC3);
		drawContours(drawing, userDrawContours, 0, Scalar(255, 255, 255), 1, 8);
		return userDrawContours;
	}
	else {
		vector<vector<Point>> buff;
		return buff;
	}
}

int imageOverlap(int a, int b, Mat& resultImg)
{
	Size _inputSize = resultImg.size();
	Mat contour_dist(_inputSize, CV_32FC1);
	Mat drawing0 = Mat::zeros(resultImg.size(), CV_8UC3);
	int contourArea = 0, foodArea = 0, overlapArea = 0;

	int strideSize = 5;
	for (int i = 0; i < _inputSize.width; i += strideSize) {
		for (int j = 0; j < _inputSize.height; j += strideSize) {
			contour_dist.at<float>(j, i) = pointPolygonTest(samplepointsOfDraw[a], Point2f(i, j), true);
			Vec4b & bgra = resultImg.at<Vec4b>(j, i);

			int same = 0;
			//calculate contour area
			if (contour_dist.at<float>(j, i) > 0) {
				same++;
				contourArea++;
				drawing0.at<Vec3b>(j, i)[2] = 255;
			}
			if (bgra[3] != 0) {
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
	return (1 - ratio);
}

bool foodCombination(int contourIdx, int foodImgIdx, int& recipe_num_tmp, int recipe_num_contour, int recipe_num, vector<Point>& c_new_contourPoint, descri& c_descriUser, Mat& resultStack, recipe& rec2) {
	if (recipe_num_tmp < recipe_num) {
		comp compDes(1, false, 0.4, inputSize, c_descriUser.seqDescri(), desOfFood[foodImgIdx], c_descriUser.sampleResult(), samplepointsOfFood[foodImgIdx], contourIdx, foodImgIdx, totalFood, totalFoodImg);
		fragList tmpPairSeq_t;
		tmpPairSeq_t.Element = compDes.fragList3();

		if (tmpPairSeq_t.Element.size() > 0)
		{
			int minSize = min((recipe_num - recipe_num_tmp), tmpPairSeq_t.Element.size());
			for (int m = 0; m < minSize; m++) {
				Mat resultStack_2 = tmpPairSeq_t.Element[m].warpImg;
				resultStack = addTransparent(resultStack, resultStack_2);
			}
			recipe_num_tmp += minSize;
		}
		else
			return false;

		if (c_new_contourPoint.size() == 0)
			return false;

		bool breakOrNot = false;
		vector<vector<Point> > new_contourPoint = getNewContour(c_new_contourPoint, resultStack);
		for (int ci = 0; ci < new_contourPoint.size(); ci++) {
			if (new_contourPoint[ci].size() == 0) {
				breakOrNot = true;
				break;
			}
			else {
				descri tmp_descriUser(new_contourPoint[ci]);
				foodCombination(contourIdx, foodImgIdx, recipe_num_tmp, recipe_num_contour, recipe_num, new_contourPoint[ci], tmp_descriUser, resultStack, rec2);
			}
		}

		if (breakOrNot)
			return false;

		//c_descriUser = descri(c_new_contourPoint);
	}
	if (recipe_num_tmp != recipe_num_contour) {
		return true;
	}
	return false;
}

/* --------------------------------Pre-Processing-------------------------------- */
void preProcess(string s_file, vector<vector<Point> >& samplePoint, string p_file, vector<vector<Mat> >& descriptor, string d_file, string d_file2, string d_file3, string d_file4) {
	int idx = 0;
	int count = 0;

	vector<int> getSize;
	ifstream fin(preDataDir + s_file, ios::in | ios::binary);
	int num;
	fin.read((char *)&num, sizeof(num));
	
	int testM = 0;
	for (int i = 0; i<num; ++i) {
		int buff;
		fin.read((char *)&buff, sizeof(buff));
		getSize.push_back(buff);
		testM += buff;
	}
	vector<Mat> get_foodDesSeq = vecmatread(preDataDir + d_file);
	vector<Mat> get_foodDesSeq2 = vecmatread(preDataDir + d_file2);
	vector<Mat> get_foodDesSeq3 = vecmatread(preDataDir + d_file3);
	vector<Mat> get_foodDesSeq4 = vecmatread(preDataDir + d_file4);
	vector<Point> get_sampleResult = vecPointRead(preDataDir + p_file);
	vector<Mat> buff_foodDesSeq;
	vector<Point> buff_sampleResult;

	cout << "get_foodDesSeq.size()= " << get_foodDesSeq.size() << endl;
	cout << "get_foodDesSeq2.size()= " << get_foodDesSeq2.size() << endl;
	cout << "get_foodDesSeq3.size()= " << get_foodDesSeq3.size() << endl;
	cout << "get_foodDesSeq4.size()= " << get_foodDesSeq4.size() << endl;
	cout << "get_sampleResult.size()= " << get_sampleResult.size() << endl;

	int tmpSampleInt = 0;
	for (int i = 0; i < get_foodDesSeq.size(); i++)	{
		buff_foodDesSeq.push_back(get_foodDesSeq[i]);
		buff_sampleResult.push_back(get_sampleResult[i]);
		count++;
		if (count == getSize[idx]) {
			idx++;
			count = 0;

			descriptor.push_back(buff_foodDesSeq);
			samplePoint.push_back(buff_sampleResult);
			buff_foodDesSeq.clear();
			buff_sampleResult.clear();
		}
	}
	cout << "idx= " << idx;

	
	tmpSampleInt = get_foodDesSeq.size();
	for (int i = 0; i < get_foodDesSeq2.size(); i++)	{
		buff_foodDesSeq.push_back(get_foodDesSeq2[i]);
		buff_sampleResult.push_back(get_sampleResult[i + tmpSampleInt]);
		count++;
		if (count == getSize[idx]) {
			idx++;
			count = 0;

			descriptor.push_back(buff_foodDesSeq);
			samplePoint.push_back(buff_sampleResult);
			buff_foodDesSeq.clear();
			buff_sampleResult.clear();
		}
	}
	cout << ", idx1= " << idx;

	
	tmpSampleInt += get_foodDesSeq2.size();
	for (int i = 0; i < get_foodDesSeq3.size(); i++)	{
		buff_foodDesSeq.push_back(get_foodDesSeq3[i]);
		buff_sampleResult.push_back(get_sampleResult[i + tmpSampleInt]);
		count++;
		if (count == getSize[idx]) {
			idx++;
			count = 0;

			descriptor.push_back(buff_foodDesSeq);
			samplePoint.push_back(buff_sampleResult);
			buff_foodDesSeq.clear();
			buff_sampleResult.clear();
		}
	}

	cout << ", idx2= " << idx;
	
	tmpSampleInt += get_foodDesSeq3.size();
	for (int i = 0; i < get_foodDesSeq4.size(); i++)	{
		buff_foodDesSeq.push_back(get_foodDesSeq4[i]);
		buff_sampleResult.push_back(get_sampleResult[i + tmpSampleInt]);
		count++;
		if (count == getSize[idx]) {
			idx++;
			count = 0;

			descriptor.push_back(buff_foodDesSeq);
			samplePoint.push_back(buff_sampleResult);
			buff_foodDesSeq.clear();
			buff_sampleResult.clear();
		}
	}

	cout << endl;
}

vector<Mat> vecmatread(const string& filename)
{
	vector<Mat> matrices;
	ifstream fs(filename, fstream::binary);

	// Get length of file
	fs.seekg(0, fs.end);
	int length = fs.tellg();
	cout << filename<<": "<<length << endl;
	fs.seekg(0, fs.beg);
	
	while (fs.tellg() < length)
	{
		//cout << "!!!" << endl;
		// Header
		int rows, cols, type, channels;
		fs.read((char*)&rows, sizeof(int));         // rows
		fs.read((char*)&cols, sizeof(int));         // cols
		fs.read((char*)&type, sizeof(int));         // type
		fs.read((char*)&channels, sizeof(int));     // channels

		// Data
		Mat mat(rows, cols, type);
		fs.read((char*)mat.data, CV_ELEM_SIZE(type) * rows * cols);

		matrices.push_back(mat);
	}

	return matrices;
}

vector<Point> vecPointRead(const string& filename)
{
	Point buff;
	vector<Point> matrices;
	ifstream fs(filename, fstream::binary);

	int num;
	fs.read((char *)&num, sizeof(num));

	for (int i = 0; i<num; ++i) {
		int val_x, val_y;
		fs.read((char *)&val_x, sizeof(val_x));
		fs.read((char *)&val_y, sizeof(val_y));
		buff.x = val_x;
		buff.y = val_y;
		matrices.push_back(buff);
	}
	return matrices;
}

void getAllRecipe(string recipeDir, vector<string>& files, vector<recipe>& RecipeFiles) {
	/*add stuff inside the table view*/
	for (int i = 0; i < files.size(); i++) {
		recipe rec;
		rec.readRecipeFile(recipeDir + files[i]);
		rec.print();
		RecipeFiles.push_back(rec);
	}
}

void readFoodPreData(vector<food>& totalFood)
{
	ifstream fileN(foodPreDir + "name.bin", ios::binary);
	ifstream fileF(foodPreDir + "fileName.bin", ios::binary);
	ifstream fileC(foodPreDir + "cate.bin", ios::binary);
	ifstream fileS(foodPreDir + "season.bin", ios::binary);
	ifstream fileP(foodPreDir + "period.bin", ios::binary);
	ifstream fileH(foodPreDir + "health.bin", ios::binary);

	int length;

	/*fileN*/
	vector<string> foodN;
	fileN.seekg(0, fileN.end);
	length = fileN.tellg();
	fileN.seekg(0, fileN.beg);
	char* tempN = new char[length + 1];
	if (fileN.good())
		fileN.read(tempN, length);
	else
		cout << "fileN error" << endl;

	tempN[length] = '\0';

	stringstream  dataN(tempN);
	string lineN;

	while (getline(dataN, lineN, '*'))
		foodN.push_back(lineN);


	/*fileF*/
	vector<string> foodF;
	fileF.seekg(0, fileF.end);
	length = fileF.tellg();
	fileF.seekg(0, fileF.beg);
	char* tempF = new char[length + 1];
	if (fileF.good())
		fileF.read(tempF, length);
	else
		cout << "fileF error" << endl;

	tempF[length] = '\0';

	stringstream  dataF(tempF);
	string lineF;

	while (getline(dataF, lineF, '*'))
		foodF.push_back(lineF);

	/*fileC*/
	vector<string> foodC;
	fileC.seekg(0, fileC.end);
	length = fileC.tellg();
	fileC.seekg(0, fileC.beg);
	char* tempC = new char[length + 1];
	if (fileC.good())
		fileC.read(tempC, length);
	else
		cout << "fileC error" << endl;

	tempC[length] = '\0';

	stringstream  dataC(tempC);
	string lineC;

	while (getline(dataC, lineC, '*'))
		foodC.push_back(lineC);

	/*fileS*/
	fileS.seekg(0, fileS.end);
	length = fileS.tellg();
	fileS.seekg(0, fileS.beg);

	vector<int> foodS(length / sizeof(int));
	fileS.read((char*)foodS.data(), foodS.size()*sizeof(int));

	/*fileP*/
	fileP.seekg(0, fileP.end);
	length = fileP.tellg();
	fileP.seekg(0, fileP.beg);

	vector<int> foodP(length / sizeof(int));
	fileP.read((char*)foodP.data(), foodP.size()*sizeof(int));
	
	/*fileH*/
	fileH.seekg(0, fileH.end);
	length = fileH.tellg();
	fileH.seekg(0, fileH.beg);
	vector<double> foodH(length / sizeof(double));
	fileH.read((char*)foodH.data(), foodH.size()*sizeof(double));

	fileH.close();

	/*push food*/
	for (int i = 0; i < foodN.size(); i++)
	{
		food food(foodN[i], foodF[i], foodC[i], foodS[i], foodP[i]);
		food.setHealth(foodH[i * 9], foodH[i * 9 + 1], foodH[i * 9 + 2], foodH[i * 9 + 3], foodH[i * 9 + 4], foodH[i * 9 + 5], foodH[i * 9 + 6], foodH[i * 9 + 7], foodH[i * 9 + 8]);
		totalFood.push_back(food);
	}


}

void readBinaryImg(vector<Mat>& totalImg, string fileName)
{
	ifstream fileI(fileName, ios::binary);
	fileI.seekg(0, fileI.end);
	int length = fileI.tellg();
	fileI.seekg(0, fileI.beg);
	int i = 0;
	while (fileI.tellg() < length)
	{
		int rows, cols, type;
		fileI.read((char*)&rows, sizeof(int));
		fileI.read((char*)&cols, sizeof(int));
		fileI.read((char*)&type, sizeof(int));

		Mat nMat = Mat::zeros(rows, cols, type);
		for (int j = 0; j < nMat.rows*nMat.cols; j++)
		{
			Vec4b data;
			fileI.read((char*)&data, sizeof(data));
			nMat.at<Vec4b>(j) = data;
		}
		//cout << "i: " << i << " ";
		//i++;
		totalImg.emplace_back(nMat);
	}
	fileI.close();
	cout << endl;
}

/* --------------------------------Image Processing-------------------------------- */
void grouping(vector<vector<int> >& groupIdx, vector<int>& groupIdxOfContour, vector<Mat>& groupWarpMatOfContour, vector<Vec4b>& colorOfContour, vector<Point>& centerOfContour)
{
	vector<vector<bool> > tmp_group(desOfDraw.size(), vector<bool>(desOfDraw.size(), false));

	for (int i = 0; i < desOfDraw.size(); i++) {
		for (int j = (i + 1); j < desOfDraw.size(); j++) {
			bool breakOrNot = false;
			for (int k = 0; k<desOfDraw.size(); k++) {
				if (tmp_group[j][k]) {
					breakOrNot = true;
					break;
				}
			}
			if (!breakOrNot && colorOfContour[i] == colorOfContour[j] && (!(desOfDraw[i].size()*0.8 > desOfDraw[j].size() || desOfDraw[j].size()*0.8 > desOfDraw[i].size()))) {
				comp compDes(2, false, 1, inputSize, desOfDrawReverse[j], desOfDraw[i], samplepointsOfDrawReverse[j], samplepointsOfDraw[i], j, i, totalFood, totalFoodImg);
				if (compDes.fragList2().size() > 0) {
					tmp_group[i][j] = true;
					tmp_group[j][i] = true;
					/*
					Mat newWarpMat = compDes.fragList2()[0].warpMatrix.clone();
					Point tmp_center = centerOfContour[i];
					double newX = newWarpMat.at<double>(0, 0) * tmp_center.x + newWarpMat.at<double>(0, 1) * tmp_center.x + newWarpMat.at<double>(0, 2);
					double newY = newWarpMat.at<double>(1, 0) * tmp_center.y + newWarpMat.at<double>(1, 1) * tmp_center.y + newWarpMat.at<double>(1, 2);

					newWarpMat.at<double>(0, 0) = 1;
					newWarpMat.at<double>(0, 1) = 0;
					newWarpMat.at<double>(1, 0) = 0;
					newWarpMat.at<double>(1, 1) = 1;
					newWarpMat.at<double>(0, 2) = centerOfContour[j].x - centerOfContour[i].x;
					newWarpMat.at<double>(1, 2) = centerOfContour[j].y - centerOfContour[i].y;
					groupWarpMatOfContour[j] = newWarpMat.clone();
					*/
					groupWarpMatOfContour[j] = compDes.fragList2()[0].warpMatrix.clone();
				}
			}
		}
	}

	vector<int> gIdx(desOfDraw.size(), -1);
	vector<int> tmpIdx;
	int gcount = 0;
	for (int i = 0; i < desOfDraw.size(); i++) {
		if (gIdx[i] == -1) {
			gIdx[i] = gcount;
			tmpIdx.push_back(i);
			groupIdxOfContour[i] = gcount;

			for (int j = (i + 1); j < desOfDraw.size(); j++) {
				if (tmp_group[i][j]) {
					gIdx[j] = gcount;
					tmpIdx.push_back(j);
					groupIdxOfContour[j] = gcount;
				}
			}
			groupIdx.push_back(tmpIdx);
			gcount++;
		}
		tmpIdx.clear();
	}
}

// get the top, left, bottom, right
void getBorder(Mat &image, int& top, int& left, int& width, int& height) {
	int bottom, right;

	bool findIt = false; // find left
	for (int j = 0; j < image.cols; j++) {
		for (int i = 0; i < image.rows; i++) {
			if (image.at<Vec4b>(i, j)[3] != 0)
			{
				findIt = true;
				left = j;
				break;
			}
		}
		if (findIt)
			break;
	}
	findIt = false; // find right
	for (int j = image.cols - 1; j >= 0; j--) {
		for (int i = 0; i < image.rows; i++) {
			if (image.at<Vec4b>(i, j)[3] != 0)
			{
				findIt = true;
				right = j;
				break;
			}
		}
		if (findIt)
			break;
	}
	findIt = false; // find top
	for (int i = 0; i < image.rows; i++) {
		for (int j = left; j <= right; j++) {
			if (image.at<Vec4b>(i, j)[3] != 0)
			{
				findIt = true;
				top = i;
				break;
			}
		}
		if (findIt)
			break;
	}
	findIt = false; // find bottom
	for (int i = image.rows - 1; i >= 0; i--) {
		for (int j = left; j <= right; j++) {
			if (image.at<Vec4b>(i, j)[3] != 0)
			{
				findIt = true;
				bottom = i;
				break;
			}
		}
		if (findIt)
			break;
	}

	width = right - left + 1;
	height = bottom - top + 1;
}

// reverse vVector<Point> with center
void reversePoint(Point center, vector<Point>& contourPoint) {
	int dist;
	for (int i = 0; i < contourPoint.size(); i++) {
		dist = contourPoint[i].x - center.x;
		contourPoint[i].x -= 2 * dist;
	}
}

// add image with transparent background
Mat addTransparent(Mat &bg, Mat &fg)
{
	Mat result;
	bg.copyTo(result);
	for (int y = 0; y < bg.rows; ++y)
	{
		int fY = y;
		if (fY >= fg.rows) break;
		for (int x = 0; x < bg.cols; ++x)
		{
			int fX = x;
			if (fX >= fg.cols) break;
			double Fopacity = ((double)fg.data[fY * fg.step + fX * fg.channels() + 3]) / 255; // opacity of the foreground pixel

			for (int c = 0; Fopacity > 0 && c < result.channels(); ++c) // combine the background and foreground pixel
			{
				unsigned char foregroundPx = fg.data[fY * fg.step + fX * fg.channels() + c];
				unsigned char backgroundPx = bg.data[y * bg.step + x * bg.channels() + c];
				result.data[y*result.step + result.channels()*x + c] = backgroundPx * (1. - Fopacity) + foregroundPx * Fopacity;

			}
		}
	}
	return result;
}

// get subPointSeq
vector<Point> subPointSeq(vector<Point> inputSeq, int startIndex, int range)
{
	vector<Point> result;

	for (int i = 0; i < range; i++)
	{
		result.push_back(inputSeq[(startIndex + i) % inputSeq.size()]);
	}
	return result;
}

// canny edge detection for each channel
Mat cannyThreeCh(Mat input, bool mode)
{
	vector<Mat> channels;

	split(input, channels);

	Mat B = channels[0];
	Mat G = channels[1];
	Mat R = channels[2];
	Mat A = channels[3];

	for (int i = 0; i < input.rows; i++)
	{
		for (int j = 0; j < input.cols; j++)
		{
			if (A.at<uchar>(i, j) == 0)
			{
				B.at<uchar>(i, j) = 0;
				G.at<uchar>(i, j) = 0;
				R.at<uchar>(i, j) = 0;

			}
		}
	}

	int lowTh;
	int highTh;

	if (mode)
	{
		lowTh = 50;
		highTh = 150;
	}
	else
	{
		lowTh = 250;
		highTh = 750;
	}

	Mat cannyB, cannyG, cannyR, cannyA;

	Canny(B, cannyB, lowTh, highTh, 3);
	Canny(G, cannyG, lowTh, highTh, 3);
	Canny(R, cannyR, lowTh, highTh, 3);
	Canny(A, cannyA, lowTh, highTh, 3);
	Mat cannyColor;

	bitwise_or(cannyB, cannyG, cannyColor);
	bitwise_or(cannyColor, cannyR, cannyColor);
	bitwise_or(cannyColor, cannyA, cannyColor);

	return cannyColor;
}

// get all files in the dir
int getdir(string dir, vector<string> &files)
{
	DIR *dp;
	struct dirent *dirp;
	if ((dp = opendir(dir.c_str())) == NULL)
	{
		return errno;
	}
	while ((dirp = readdir(dp)) != NULL)
	{
		files.push_back(string(dirp->d_name));
	}
	closedir(dp);

	files.erase(files.begin(), files.begin() + 2);
	return 0;
}

//segment image with alpha value
Mat alphaBinary(Mat input)
{
	Mat alphaOrNot = Mat::zeros(input.size(), CV_32S);
	for (int i = 0; i < input.cols; i++)
	{
		for (int j = 0; j < input.rows; j++)
		{
			Vec4b & bgra = input.at<Vec4b>(j, i);
			if (bgra[3] != 0) // not transparency
				alphaOrNot.at<int>(j, i) = 1;
			else
				alphaOrNot.at<int>(j, i) = 0;

		}
	}
	return alphaOrNot;
}

//scale img icon
void scaleIconImg(vector<Mat> tmpFoodImg)
{
	///*
	for (int i = 0; i < samplepointsOfFood.size(); i++)
	{
		int tmp_top, tmp_left, tmp_width, tmp_height;
		getBorder(tmpFoodImg[i], tmp_top, tmp_left, tmp_width, tmp_height);
		Mat cutMat = tmpFoodImg[i](Rect(tmp_left, tmp_top, tmp_width, tmp_height));

		double w_scale = (double)tmpFoodImg[i].cols / (double)tmp_width;
		double h_scale = (double)tmpFoodImg[i].rows / (double)tmp_height;
		double small_scale;
		if (w_scale < h_scale)
			small_scale = w_scale;
		else
			small_scale = h_scale;
		small_scale *= 0.7;

		Size newS = Size(cutMat.cols*small_scale, cutMat.rows*small_scale);
		resize(cutMat, cutMat, newS);

		Size iconSize = Size(tmpFoodImg[i].cols * 0.8, tmpFoodImg[i].rows * 0.8);
		Mat finalMat = Mat::zeros(iconSize, CV_8UC4);
		cutMat.copyTo(finalMat(cv::Rect((finalMat.cols - cutMat.cols) / 2, (finalMat.rows - cutMat.rows) / 2, cutMat.cols, cutMat.rows)));
	
		modifyFoodImg[totalFood[i].fileName] = finalMat.clone();
		imwrite("ImagTest/" + to_string(i) + ".png", finalMat);
	}
	//*/
}

//return the vector of seqence of error value from small to large
void vecSeqIndex(fragList& input, int newAmount)
{
	sort(input.Element.begin(), input.Element.end(), compareWithCertainKey);
	fragList fragTmp/* = input*/;
	for (int i = 0; i < input.Element.size(); i++)
		fragTmp.Element.push_back(input.Element[i]);

	vector<int> foodTmp;
	int eraseN = 0;
	for (int i = 0; i < fragTmp.Element.size(); i++) {

		bool store = true;
		for (int j = 0; j<foodTmp.size(); j++) {
			if (fragTmp.Element[i].fIndex == foodTmp[j]) {
				store = false;
				break;
			}
		}
		if (store) {
			foodTmp.push_back(fragTmp.Element[i].fIndex);
		}
		else {
			eraseN++;
			input.Element.erase(input.Element.begin() + i - eraseN + 1);
		}
	}

	if (input.Element.size() > newAmount) {
		input.Element.erase(input.Element.begin() + newAmount, input.Element.end());
	}

}

/* --------------------------------Error Function-------------------------------- */
//edge error
double edgeError(Mat draw, Mat food)
{
	Mat drawEdge = cannyThreeCh(draw, true);
	Mat foodEdge = cannyThreeCh(food, true);

	vector<vector<Point>> drawSeqContours;
	vector<Vec4i> hierarchyD;

	vector<vector<Point>> foodSeqContours;
	vector<Vec4i> hierarchyF;

	findContours(drawEdge.clone(), drawSeqContours, hierarchyD, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, Point(0, 0));
	findContours(foodEdge.clone(), foodSeqContours, hierarchyF, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, Point(0, 0));

	int foodPointNum = 0;

	for (int i = 0; i < foodSeqContours.size(); i++)
	{
		foodPointNum += int(foodSeqContours[i].size());
	}
	Mat drawDrawContour = Mat::zeros(draw.size(), CV_32FC3);
	Mat foodDrawContour = Mat::zeros(food.size(), CV_32FC3);

	for (int i = 0; i < drawSeqContours.size(); i++)
		drawContours(drawDrawContour, drawSeqContours, i, Scalar(255, 255, 255), 1, 8);

	//for(int i = 0 ; i < foodSeqContours.size() ; i++)
	//drawContours( foodDrawContour, foodSeqContours, i, Scalar(255, 255, 255), 1, 8);

	Mat drawConGray;
	//Mat foodConGray;

	cvtColor(drawDrawContour, drawConGray, CV_BGR2GRAY);
	//cvtColor(foodDrawContour, foodConGray, CV_BGR2GRAY);

	Mat drawBin = drawConGray > 128;
	//Mat foodBin = foodConGray > 128;

	Mat nonZeroDraw;
	//Mat nonZeroFood;
	findNonZero(drawBin, nonZeroDraw);
	//findNonZero(foodBin, nonZeroFood);

	vector<double> pointDist;

	double score = 0;

	for (int i = 0; i < foodSeqContours.size(); i++)
	{
		for (int j = 0; j < foodSeqContours[i].size(); j++)
		{
			Point locF = foodSeqContours[i][j];

			for (int k = 0; k < nonZeroDraw.rows; k++)
			{
				Point locD = nonZeroDraw.at<Point>(k);
				pointDist.push_back(norm(locF - locD));
			}
			double tmp = *min_element(pointDist.begin(), pointDist.end());
			score += tmp;
			pointDist.clear();
		}
	}
	double denominator = pow(pow((bottomRight.y - topLeft.y), 2) + pow((bottomRight.x - topLeft.x), 2), 0.5);
	return (score / foodPointNum) / denominator;
}

//color errorrefError
double colorError(Mat draw, Mat food)
{
	double score = 0.0;

	Mat drawAlphaBin = alphaBinary(draw);
	Mat foodAlphaBin = alphaBinary(food);

	int orValue;
	double tmp;
	for (int i = 0; i < drawAlphaBin.rows; i++)
	{
		for (int j = 0; j < drawAlphaBin.cols; j++)
		{
			orValue = drawAlphaBin.at<int>(i, j) | foodAlphaBin.at<int>(i, j);

			if (orValue == 1)
			{
				Vec4b pixDraw = draw.at<Vec4b>(i, j);
				Vec4b pixFood = food.at<Vec4b>(i, j);

				tmp = sqrt(pow(pixDraw.val[0] - pixFood.val[0], 2) + pow(pixDraw.val[1] - pixFood.val[1], 2) + pow(pixDraw.val[2] - pixFood.val[2], 2));

				score += tmp;

			}
		}
	}
	double denominator = pow(255, 1 / 3);
	denominator *= 255;
	return (score / (draw.rows*draw.cols)) / denominator;
}

//reference error
double refError(Mat draw, Mat food)
{
	Mat drawEdge = cannyThreeCh(draw, true);
	Mat foodEdge = cannyThreeCh(food, false);

	vector<vector<Point>> drawSeqContours;
	vector<Vec4i> hierarchyD;

	vector<vector<Point>> foodSeqContours;
	vector<Vec4i> hierarchyF;

	findContours(drawEdge.clone(), drawSeqContours, hierarchyD, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, Point(0, 0));
	findContours(foodEdge.clone(), foodSeqContours, hierarchyF, CV_RETR_CCOMP, CV_CHAIN_APPROX_NONE, Point(0, 0));

	vector<vector<Point>> disjointContour;

	for (int i = 0; i < drawSeqContours.size(); i++)
		if (drawSeqContours[i].size()>70 && hierarchyD[i][3] != -1)
			disjointContour.push_back(drawSeqContours[i]);

	sort(disjointContour.begin(), disjointContour.end(), compareContourSize);

	int drawPointNum = 0;

	for (int i = 0; i < drawSeqContours.size(); i++)
	{
		drawPointNum += int(drawSeqContours[i].size());
	}

	Mat drawDrawContour = Mat::zeros(draw.size(), CV_32FC3);
	Mat foodDrawContour = Mat::zeros(food.size(), CV_32FC3);

	for (int i = 0; i < drawSeqContours.size(); i++)
		drawContours(drawDrawContour, drawSeqContours, i, Scalar(255, 255, 255), 1, 8);

	for (int i = 0; i < foodSeqContours.size(); i++)
		drawContours(foodDrawContour, foodSeqContours, i, Scalar(255, 255, 255), 1, 8);

	//Mat drawConGray;
	Mat foodConGray;

	//cvtColor(drawDrawContour, drawConGray, CV_BGR2GRAY);
	cvtColor(foodDrawContour, foodConGray, CV_BGR2GRAY);

	//Mat drawBin = drawConGray > 128;
	Mat foodBin = foodConGray > 128;

	//Mat nonZeroDraw;
	Mat nonZeroFood;
	//findNonZero(drawBin, nonZeroDraw);
	findNonZero(foodBin, nonZeroFood);

	int thrC = 3;
	double score = 0;
	double contourErr = 0;
	vector<double> pointDist;
	vector<double> perContourErr;

	for (int i = 0; i < disjointContour.size(); i++)
	{
		for (int j = 0; j < disjointContour[i].size(); j++)
		{
			Point locD = disjointContour[i][j];

			for (int k = 0; k < nonZeroFood.rows; k++)
			{
				Point locF = nonZeroFood.at<Point>(k);
				pointDist.push_back(norm(locF - locD));
			}
			double tmp = 0;
			if (pointDist.size() != 0)
				tmp = *min_element(pointDist.begin(), pointDist.end());
			contourErr += tmp;
			score += tmp;
			pointDist.clear();
		}
		perContourErr.push_back(contourErr / disjointContour[i].size());
		contourErr = 0;
	}
	double denominator = pow(pow((bottomRight.y - topLeft.y), 2) + pow((bottomRight.x - topLeft.x), 2), 0.5);
	return (score / drawPointNum) / denominator;
}

//intersection error
double interError(Mat draw, Mat food)
{
	Mat drawAlphaBin = alphaBinary(draw);
	Mat foodAlphaBin = alphaBinary(food);
	int intersectionArea = 0;
	int unionArea = 0;

	for (int i = 0; i < foodAlphaBin.cols; i++)
	{
		for (int j = 0; j < foodAlphaBin.rows; j++)
		{
			if (drawAlphaBin.at<int>(i, j) == 1 || foodAlphaBin.at<int>(i, j) == 1)
			{
				unionArea++;
				if (drawAlphaBin.at<int>(i, j) == 1 && foodAlphaBin.at<int>(i, j) == 1)
					intersectionArea++;
			}
		}
	}

	double ratio = (double)intersectionArea / (double)unionArea;
	return (1 - ratio);
}

// iError with two contour
double iContourError(vector<Point> contour1, vector<Point> contour2, Mat food)
{
	Mat drawGray, foodGray;
	//cvtColor(food, foodGray, CV_BGR2GRAY);

	Mat drawNZ, foodNZ;
	findNonZero(food, foodNZ);

	double minDist = 0.0;
	double totalDist = 0.0;

	for (int c = 0; c < contour1.size(); c++)
	{
		for (int i = 0; i < foodNZ.rows; i++)
		{
			Point locF = foodNZ.at<Point>(i);
			double dist = sqrt(pow(locF.x - contour1[c].x, 2) + pow(locF.y - contour1[c].y, 2));
			if (i == 0)
				minDist = dist;
			else
				if (minDist>dist)
					minDist = dist;
		}
		totalDist += minDist;
	}

	for (int c = 0; c < contour2.size(); c++)
	{
		for (int i = 0; i < foodNZ.rows; i++)
		{
			Point locF = foodNZ.at<Point>(i);
			double dist = sqrt(pow(locF.x - contour2[c].x, 2) + pow(locF.y - contour2[c].y, 2));
			if (i == 0)
				minDist = dist;
			else
				if (minDist>dist)
					minDist = dist;
		}
		totalDist += minDist;
	}

	double totalPoints = (double)(contour1.size() + contour2.size());

	return totalDist / totalPoints;
}

/* --------------------------------Comparison Value Function-------------------------------- */
bool compareContourSize(vector<Point> contour1, vector<Point> contour2) {
	size_t i = contour1.size();
	size_t j = contour2.size();
	return (i > j);
}

bool compareWithCertainKey(frag input1, frag input2)
{
	double i = input1.iError;
	double j = input2.iError;
	return(i<j);
}

bool compareWithTotalError(leafNode input1, leafNode input2)
{
	double i = input1.totalError;
	double j = input2.totalError;
	return(i<j);
}

/* --------------------------------Comparison Contour Function-------------------------------- */
// compare contour with descriptor and sample point
void doCompare(bool isReverse, fragList& pairSeq, Mat& userDraw, vector<vector<Mat> >& descriptor, vector<vector<Point> >& sampleP, int contourIdx, int recipeIdx, recipe& rec) {

	fragList tmpPairSeq;
	int foodImgIdx = rec.foodIndex()[recipeIdx];
	int foodType = rec.foodType()[recipeIdx];
	int recipeSize = rec.size;

	if (!(foodType == 1 && 3 * sampleP[foodImgIdx].size() < samplepointsOfDraw[contourIdx].size())) {
		if (!isReverse) {
			comp compDes(0, false, 0.4, inputSize, desOfDraw[contourIdx], descriptor[foodImgIdx], samplepointsOfDraw[contourIdx], sampleP[foodImgIdx], contourIdx, foodImgIdx, totalFood, totalFoodImg);
			tmpPairSeq.Element = compDes.fragList2();

			if (tmpPairSeq.Element.size() > 0)
			{
				for (int k = 0; k < tmpPairSeq.Element.size(); k++)
				{
					tmpPairSeq.Element[k].isReverse = false;
					tmpPairSeq.Element[k].setError(edgeError(userDraw, tmpPairSeq.Element[k].warpImg), colorError(userDraw, tmpPairSeq.Element[k].warpImg), refError(userDraw, tmpPairSeq.Element[k].warpImg), tmpPairSeq.Element[k].iError, tmpPairSeq.Element[k].iErrorRatio1);
					tmpPairSeq.Element[k].recipe_num.resize(recipeSize);
					for (int n = 0; n < recipeSize; n++) {
						tmpPairSeq.Element[k].recipe_num[n] = 0;
					}
					tmpPairSeq.Element[k].recipe_num[recipeIdx] = 1;
					tmpPairSeq.Element[k].rIndex = recipeIdx;
				}
			}
		}
		else {
			comp compDes(0, true, 0.4, inputSize, desOfDraw[contourIdx], descriptor[foodImgIdx], samplepointsOfDraw[contourIdx], sampleP[foodImgIdx], contourIdx, foodImgIdx, totalFood, totalFoodImg);
			tmpPairSeq.Element = compDes.fragList2();

			if (tmpPairSeq.Element.size() > 0)
			{
				for (int k = 0; k < tmpPairSeq.Element.size(); k++)
				{
					tmpPairSeq.Element[k].isReverse = true;
					tmpPairSeq.Element[k].setError(edgeError(userDraw, tmpPairSeq.Element[k].warpImg), colorError(userDraw, tmpPairSeq.Element[k].warpImg), refError(userDraw, tmpPairSeq.Element[k].warpImg), tmpPairSeq.Element[k].iError, tmpPairSeq.Element[k].iErrorRatio1);
					tmpPairSeq.Element[k].recipe_num.resize(recipeSize);
					for (int n = 0; n < recipeSize; n++) {
						tmpPairSeq.Element[k].recipe_num[n] = 0;
					}
					tmpPairSeq.Element[k].recipe_num[recipeIdx] = 1;
					tmpPairSeq.Element[k].rIndex = recipeIdx;
				}
			}
		}
	}

	pairSeq.Element.insert(pairSeq.Element.end(), tmpPairSeq.Element.begin(), tmpPairSeq.Element.end());
}

/* --------------------------------End-------------------------------- */

