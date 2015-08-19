#include "glwidget.h"
#include "window.h"
#include "mainwindow.h"
#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QKeyEvent>
#include <QPushButton>
#include <QDesktopWidget>
#include <QApplication>
#include <QMessageBox>
#include <QRadioButton>
#include <qlistwidget.h>
#include <QLabel>
#include <QCheckBox>
#include <QDir>
#include <QFileSystemModel>
#include <QTreeView>
#include <QListView>
#include <QMenu>
#include <QAction>
#include <meshWidget.h>
#include <QStandardItemModel>
#include <QLineEdit>


Window::Window(MainWindow *mw)
    : mainWindow(mw)
{
    glWidget = new GLWidget(this);

    xSlider = createSlider();
    ySlider = createSlider();
    //zSlider = createSlider();

	QDir dir("Meshes");

	if (!dir.exists())
		qWarning("Cannot find the Meshes directory");

	dir.setFilter(/*QDir::Files | */QDir::Hidden | QDir::Dirs | QDir::NoSymLinks);
	dir.setSorting(QDir::Name);
	QFileInfoList fileList = dir.entryInfoList();
	
    connect(xSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(sliderScale(int)));
    connect(glWidget, SIGNAL(sliderScaleChanged(int)), xSlider, SLOT(setValue(int)));
	connect(ySlider, SIGNAL(valueChanged(int)), glWidget, SLOT(sliderStep(int)));
	connect(glWidget, SIGNAL(sliderStepChanged(int)), ySlider, SLOT(setValue(int)));

	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	QHBoxLayout *container = new QHBoxLayout(this);
	QVBoxLayout *listLayout = new QVBoxLayout(this);
	
	QFont ft;
	ft.setBold(true);
	ft.setPointSize(12);
	
	comboBox = new QComboBox(this);
	comboBox->setMaximumSize(QSize(180, 30));
	comboBox->setMinimumSize(QSize(0, 30));
	comboBox->setFont(ft);
	ft.setPointSize(12);
	comboBox->addItem(QString("Camera"), -1);

	connect(comboBox, SIGNAL(currentIndexChanged(int)), glWidget, SLOT(meshChanged(int)));
	emit comboBox->currentIndexChanged(0);
	listLayout->addWidget(comboBox);
	listLayout->addSpacing(10);
	QPushButton * deleteButton = new QPushButton("Delete",this);
	ft.setPointSize(10);
	deleteButton->setFont(ft);
	//deleteButton->setMinimumHeight();
	deleteButton->setMaximumSize(QSize(180, 25));
	deleteButton->setMinimumSize(QSize(0, 25));
	listLayout->addWidget(deleteButton);
	
	connect(deleteButton,SIGNAL(clicked()),glWidget, SLOT(removeCurrentMesh()));

	QRadioButton *nocoGridButton = new QRadioButton("No Coordinate",this);
	QRadioButton *XYGridButton = new QRadioButton("XY Coordinate", this);
	QRadioButton *YZGridButton = new QRadioButton("YZ Coordinate", this);
	QRadioButton *XZGridButton = new QRadioButton("XZ Coordinate", this);

	ft.setPointSize(10);
	nocoGridButton->setFont(ft);
	XYGridButton->setFont(ft);
	YZGridButton->setFont(ft);
	XZGridButton->setFont(ft);

	listLayout->addWidget(nocoGridButton);
	listLayout->addWidget(XYGridButton);
	listLayout->addWidget(YZGridButton);
	listLayout->addWidget(XZGridButton);

	connect(nocoGridButton, SIGNAL(clicked(bool)), glWidget, SLOT(nocoClicked(bool)));
	connect(XYGridButton, SIGNAL(clicked(bool)), glWidget, SLOT(XYClicked(bool)));
	connect(YZGridButton, SIGNAL(clicked(bool)), glWidget, SLOT(YZClicked(bool)));
	connect(XZGridButton, SIGNAL(clicked(bool)), glWidget, SLOT(XZClicked(bool)));

	XZGridButton->click();

	QWidget *m = new QWidget(this);
	m->setLayout(listLayout);
	m->setMaximumHeight(200);
	container->addWidget(m);
   
    mainLayout->addWidget(glWidget);
	QHBoxLayout *sliderLayout = new QHBoxLayout(this);
	QLabel *transLabel = new QLabel("Translation mode:",this);
	QLabel *scaleLabel = new QLabel("Scale:", this);
	QLabel *stepLabel = new QLabel("Step:", this);
	
	QComboBox * transModeCombobox = new QComboBox(this);
	transModeCombobox->setMaximumSize(QSize(180, 30));
	transModeCombobox->setMinimumSize(QSize(0, 30));
	ft.setPointSize(12);
	transModeCombobox->setFont(ft);
	transLabel->setFont(ft);
	transModeCombobox->addItem(QString("Global"), -1);
	transModeCombobox->addItem(QString("Local"), -1);
	connect(transModeCombobox, SIGNAL(currentIndexChanged(int)), glWidget, SLOT(setTransMode(int)));

	ySlider->setValue(50);

	scaleLabel->setFont(ft);
	stepLabel->setFont(ft);
	sliderLayout->addWidget(transLabel);
	sliderLayout->addWidget(transModeCombobox);
	sliderLayout->addWidget(stepLabel);
	sliderLayout->addWidget(ySlider);
	sliderLayout->addWidget(scaleLabel);
    sliderLayout->addWidget(xSlider);
	//sliderLayout->addWidget(lineEdit);
	mainLayout->addLayout(sliderLayout);

	QFileSystemModel *modelsys = new QFileSystemModel(this);
	modelsys->setRootPath("Meshes");

	QStandardItemModel* modelstd = new QStandardItemModel(this);
	modelstd->setHorizontalHeaderLabels(QStringList() << QStringLiteral("Meshes"));

	for (int i = 0; i < fileList.size(); i++)
	{
		QFileInfo fileInfo = fileList.at(i);
		if (fileInfo.fileName() != "." && fileInfo.fileName() != "..")
		{
			QStandardItem* itemProject = new QStandardItem(fileInfo.fileName());
			itemProject->setEditable(false);
			
			modelstd->appendRow(itemProject);
		}
	}

	tree = new QTreeView(this);
	tree->setModel(modelstd);
	tree->setRootIndex(modelsys->index("Meshes"));
	tree->setContextMenuPolicy(Qt::CustomContextMenu);

	connect(tree, SIGNAL(customContextMenuRequested(const QPoint)), this, SLOT(showMouseRightButton(const QPoint)));
	connect(tree, SIGNAL(doubleClicked(const QModelIndex)), this, SLOT(fileDoubleClicked(const QModelIndex)));
	tree->setMaximumHeight(200);
	container->addWidget(tree);
	
	QWidget *w = new QWidget(this);
	w->setLayout(container);
	w->setMaximumHeight(200);
	mainLayout->addWidget(w);

    setLayout(mainLayout);
	glWidget->setFocus();
	glWidget->setFocusPolicy(Qt::StrongFocus);
	connect(glWidget, SIGNAL(addComboItem(modelGuy*)), this, SLOT(addComboItem(modelGuy*)));
	connect(glWidget, SIGNAL(refreshComboItem()), this, SLOT(refreshComboItem()));
	connect(this,SIGNAL(addMeshDoubleClick()),glWidget,SLOT(addMeshDoubleClick()));
	connect(glWidget, SIGNAL(currentMeshChanged(int)), this, SLOT(meshChanged(int)));
    setWindowTitle(tr("Hello GL"));
}

