/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created: Mon Feb 24 08:34:26 2014
**      by: Qt User Interface Compiler version 5.0.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QDoubleSpinBox>
#include <QtWidgets/QFrame>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTabWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *actionFile;
    QAction *actionNew_Model;
    QAction *actionQuit_Open_Chemical_Physics_Suite;
    QAction *actionDelete_Model;
    QAction *actionOpen_Model;
    QAction *actionSave_Model;
    QWidget *centralWidget;
    QHBoxLayout *horizontalLayout_7;
    QVBoxLayout *verticalLayout_2;
    QPushButton *viewTool;
    QPushButton *selectTool;
    QPushButton *atomTool;
    QPushButton *deleteTool;
    QPushButton *measureTool;
    QPushButton *increaseBondOrderTool;
    QPushButton *decreaseBondOrderTool;
    QSpacerItem *verticalSpacer_2;
    QPushButton *elements;
    QTabWidget *tabWidget;
    QWidget *tab;
    QHBoxLayout *horizontalLayout_2;
    QListWidget *atlasList;
    QWidget *tab_2;
    QHBoxLayout *horizontalLayout_3;
    QTabWidget *tabWidget_2;
    QWidget *DisplayTab;
    QVBoxLayout *verticalLayout_3;
    QLabel *label;
    QComboBox *comboBox_RepresentationStyle;
    QFrame *line;
    QLabel *label_3;
    QComboBox *comboBox_PaintStyle;
    QHBoxLayout *horizontalLayout_5;
    QPushButton *EditPaintColors;
    QSpacerItem *horizontalSpacer;
    QFrame *line_3;
    QLabel *label_4;
    QComboBox *comboBox_LabelStyle;
    QFrame *line_4;
    QSpacerItem *verticalSpacer_3;
    QWidget *FFSetupTab;
    QVBoxLayout *verticalLayout_7;
    QLabel *label_7;
    QComboBox *comboBox_Typing;
    QHBoxLayout *horizontalLayout;
    QPushButton *pushButton_AssignAtomTypes;
    QSpacerItem *horizontalSpacer_2;
    QFrame *line_5;
    QLabel *label_8;
    QHBoxLayout *horizontalLayout_6;
    QLabel *label_9;
    QDoubleSpinBox *netChargeSpinBox;
    QPushButton *QEQpushButton;
    QSpacerItem *horizontalSpacer_4;
    QFrame *line_6;
    QSpacerItem *verticalSpacer_7;
    QWidget *ViewerOptionsTab;
    QVBoxLayout *verticalLayout;
    QLabel *label_2;
    QComboBox *comboBox_ProjectionMethod;
    QFrame *line_2;
    QSpacerItem *verticalSpacer;
    QWidget *tab_3;
    QVBoxLayout *verticalLayout_6;
    QPushButton *toggleBitmap;
    QSpacerItem *verticalSpacer_5;
    QHBoxLayout *horizontalLayout_4;
    QVBoxLayout *verticalLayout_4;
    QLabel *label_5;
    QSpinBox *spinBox_height;
    QVBoxLayout *verticalLayout_5;
    QLabel *label_6;
    QSpinBox *spinBox_width;
    QPushButton *createBitmapGrid;
    QSpacerItem *verticalSpacer_6;
    QPushButton *pushButton;
    QSpacerItem *verticalSpacer_4;
    QMenuBar *menuBar;
    QMenu *menuFile;
    QStatusBar *statusBar;
    QToolBar *mainToolBar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(681, 614);
        MainWindow->setMinimumSize(QSize(400, 400));
        actionFile = new QAction(MainWindow);
        actionFile->setObjectName(QStringLiteral("actionFile"));
        actionNew_Model = new QAction(MainWindow);
        actionNew_Model->setObjectName(QStringLiteral("actionNew_Model"));
        actionQuit_Open_Chemical_Physics_Suite = new QAction(MainWindow);
        actionQuit_Open_Chemical_Physics_Suite->setObjectName(QStringLiteral("actionQuit_Open_Chemical_Physics_Suite"));
        actionDelete_Model = new QAction(MainWindow);
        actionDelete_Model->setObjectName(QStringLiteral("actionDelete_Model"));
        actionOpen_Model = new QAction(MainWindow);
        actionOpen_Model->setObjectName(QStringLiteral("actionOpen_Model"));
        actionSave_Model = new QAction(MainWindow);
        actionSave_Model->setObjectName(QStringLiteral("actionSave_Model"));
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        horizontalLayout_7 = new QHBoxLayout(centralWidget);
        horizontalLayout_7->setSpacing(6);
        horizontalLayout_7->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_7->setObjectName(QStringLiteral("horizontalLayout_7"));
        verticalLayout_2 = new QVBoxLayout();
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setObjectName(QStringLiteral("verticalLayout_2"));
        viewTool = new QPushButton(centralWidget);
        viewTool->setObjectName(QStringLiteral("viewTool"));
        viewTool->setMaximumSize(QSize(80, 16777215));
        viewTool->setCheckable(true);
        viewTool->setChecked(true);
        viewTool->setAutoExclusive(true);
        viewTool->setDefault(false);
        viewTool->setFlat(false);

        verticalLayout_2->addWidget(viewTool);

        selectTool = new QPushButton(centralWidget);
        selectTool->setObjectName(QStringLiteral("selectTool"));
        selectTool->setMaximumSize(QSize(80, 16777215));
        selectTool->setCheckable(true);
        selectTool->setAutoExclusive(true);

        verticalLayout_2->addWidget(selectTool);

        atomTool = new QPushButton(centralWidget);
        atomTool->setObjectName(QStringLiteral("atomTool"));
        atomTool->setMaximumSize(QSize(80, 16777215));
        atomTool->setCheckable(true);
        atomTool->setAutoExclusive(true);

        verticalLayout_2->addWidget(atomTool);

        deleteTool = new QPushButton(centralWidget);
        deleteTool->setObjectName(QStringLiteral("deleteTool"));
        deleteTool->setMaximumSize(QSize(80, 16777215));
        deleteTool->setCheckable(true);
        deleteTool->setAutoExclusive(true);

        verticalLayout_2->addWidget(deleteTool);

        measureTool = new QPushButton(centralWidget);
        measureTool->setObjectName(QStringLiteral("measureTool"));
        measureTool->setMaximumSize(QSize(80, 16777215));
        measureTool->setCheckable(true);
        measureTool->setAutoExclusive(true);

        verticalLayout_2->addWidget(measureTool);

        increaseBondOrderTool = new QPushButton(centralWidget);
        increaseBondOrderTool->setObjectName(QStringLiteral("increaseBondOrderTool"));
        increaseBondOrderTool->setMaximumSize(QSize(80, 16777215));
        increaseBondOrderTool->setCheckable(true);
        increaseBondOrderTool->setAutoExclusive(true);

        verticalLayout_2->addWidget(increaseBondOrderTool);

        decreaseBondOrderTool = new QPushButton(centralWidget);
        decreaseBondOrderTool->setObjectName(QStringLiteral("decreaseBondOrderTool"));
        decreaseBondOrderTool->setMaximumSize(QSize(80, 16777215));
        decreaseBondOrderTool->setCheckable(true);
        decreaseBondOrderTool->setAutoExclusive(true);

        verticalLayout_2->addWidget(decreaseBondOrderTool);

        verticalSpacer_2 = new QSpacerItem(65, 228, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_2->addItem(verticalSpacer_2);

        elements = new QPushButton(centralWidget);
        elements->setObjectName(QStringLiteral("elements"));
        elements->setMaximumSize(QSize(80, 16777215));
        elements->setCheckable(false);

        verticalLayout_2->addWidget(elements);


        horizontalLayout_7->addLayout(verticalLayout_2);

        tabWidget = new QTabWidget(centralWidget);
        tabWidget->setObjectName(QStringLiteral("tabWidget"));
        tabWidget->setMinimumSize(QSize(200, 0));
        tabWidget->setTabShape(QTabWidget::Rounded);
        tabWidget->setUsesScrollButtons(true);
        tabWidget->setTabsClosable(false);
        tabWidget->setMovable(false);
        tab = new QWidget();
        tab->setObjectName(QStringLiteral("tab"));
        horizontalLayout_2 = new QHBoxLayout(tab);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        atlasList = new QListWidget(tab);
        atlasList->setObjectName(QStringLiteral("atlasList"));

        horizontalLayout_2->addWidget(atlasList);

        tabWidget->addTab(tab, QString());
        tab_2 = new QWidget();
        tab_2->setObjectName(QStringLiteral("tab_2"));
        horizontalLayout_3 = new QHBoxLayout(tab_2);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        tabWidget_2 = new QTabWidget(tab_2);
        tabWidget_2->setObjectName(QStringLiteral("tabWidget_2"));
        DisplayTab = new QWidget();
        DisplayTab->setObjectName(QStringLiteral("DisplayTab"));
        verticalLayout_3 = new QVBoxLayout(DisplayTab);
        verticalLayout_3->setSpacing(6);
        verticalLayout_3->setContentsMargins(11, 11, 11, 11);
        verticalLayout_3->setObjectName(QStringLiteral("verticalLayout_3"));
        label = new QLabel(DisplayTab);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout_3->addWidget(label);

        comboBox_RepresentationStyle = new QComboBox(DisplayTab);
        comboBox_RepresentationStyle->setObjectName(QStringLiteral("comboBox_RepresentationStyle"));

        verticalLayout_3->addWidget(comboBox_RepresentationStyle);

        line = new QFrame(DisplayTab);
        line->setObjectName(QStringLiteral("line"));
        line->setFrameShape(QFrame::HLine);
        line->setFrameShadow(QFrame::Sunken);

        verticalLayout_3->addWidget(line);

        label_3 = new QLabel(DisplayTab);
        label_3->setObjectName(QStringLiteral("label_3"));

        verticalLayout_3->addWidget(label_3);

        comboBox_PaintStyle = new QComboBox(DisplayTab);
        comboBox_PaintStyle->setObjectName(QStringLiteral("comboBox_PaintStyle"));
        comboBox_PaintStyle->setMinimumSize(QSize(230, 0));

        verticalLayout_3->addWidget(comboBox_PaintStyle);

        horizontalLayout_5 = new QHBoxLayout();
        horizontalLayout_5->setSpacing(6);
        horizontalLayout_5->setObjectName(QStringLiteral("horizontalLayout_5"));
        EditPaintColors = new QPushButton(DisplayTab);
        EditPaintColors->setObjectName(QStringLiteral("EditPaintColors"));

        horizontalLayout_5->addWidget(EditPaintColors);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_5->addItem(horizontalSpacer);


        verticalLayout_3->addLayout(horizontalLayout_5);

        line_3 = new QFrame(DisplayTab);
        line_3->setObjectName(QStringLiteral("line_3"));
        line_3->setFrameShape(QFrame::HLine);
        line_3->setFrameShadow(QFrame::Sunken);

        verticalLayout_3->addWidget(line_3);

        label_4 = new QLabel(DisplayTab);
        label_4->setObjectName(QStringLiteral("label_4"));

        verticalLayout_3->addWidget(label_4);

        comboBox_LabelStyle = new QComboBox(DisplayTab);
        comboBox_LabelStyle->setObjectName(QStringLiteral("comboBox_LabelStyle"));

        verticalLayout_3->addWidget(comboBox_LabelStyle);

        line_4 = new QFrame(DisplayTab);
        line_4->setObjectName(QStringLiteral("line_4"));
        line_4->setFrameShape(QFrame::HLine);
        line_4->setFrameShadow(QFrame::Sunken);

        verticalLayout_3->addWidget(line_4);

        verticalSpacer_3 = new QSpacerItem(20, 160, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_3->addItem(verticalSpacer_3);

        tabWidget_2->addTab(DisplayTab, QString());
        FFSetupTab = new QWidget();
        FFSetupTab->setObjectName(QStringLiteral("FFSetupTab"));
        verticalLayout_7 = new QVBoxLayout(FFSetupTab);
        verticalLayout_7->setSpacing(6);
        verticalLayout_7->setContentsMargins(11, 11, 11, 11);
        verticalLayout_7->setObjectName(QStringLiteral("verticalLayout_7"));
        label_7 = new QLabel(FFSetupTab);
        label_7->setObjectName(QStringLiteral("label_7"));

        verticalLayout_7->addWidget(label_7);

        comboBox_Typing = new QComboBox(FFSetupTab);
        comboBox_Typing->setObjectName(QStringLiteral("comboBox_Typing"));

        verticalLayout_7->addWidget(comboBox_Typing);

        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setSpacing(6);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        pushButton_AssignAtomTypes = new QPushButton(FFSetupTab);
        pushButton_AssignAtomTypes->setObjectName(QStringLiteral("pushButton_AssignAtomTypes"));
        pushButton_AssignAtomTypes->setMinimumSize(QSize(150, 0));

        horizontalLayout->addWidget(pushButton_AssignAtomTypes);

        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);


        verticalLayout_7->addLayout(horizontalLayout);

        line_5 = new QFrame(FFSetupTab);
        line_5->setObjectName(QStringLiteral("line_5"));
        line_5->setFrameShape(QFrame::HLine);
        line_5->setFrameShadow(QFrame::Sunken);

        verticalLayout_7->addWidget(line_5);

        label_8 = new QLabel(FFSetupTab);
        label_8->setObjectName(QStringLiteral("label_8"));

        verticalLayout_7->addWidget(label_8);

        horizontalLayout_6 = new QHBoxLayout();
        horizontalLayout_6->setSpacing(6);
        horizontalLayout_6->setObjectName(QStringLiteral("horizontalLayout_6"));
        label_9 = new QLabel(FFSetupTab);
        label_9->setObjectName(QStringLiteral("label_9"));

        horizontalLayout_6->addWidget(label_9);

        netChargeSpinBox = new QDoubleSpinBox(FFSetupTab);
        netChargeSpinBox->setObjectName(QStringLiteral("netChargeSpinBox"));
        netChargeSpinBox->setMinimumSize(QSize(70, 0));
        netChargeSpinBox->setMinimum(-100);
        netChargeSpinBox->setMaximum(100);
        netChargeSpinBox->setSingleStep(0.01);

        horizontalLayout_6->addWidget(netChargeSpinBox);

        QEQpushButton = new QPushButton(FFSetupTab);
        QEQpushButton->setObjectName(QStringLiteral("QEQpushButton"));
        QEQpushButton->setMinimumSize(QSize(150, 0));

        horizontalLayout_6->addWidget(QEQpushButton);

        horizontalSpacer_4 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_6->addItem(horizontalSpacer_4);


        verticalLayout_7->addLayout(horizontalLayout_6);

        line_6 = new QFrame(FFSetupTab);
        line_6->setObjectName(QStringLiteral("line_6"));
        line_6->setFrameShape(QFrame::HLine);
        line_6->setFrameShadow(QFrame::Sunken);

        verticalLayout_7->addWidget(line_6);

        verticalSpacer_7 = new QSpacerItem(68, 213, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_7->addItem(verticalSpacer_7);

        tabWidget_2->addTab(FFSetupTab, QString());

        horizontalLayout_3->addWidget(tabWidget_2);

        tabWidget->addTab(tab_2, QString());
        ViewerOptionsTab = new QWidget();
        ViewerOptionsTab->setObjectName(QStringLiteral("ViewerOptionsTab"));
        verticalLayout = new QVBoxLayout(ViewerOptionsTab);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label_2 = new QLabel(ViewerOptionsTab);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout->addWidget(label_2);

        comboBox_ProjectionMethod = new QComboBox(ViewerOptionsTab);
        comboBox_ProjectionMethod->setObjectName(QStringLiteral("comboBox_ProjectionMethod"));

        verticalLayout->addWidget(comboBox_ProjectionMethod);

        line_2 = new QFrame(ViewerOptionsTab);
        line_2->setObjectName(QStringLiteral("line_2"));
        line_2->setFrameShape(QFrame::HLine);
        line_2->setFrameShadow(QFrame::Sunken);

        verticalLayout->addWidget(line_2);

        verticalSpacer = new QSpacerItem(20, 399, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        tabWidget->addTab(ViewerOptionsTab, QString());
        tab_3 = new QWidget();
        tab_3->setObjectName(QStringLiteral("tab_3"));
        verticalLayout_6 = new QVBoxLayout(tab_3);
        verticalLayout_6->setSpacing(6);
        verticalLayout_6->setContentsMargins(11, 11, 11, 11);
        verticalLayout_6->setObjectName(QStringLiteral("verticalLayout_6"));
        toggleBitmap = new QPushButton(tab_3);
        toggleBitmap->setObjectName(QStringLiteral("toggleBitmap"));
        toggleBitmap->setCheckable(true);

        verticalLayout_6->addWidget(toggleBitmap);

        verticalSpacer_5 = new QSpacerItem(20, 74, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_5);

        horizontalLayout_4 = new QHBoxLayout();
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setObjectName(QStringLiteral("horizontalLayout_4"));
        verticalLayout_4 = new QVBoxLayout();
        verticalLayout_4->setSpacing(6);
        verticalLayout_4->setObjectName(QStringLiteral("verticalLayout_4"));
        label_5 = new QLabel(tab_3);
        label_5->setObjectName(QStringLiteral("label_5"));

        verticalLayout_4->addWidget(label_5);

        spinBox_height = new QSpinBox(tab_3);
        spinBox_height->setObjectName(QStringLiteral("spinBox_height"));
        spinBox_height->setMinimum(1);
        spinBox_height->setMaximum(100);

        verticalLayout_4->addWidget(spinBox_height);


        horizontalLayout_4->addLayout(verticalLayout_4);

        verticalLayout_5 = new QVBoxLayout();
        verticalLayout_5->setSpacing(6);
        verticalLayout_5->setObjectName(QStringLiteral("verticalLayout_5"));
        label_6 = new QLabel(tab_3);
        label_6->setObjectName(QStringLiteral("label_6"));

        verticalLayout_5->addWidget(label_6);

        spinBox_width = new QSpinBox(tab_3);
        spinBox_width->setObjectName(QStringLiteral("spinBox_width"));
        spinBox_width->setMinimum(1);
        spinBox_width->setMaximum(100);

        verticalLayout_5->addWidget(spinBox_width);


        horizontalLayout_4->addLayout(verticalLayout_5);


        verticalLayout_6->addLayout(horizontalLayout_4);

        createBitmapGrid = new QPushButton(tab_3);
        createBitmapGrid->setObjectName(QStringLiteral("createBitmapGrid"));

        verticalLayout_6->addWidget(createBitmapGrid);

        verticalSpacer_6 = new QSpacerItem(20, 74, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_6);

        pushButton = new QPushButton(tab_3);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        verticalLayout_6->addWidget(pushButton);

        verticalSpacer_4 = new QSpacerItem(20, 118, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout_6->addItem(verticalSpacer_4);

        tabWidget->addTab(tab_3, QString());

        horizontalLayout_7->addWidget(tabWidget);

        MainWindow->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(MainWindow);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 681, 22));
        menuFile = new QMenu(menuBar);
        menuFile->setObjectName(QStringLiteral("menuFile"));
        MainWindow->setMenuBar(menuBar);
        statusBar = new QStatusBar(MainWindow);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        MainWindow->setStatusBar(statusBar);
        mainToolBar = new QToolBar(MainWindow);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        MainWindow->addToolBar(Qt::TopToolBarArea, mainToolBar);

        menuBar->addAction(menuFile->menuAction());
        menuFile->addAction(actionNew_Model);
        menuFile->addAction(actionOpen_Model);
        menuFile->addAction(actionSave_Model);
        menuFile->addAction(actionDelete_Model);
        menuFile->addSeparator();
        menuFile->addAction(actionQuit_Open_Chemical_Physics_Suite);
        mainToolBar->addAction(actionOpen_Model);
        mainToolBar->addAction(actionSave_Model);

        retranslateUi(MainWindow);

        tabWidget->setCurrentIndex(1);
        tabWidget_2->setCurrentIndex(0);
        comboBox_PaintStyle->setCurrentIndex(1);


        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "Prometheus: The Open Chemical Physics Suite", 0));
        actionFile->setText(QApplication::translate("MainWindow", "File", 0));
        actionNew_Model->setText(QApplication::translate("MainWindow", "New Model...", 0));
        actionQuit_Open_Chemical_Physics_Suite->setText(QApplication::translate("MainWindow", "Quit Prometheus", 0));
        actionDelete_Model->setText(QApplication::translate("MainWindow", "Delete Model", 0));
        actionOpen_Model->setText(QApplication::translate("MainWindow", "Open Model...", 0));
        actionSave_Model->setText(QApplication::translate("MainWindow", "Save Model...", 0));
