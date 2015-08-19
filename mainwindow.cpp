#include "mainwindow.h"
#include "window.h"
#include <QMenuBar>
#include <QMenu>
#include <QMessageBox>
#include <QWidgetAction>
#include <QComboBox>
#include <QToolBar>
#include <QSize>

MainWindow::MainWindow()
{
    QMenuBar *menuBar = new QMenuBar;
    QMenu *menuWindow = menuBar->addMenu(tr("&Window"));
	//QToolBar *toolBar = addToolBar(tr("&TEST"));

	//QWidgetAction *widgetAction = new QWidgetAction(this);
	//QComboBox *comboBox = new QComboBox;
	//QAction *openAction;

	//openAction = new QAction(tr("&open"), this);
	//openAction->setShortcut(QKeySequence::Open);
	//openAction->setStatusTip(tr("Open a file."));

	////comboBox->addAction(openAction);
	////comboBox->addAction(openAction);
	////comboBox->addAction(openAction);
	//for (int i = 0; i != 10; ++i)
	//{
	//	//此处的QString：：number用于将i转换为QString  
	//	comboBox->addItem(QString::number(i) + " Item");
	//}
	//comboBox->setMinimumSize(QSize(150,30));
	//widgetAction->setDefaultWidget(comboBox);
	//toolBar->addAction(widgetAction);

    setMenuBar(menuBar);
	setCentralWidget(new Window(this));
	setWindowTitle(tr("Hello GL"));
    //onAddNew();
}

void MainWindow::onAddNew()
{
    if (!centralWidget())
        setCentralWidget(new Window(this));
    else
        QMessageBox::information(0, tr("Cannot add new window"), tr("Already occupied. Undock first."));
}
