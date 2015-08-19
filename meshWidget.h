#ifndef MESHWIDGET_H
#define MESHWIDGET_H
#include <QWidget>
#include <QCheckBox>
#include <QVBoxLayout>
class MeshWidget : public QWidget
{
	Q_OBJECT
public:
	explicit MeshWidget(QWidget *parent, QString filename);
signals:
	void addMesh(QString filename);
	void removeMesh(QString filename);

public slots :
	void checkChanged(int b);
	
private:	QCheckBox meshCheck;
	QString file;

};
#endif