#ifndef QT_NO_TOOLTIP
        viewTool->setToolTip(QApplication::translate("MainWindow", "View Tool", 0));
#endif // QT_NO_TOOLTIP
        viewTool->setText(QApplication::translate("MainWindow", "View", 0));
#ifndef QT_NO_TOOLTIP
        selectTool->setToolTip(QApplication::translate("MainWindow", "Select Tool", 0));
#endif // QT_NO_TOOLTIP
        selectTool->setText(QApplication::translate("MainWindow", "Select", 0));
#ifndef QT_NO_TOOLTIP
        atomTool->setToolTip(QApplication::translate("MainWindow", "Build Atom Tool", 0));
#endif // QT_NO_TOOLTIP
        atomTool->setText(QApplication::translate("MainWindow", "Atom", 0));
#ifndef QT_NO_TOOLTIP
        deleteTool->setToolTip(QApplication::translate("MainWindow", "Delete Tool", 0));
#endif // QT_NO_TOOLTIP
        deleteTool->setText(QApplication::translate("MainWindow", "Delete", 0));
#ifndef QT_NO_TOOLTIP
        measureTool->setToolTip(QApplication::translate("MainWindow", "Measurement Tool", 0));
#endif // QT_NO_TOOLTIP
        measureTool->setText(QApplication::translate("MainWindow", "Measure", 0));
