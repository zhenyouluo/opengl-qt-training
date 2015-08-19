#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QComboBox>
#include <QTreeView>
#include <glm.h>
#include <itemaction.h>
#include <glwidget.h>

QT_BEGIN_NAMESPACE
class QSlider;
class QPushButton;
QT_END_NAMESPACE

class GLWidget;
class MainWindow;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window(MainWindow *mw);
	QComboBox *comboBox;
	QTreeView *tree;

protected:
    void keyPressEvent(QKeyEvent *event) Q_DECL_OVERRIDE;

signals:
	void addMeshDoubleClick();

private slots:
    void dockUndock();
	void addComboItem(modelGuy * modelM);
	void refreshComboItem();
	void showMouseRightButton(const QPoint);
	void fileDoubleClicked(const QModelIndex & index);
	void meshChanged(int index);
private:
    QSlider *createSlider();

    GLWidget *glWidget;
    QSlider *xSlider;
    QSlider *ySlider;
    QSlider *zSlider;
    QPushButton *dockBtn;
    MainWindow *mainWindow;
};

#endif