void Window::fileDoubleClicked(const QModelIndex & index){
	QString fileName = tree->model()->data(index).toString();
	glWidget->fileNameRightClick = fileName;
	emit addMeshDoubleClick();
	//QMessageBox::information(0, tr("Cannot dock"), tr("Main window already closed"));
}

void Window::meshChanged(int index){
	comboBox->setCurrentIndex(index + 1);
}

void Window::showMouseRightButton(const QPoint){
	QMenu *qMenu = NULL;

	if (qMenu)
	{
		delete qMenu;
		qMenu = NULL;
	}
	QModelIndex index = tree->currentIndex();
	QString fileName = tree->model()->data(index).toString();
	glWidget->fileNameRightClick = fileName;
	qMenu = new QMenu(this);

	QAction* addTreeItemAction = new QAction("&AddItem", this);
	connect(addTreeItemAction, SIGNAL(triggered()), glWidget, SLOT(addMeshRightClick()));

	qMenu->addAction(addTreeItemAction);
	
	qMenu->exec(QCursor::pos()); 
	glWidget->setFocus();
}

QSlider *Window::createSlider()
{
    QSlider *slider = new QSlider(Qt::Horizontal,this);
    slider->setRange(1, 100);
    slider->setSingleStep(1);
    slider->setPageStep(1);
    //slider->setTickInterval(1);
    //slider->setTickPosition(QSlider::TicksRight);
    return slider;
}

void Window::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
        close();
    else
        QWidget::keyPressEvent(e);
}

void Window::addComboItem(/*GLMmodel*/modelGuy * modelM){
	comboBox->addItem(modelM->meshName, modelM->modelIndex);
	comboBox->setCurrentIndex(comboBox->count()-1);
	xSlider->setValue(modelM->scale * glWidget->dpi);
	glWidget->setFocus();
}

void Window::refreshComboItem(){
	comboBox->setCurrentIndex(0);
	int k = comboBox->count();
	for (int i = 0; i < k; i++)
	{
		comboBox->removeItem(0);
	}
	
	comboBox->addItem(QString("Camera"), -1);
	
	for (int j = 0; j <glWidget->modelManager.size(); ++j) {
		addComboItem(glWidget->modelManager.at(j));
	}
	comboBox->setCurrentIndex(0);
}

void Window::dockUndock()
{
    if (parent()) {
        setParent(0);
        setAttribute(Qt::WA_DeleteOnClose);
        move(QApplication::desktop()->width() / 2 - width() / 2,
             QApplication::desktop()->height() / 2 - height() / 2);
        dockBtn->setText(tr("Dock"));
        show();
    } else {
        if (!mainWindow->centralWidget()) {
            if (mainWindow->isVisible()) {
                setAttribute(Qt::WA_DeleteOnClose, false);
                dockBtn->setText(tr("Undock"));
                mainWindow->setCentralWidget(this);
            } else {
                QMessageBox::information(0, tr("Cannot dock"), tr("Main window already closed"));
            }
        } else {
            QMessageBox::information(0, tr("Cannot dock"), tr("Main window already occupied"));
        }
    }
}