#ifndef QT_NO_TOOLTIP
        increaseBondOrderTool->setToolTip(QApplication::translate("MainWindow", "Increase Bond Order", 0));
#endif // QT_NO_TOOLTIP
        increaseBondOrderTool->setText(QApplication::translate("MainWindow", "+ BO", 0));
#ifndef QT_NO_TOOLTIP
        decreaseBondOrderTool->setToolTip(QApplication::translate("MainWindow", "Decrease Bond Order", 0));
#endif // QT_NO_TOOLTIP
        decreaseBondOrderTool->setText(QApplication::translate("MainWindow", "- BO", 0));
#ifndef QT_NO_TOOLTIP
        elements->setToolTip(QApplication::translate("MainWindow", "Select Builder Element", 0));
#endif // QT_NO_TOOLTIP
        elements->setText(QApplication::translate("MainWindow", "Elements", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab), QApplication::translate("MainWindow", "Model Overview", 0));
        label->setText(QApplication::translate("MainWindow", "Model Representation Style", 0));
        comboBox_RepresentationStyle->clear();
        comboBox_RepresentationStyle->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Ball and Stick", 0)
         << QApplication::translate("MainWindow", "Dreiding (Licorice)", 0)
         << QApplication::translate("MainWindow", "Space Filling (CPK)", 0)
         << QApplication::translate("MainWindow", "Wireframe (Vector)", 0)
         << QApplication::translate("MainWindow", "Enhanced Wireframe", 0)
        );
        label_3->setText(QApplication::translate("MainWindow", "Model Paint Style", 0));
        comboBox_PaintStyle->clear();
        comboBox_PaintStyle->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Chain ID", 0)
         << QApplication::translate("MainWindow", "Element", 0)
         << QApplication::translate("MainWindow", "Energy - Kinetic", 0)
         << QApplication::translate("MainWindow", "Energy - Potential", 0)
         << QApplication::translate("MainWindow", "Energy - Total", 0)
         << QApplication::translate("MainWindow", "Entropy", 0)
         << QApplication::translate("MainWindow", "Force Field Type", 0)
         << QApplication::translate("MainWindow", "Index", 0)
         << QApplication::translate("MainWindow", "Mass", 0)
         << QApplication::translate("MainWindow", "Molecule Group", 0)
         << QApplication::translate("MainWindow", "Occupancy", 0)
         << QApplication::translate("MainWindow", "Oxidation State", 0)
         << QApplication::translate("MainWindow", "Residue Type", 0)
         << QApplication::translate("MainWindow", "Residue ID", 0)
         << QApplication::translate("MainWindow", "Partial Charge", 0)
         << QApplication::translate("MainWindow", "Position - X", 0)
         << QApplication::translate("MainWindow", "Position - Y", 0)
         << QApplication::translate("MainWindow", "Position - Z", 0)
         << QApplication::translate("MainWindow", "Position - Radial", 0)
         << QApplication::translate("MainWindow", "Pressure", 0)
         << QApplication::translate("MainWindow", "Velocity", 0)
        );
        EditPaintColors->setText(QApplication::translate("MainWindow", "Edit Paint Colors", 0));
        label_4->setText(QApplication::translate("MainWindow", "Atomic Label Style", 0));
        comboBox_LabelStyle->clear();
        comboBox_LabelStyle->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "None", 0)
         << QApplication::translate("MainWindow", "Atom Name", 0)
         << QApplication::translate("MainWindow", "Element", 0)
         << QApplication::translate("MainWindow", "Force Field Type", 0)
         << QApplication::translate("MainWindow", "Partial Charge", 0)
        );
        tabWidget_2->setTabText(tabWidget_2->indexOf(DisplayTab), QApplication::translate("MainWindow", "Display", 0));
        label_7->setText(QApplication::translate("MainWindow", "Force Field Typing", 0));
        comboBox_Typing->clear();
        comboBox_Typing->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "AIREBO      (Adaptive Intermolecular Reactive Emperical Bond Order)", 0)
         << QApplication::translate("MainWindow", "AMBER       (Assisted Model Building with Energy Refinement)", 0)
         << QApplication::translate("MainWindow", "CHARMM   (Chemistry at HARvard Molecular Mechanics)", 0)
         << QApplication::translate("MainWindow", "DREIDING", 0)
         << QApplication::translate("MainWindow", "GAFF         (Generalized AMBER Force Field)", 0)
         << QApplication::translate("MainWindow", "ReaxFF      (Reactive Force Field)", 0)
         << QApplication::translate("MainWindow", "REBO         (Reactive Emperical Bond Order)", 0)
         << QApplication::translate("MainWindow", "UFF           (Universal Force Field)", 0)
        );
        pushButton_AssignAtomTypes->setText(QApplication::translate("MainWindow", "Assign Atom Types", 0));
        label_8->setText(QApplication::translate("MainWindow", "Charge Equilibration (QEq)", 0));
        label_9->setText(QApplication::translate("MainWindow", "Final Net Charge", 0));
        QEQpushButton->setText(QApplication::translate("MainWindow", "Assign Partial Charges", 0));
        tabWidget_2->setTabText(tabWidget_2->indexOf(FFSetupTab), QApplication::translate("MainWindow", "Force Field Setup", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_2), QApplication::translate("MainWindow", "Model Options", 0));
        label_2->setText(QApplication::translate("MainWindow", "Viewer Projection Method", 0));
        comboBox_ProjectionMethod->clear();
        comboBox_ProjectionMethod->insertItems(0, QStringList()
         << QApplication::translate("MainWindow", "Perspective", 0)
         << QApplication::translate("MainWindow", "Orthogonal", 0)
        );
        tabWidget->setTabText(tabWidget->indexOf(ViewerOptionsTab), QApplication::translate("MainWindow", "Viewer Settings", 0));
        toggleBitmap->setText(QApplication::translate("MainWindow", "Toggle Bitmap Mode", 0));
        label_5->setText(QApplication::translate("MainWindow", "Height", 0));
        label_6->setText(QApplication::translate("MainWindow", "Width", 0));
        createBitmapGrid->setText(QApplication::translate("MainWindow", "Create Bitmap Grid", 0));
        pushButton->setText(QApplication::translate("MainWindow", "Print Bitmap", 0));
        tabWidget->setTabText(tabWidget->indexOf(tab_3), QApplication::translate("MainWindow", "Bitmap", 0));
        menuFile->setTitle(QApplication::translate("MainWindow", "File", 0));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
