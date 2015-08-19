#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QOpenglTexture>
#include <QPair>
#include <QVector4D>
//#include "logo.h"
#include "glm.h"

#define NoCo 0x1001
#define XY   0x1002
#define YZ   0x1003
#define XZ   0x1004
#define Mc 10.0

QT_FORWARD_DECLARE_CLASS(QOpenGLShaderProgram)

typedef struct node{
	GLMmodel * model_ptr = NULL;
	QString meshName;
	QMatrix4x4 m_world;
	float scale = 1.0;
	int m_xRot = 0;
	int m_yRot = 0;
	int m_zRot = 0;

	float m_xTrans = 0;
	float m_yTrans = 0;
	float m_zTrans = 0;

	int modelIndex = 0;
} modelGuy;

class triangle{
public:
	triangle(const QVector4D & a0, const QVector4D & a1, const QVector4D & a2):v0(a0),e1(a1),e2(a2){
	
	};
	QVector3D getVectorByNum(int index);
	QVector4D v0;
	QVector4D e1;
	QVector4D e2;
};

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
    Q_OBJECT

public:
    GLWidget(QWidget *parent = 0);
    ~GLWidget();

    QSize minimumSizeHint() const Q_DECL_OVERRIDE;
    QSize sizeHint() const Q_DECL_OVERRIDE;
	QOpenGLTexture* texture;
	QVector<QOpenGLTexture*> texList;
	int coord = XZ;
	float dpi = 5.0;
	//GLMmodel* currentMesh;
	modelGuy* currentMesh;
	bool loadTexture(GLMmodel* model);
	QVector <GLMmodel *> modelList;
	GLMmodel * addModel(QString name);
	GLMmodel * removeModel(QString name);
	QVector<GLfloat> m_data;
	int m_count;
	void add(const QVector3D &v, const QVector3D &n);
	void addTex(const QVector2D &t);
	GLvoid glmVN(GLMmodel* model);
	GLvoid glmBox(GLMmodel* model);
	void draw(QOpenGLShaderProgram *m_program, int m_hasTex, int m_mvMatrixLoc, int m_normalMatrixLoc, QMatrix4x4 m_camera, QMatrix4x4 m_world, QVector<QOpenGLTexture*> &texList,
		int matAmbientLoc, int matDiffuseLoc, int matSpecularLoc, int matShineLoc);
	bool updateBuffer();
	QString fileNameRightClick;
	QVector<modelGuy *> modelManager;
	int width;
	int height;
	

public slots:
    void setXRotation(int angle);
    void setYRotation(int angle);
    void setZRotation(int angle);
	void setCamXRotation(int angle);
	void setCamYRotation(int angle);
	void setCamZRotation(int angle);
	void setTrans(float dx, float dy, float dz);
	void meshChanged(int index);
	void nocoClicked(bool b);
	void XYClicked(bool b);
	void YZClicked(bool b);
	void XZClicked(bool b);
    void cleanup();
	void addMesh(QString name);
	void addMeshRightClick();
	void removeMesh(QString name);
	void removeCurrentMesh();
	void sliderScale(int scale);
	void sliderStep(int scale);
	void setTransMode(int);
	void addMeshDoubleClick();

signals:
    void xRotationChanged(int angle);
    void yRotationChanged(int angle);
    void zRotationChanged(int angle);
	//void addComboItem(GLMmodel* model);
	void addComboItem(modelGuy* model);
	void refreshComboItem();
	void sliderScaleChanged(int scale);
	void sliderStepChanged(int scale);
	void currentMeshChanged(int index);

protected:
    void initializeGL() Q_DECL_OVERRIDE;
    void paintGL() Q_DECL_OVERRIDE;
    void resizeGL(int width, int height) Q_DECL_OVERRIDE;
    void mousePressEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
    void mouseMoveEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void mouseReleaseEvent(QMouseEvent *event) Q_DECL_OVERRIDE;
	void keyPressEvent(QKeyEvent * event) Q_DECL_OVERRIDE;
	void keyReleaseEvent(QKeyEvent * event) Q_DECL_OVERRIDE;
	void wheelEvent(QWheelEvent * event) Q_DECL_OVERRIDE;

private:
    void setupVertexAttribs();
	int iTransMode;
    bool m_core;
    int m_xRot;
    int m_yRot;
    int m_zRot;
    QPoint m_lastPos;
    QOpenGLVertexArrayObject m_vao;
    QOpenGLBuffer m_logoVbo;
    QOpenGLShaderProgram *m_program;
    int m_projMatrixLoc;
    int m_mvMatrixLoc;
    int m_normalMatrixLoc;
    int m_lightPosLoc;
	int m_hasTex;
	int m_fixedPipeline;
	int m_fixedColor;
	int matAmbientLoc;
	int matDiffuseLoc;
	int matSpecularLoc;
	int matShineLoc;
	int m_viewPos;
    QMatrix4x4 m_proj;
    QMatrix4x4 m_camera;
    QMatrix4x4 m_world;
	QVector3D eye;
	QVector3D at;
	QVector3D up;
	QVector3D selectRay;
    bool m_transparent;
	GLMmodel * model_ptr;
	float stepDPI;
	float stepLength;
	float nearPlane;
	float length;
	float lengthFormer;
	int interIndex;
	bool mouseMoved;
};

#endif
