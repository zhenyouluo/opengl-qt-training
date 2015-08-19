#include <meshWidget.h>
#include <QMessageBox>
MeshWidget::MeshWidget(QWidget *parent, QString filename):file(filename){
	//meshCheck = new QCheckBox(filename);
	meshCheck.setText(filename);
	QVBoxLayout *itemLayout = new QVBoxLayout;
	itemLayout->addWidget(&meshCheck);
	this->setLayout(itemLayout);
	connect(&meshCheck, SIGNAL(stateChanged(int)), this, SLOT(checkChanged(int)));
}

void MeshWidget::checkChanged(int b){
	if (b){
		//QMessageBox::about(this, "Adding a mesh", "This is a signal/slot sample with one parameter.");
		emit addMesh(file);
	}
	else{
		emit removeMesh(file);
	}
}
