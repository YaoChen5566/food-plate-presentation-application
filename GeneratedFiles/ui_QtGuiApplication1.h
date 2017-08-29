/********************************************************************************
** Form generated from reading UI file 'QtGuiApplication1.ui'
**
** Created by: Qt User Interface Compiler version 5.6.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTGUIAPPLICATION1_H
#define UI_QTGUIAPPLICATION1_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QFrame>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QTreeWidget>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtGuiApplication1Class
{
public:
    QAction *actionSelect_an_image;
    QAction *actionExit;
    QAction *actionBackground;
    QWidget *centralWidget;
    QGridLayout *gridLayout_2;
    QHBoxLayout *horizontalLayout_3;
    QVBoxLayout *verticalLayout_2;
    QHBoxLayout *horizontalLayout_2;
    QTabWidget *tabWidget;
    QWidget *tab;
    QHBoxLayout *horizontalLayout;
    QTreeWidget *treeWidget;
    QWidget *tab_2;
    QGridLayout *gridLayout;
    QTreeWidget *treeWidget_2;
    QVBoxLayout *verticalLayout;
    QVBoxLayout *verticalLayout_6;
    QTableWidget *tableWidget;
    QGridLayout *gridLayout_5;
    QTableWidget *tableWidget_2;
    QPushButton *ApplyButton;
    QVBoxLayout *verticalLayout_3;
    QVBoxLayout *verticalLayout_4;
    QTabWidget *tabWidget_2;
    QWidget *tab_3;
    QGridLayout *gridLayout_4;
    QLabel *label_6;
    QWidget *tab_4;
    QGridLayout *gridLayout_3;
    QLabel *label_5;
    QWidget *tab_5;
    QHBoxLayout *horizontalLayout_7;
    QHBoxLayout *horizontalLayout_8;
    QLabel *label_2;
    QFrame *line;
    QLabel *label;
    QTableWidget *tableWidget_6;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *RunRecipeButton;
    QPushButton *RunImageButton;
    QPushButton *ExitButton;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *QtGuiApplication1Class)
    {
        if (QtGuiApplication1Class->objectName().isEmpty())
            QtGuiApplication1Class->setObjectName(QStringLiteral("QtGuiApplication1Class"));
        QtGuiApplication1Class->resize(1097, 678);
        QFont font;
        font.setKerning(true);
        QtGuiApplication1Class->setFont(font);
        actionSelect_an_image = new QAction(QtGuiApplication1Class);
        actionSelect_an_image->setObjectName(QStringLiteral("actionSelect_an_image"));
        QFont font1;
        font1.setPointSize(15);
        font1.setBold(false);
        font1.setWeight(50);
        actionSelect_an_image->setFont(font1);
        actionExit = new QAction(QtGuiApplication1Class);
        actionExit->setObjectName(QStringLiteral("actionExit"));
        QFont font2;
        font2.setPointSize(15);
        actionExit->setFont(font2);
        actionBackground = new QAction(QtGuiApplication1Class);
        actionBackground->setObjectName(QStringLiteral("actionBackground"));
        centralWidget = new QWidget(QtGuiApplication1Class);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout_2 = new QGridLayout(centralWidget);
        gridLayout_2->setSpacing(6);
        gridLayout_2->setContentsMargins(11, 11, 11, 11);
        gridLayout_2->setObjectName(QStringLiteral("gridLayout_2"));
        horizontalLayout_3 = new QHBoxLayout();
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, -1, 0, -1);
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, -1, 0, 0);
        horizontalLayout_2 = new QHBoxLayout();
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setBaseSize(QSize(0, 0));
        QFont font3;
        font3.setFamily(QStringLiteral("Times New Roman"));
        font3.setPointSize(15);
        tabWidget->setFont(font3);
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        horizontalLayout = new QHBoxLayout(tab);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        treeWidget = new QTreeWidget(tab);
        QTreeWidgetItem *__qtreewidgetitem = new QTreeWidgetItem();
        __qtreewidgetitem->setText(0, QStringLiteral("1"));
        treeWidget->setHeaderItem(__qtreewidgetitem);
        treeWidget->setObjectName(QStringLiteral("treeWidget"));
        QFont font4;
        font4.setBold(true);
        font4.setWeight(75);
        treeWidget->setFont(font4);
        treeWidget->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::EditKeyPressed);
        treeWidget->setSelectionMode(QAbstractItemView::NoSelection);
        treeWidget->header()->setStretchLastSection(true);

        horizontalLayout->addWidget(treeWidget);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        gridLayout = new QGridLayout(tab_2);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        treeWidget_2 = new QTreeWidget(tab_2);
        QTreeWidgetItem *__qtreewidgetitem1 = new QTreeWidgetItem();
        __qtreewidgetitem1->setText(0, QStringLiteral("1"));
        treeWidget_2->setHeaderItem(__qtreewidgetitem1);
        treeWidget_2->setObjectName(QStringLiteral("treeWidget_2"));
        treeWidget_2->setFont(font4);
        treeWidget_2->setEditTriggers(QAbstractItemView::DoubleClicked|QAbstractItemView::EditKeyPressed);
        treeWidget_2->setSelectionMode(QAbstractItemView::NoSelection);

        gridLayout->addWidget(treeWidget_2, 0, 0, 1, 1);

        tabWidget->addTab(tab_2, QString());

        horizontalLayout_2->addWidget(tabWidget);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setSpacing(6);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayout_6 = new QVBoxLayout();
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        verticalLayout_6->setContentsMargins(-1, 47, -1, -1);

        verticalLayout->addLayout(verticalLayout_6);

        tableWidget = new QTableWidget(centralWidget);
        tableWidget->setObjectName(QStringLiteral("tableWidget"));
        QFont font5;
        font5.setFamily(QStringLiteral("Times New Roman"));
        font5.setPointSize(15);
        font5.setBold(true);
        font5.setWeight(75);
        tableWidget->setFont(font5);
        tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidget->setSelectionMode(QAbstractItemView::NoSelection);
        tableWidget->setRowCount(0);
        tableWidget->setColumnCount(0);

        verticalLayout->addWidget(tableWidget);

        gridLayout_5 = new QGridLayout();
        gridLayout_5->setSpacing(6);
        gridLayout_5->setObjectName(QStringLiteral("gridLayout_5"));

        verticalLayout->addLayout(gridLayout_5);

        tableWidget_2 = new QTableWidget(centralWidget);
        tableWidget_2->setObjectName(QStringLiteral("tableWidget_2"));
        QFont font6;
        font6.setFamily(QStringLiteral("Times New Roman"));
        font6.setPointSize(15);
        font6.setBold(false);
        font6.setWeight(50);
        tableWidget_2->setFont(font6);
        tableWidget_2->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidget_2->setSelectionMode(QAbstractItemView::NoSelection);
        tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);

        verticalLayout->addWidget(tableWidget_2);

        ApplyButton = new QPushButton(centralWidget);
        ApplyButton->setObjectName(QStringLiteral("ApplyButton"));
        QFont font7;
        font7.setFamily(QStringLiteral("Times New Roman"));
        font7.setPointSize(12);
        ApplyButton->setFont(font7);

        verticalLayout->addWidget(ApplyButton);


        horizontalLayout_2->addLayout(verticalLayout);


        verticalLayout_2->addLayout(horizontalLayout_2);


        horizontalLayout_3->addLayout(verticalLayout_2);

        verticalLayout_3 = new QVBoxLayout();
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        verticalLayout_3->setContentsMargins(0, 0, 0, 0);
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        verticalLayout_4->setContentsMargins(-1, 0, -1, 0);
        tabWidget_2 = new QTabWidget(centralWidget);
        tabWidget_2->setObjectName(QStringLiteral("tabWidget_2"));
        tabWidget_2->setFont(font3);
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        gridLayout_4 = new QGridLayout(tab_3);
        gridLayout_4->setSpacing(6);
        gridLayout_4->setContentsMargins(11, 11, 11, 11);
        gridLayout_4->setObjectName(QStringLiteral("gridLayout_4"));
        label_6 = new QLabel(tab_3);
        label_6->setObjectName(QStringLiteral("label_6"));

        gridLayout_4->addWidget(label_6, 0, 0, 1, 1);

        tabWidget_2->addTab(tab_3, QString());
        tab_4 = new QWidget();
        tab_4->setObjectName(QStringLiteral("tab_4"));
        tab_4->setLayoutDirection(Qt::RightToLeft);
        gridLayout_3 = new QGridLayout(tab_4);
        gridLayout_3->setSpacing(6);
        gridLayout_3->setContentsMargins(11, 11, 11, 11);
        gridLayout_3->setObjectName(QStringLiteral("gridLayout_3"));
        label_5 = new QLabel(tab_4);
        label_5->setObjectName(QStringLiteral("label_5"));
        label_5->setAlignment(Qt::AlignCenter);

        gridLayout_3->addWidget(label_5, 0, 0, 1, 1);

        tabWidget_2->addTab(tab_4, QString());
        tab_5 = new QWidget();
        tab_5->setObjectName(QStringLiteral("tab_5"));
        horizontalLayout_7 = new QHBoxLayout(tab_5);
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        horizontalLayout_8 = new QHBoxLayout();
        horizontalLayout_8->setSpacing(6);
        horizontalLayout_8->setObjectName(QStringLiteral("horizontalLayout_8"));
        label_2 = new QLabel(tab_5);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_8->addWidget(label_2);

        line = new QFrame(tab_5);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::VLine);
        line->setFrameShadow(QFrame::Sunken);

        horizontalLayout_8->addWidget(line);

        label = new QLabel(tab_5);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_8->addWidget(label);


        horizontalLayout_7->addLayout(horizontalLayout_8);

        tabWidget_2->addTab(tab_5, QString());

        verticalLayout_4->addWidget(tabWidget_2);


        verticalLayout_3->addLayout(verticalLayout_4);

        tableWidget_6 = new QTableWidget(centralWidget);
        tableWidget_6->setObjectName(QStringLiteral("tableWidget_6"));
        tableWidget_6->setEditTriggers(QAbstractItemView::NoEditTriggers);
        tableWidget_6->setTabKeyNavigation(false);
        tableWidget_6->setProperty("showDropIndicator", QVariant(false));
        tableWidget_6->setDragDropOverwriteMode(false);
        tableWidget_6->setDefaultDropAction(Qt::CopyAction);
        tableWidget_6->setSelectionMode(QAbstractItemView::NoSelection);

        verticalLayout_3->addWidget(tableWidget_6);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        horizontalLayout_5->setContentsMargins(0, 0, 0, 0);
        RunRecipeButton = new QPushButton(centralWidget);
        RunRecipeButton->setObjectName(QStringLiteral("RunRecipeButton"));
        RunRecipeButton->setFont(font7);

        horizontalLayout_5->addWidget(RunRecipeButton);

        RunImageButton = new QPushButton(centralWidget);
        RunImageButton->setObjectName(QStringLiteral("RunImageButton"));
        RunImageButton->setFont(font7);

        horizontalLayout_5->addWidget(RunImageButton);

        ExitButton = new QPushButton(centralWidget);
        ExitButton->setObjectName(QStringLiteral("ExitButton"));
        ExitButton->setFont(font7);

        horizontalLayout_5->addWidget(ExitButton);


        verticalLayout_3->addLayout(horizontalLayout_5);


        horizontalLayout_3->addLayout(verticalLayout_3);


        gridLayout_2->addLayout(horizontalLayout_3, 0, 0, 1, 1);

        QtGuiApplication1Class->setCentralWidget(centralWidget);
        statusBar = new QStatusBar(QtGuiApplication1Class);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        statusBar->setEnabled(true);
        QtGuiApplication1Class->setStatusBar(statusBar);

        retranslateUi(QtGuiApplication1Class);

        tabWidget->setCurrentIndex(1);
        tabWidget_2->setCurrentIndex(2);


        QMetaObject::connectSlotsByName(QtGuiApplication1Class);
    } // setupUi

    void retranslateUi(QMainWindow *QtGuiApplication1Class)
    {
        QtGuiApplication1Class->setWindowTitle(QApplication::translate("QtGuiApplication1Class", "QtGuiApplication1", 0));
        actionSelect_an_image->setText(QApplication::translate("QtGuiApplication1Class", "select image", 0));
        actionExit->setText(QApplication::translate("QtGuiApplication1Class", "exit", 0));
        actionBackground->setText(QApplication::translate("QtGuiApplication1Class", "change background", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("QtGuiApplication1Class", "RecipeList", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("QtGuiApplication1Class", "FoodList", 0));
        ApplyButton->setText(QApplication::translate("QtGuiApplication1Class", "Apply", 0));
        label_6->setText(QString());
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_3), QApplication::translate("QtGuiApplication1Class", " Image ", 0));
        label_5->setText(QString());
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_4), QApplication::translate("QtGuiApplication1Class", " Result ", 0));
        label_2->setText(QString());
        label->setText(QString());
        tabWidget_2->setTabText(tabWidget_2->indexOf(tab_5), QApplication::translate("QtGuiApplication1Class", "Compare", 0));
        RunRecipeButton->setText(QApplication::translate("QtGuiApplication1Class", "run all recipe", 0));
        RunImageButton->setText(QApplication::translate("QtGuiApplication1Class", "run all image", 0));
        ExitButton->setText(QApplication::translate("QtGuiApplication1Class", "Exit", 0));
    } // retranslateUi

};

namespace Ui {
    class QtGuiApplication1Class: public Ui_QtGuiApplication1Class {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTGUIAPPLICATION1_H
