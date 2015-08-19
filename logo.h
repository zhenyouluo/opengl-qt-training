#ifndef LOGO_H
#define LOGO_H

#include <qopengl.h>
#include <QVector>
#include <QVector3D>
#include <QVector2D>
#include <QOpenGLShaderProgram>
#include <QOpenglTexture>
#include "glm.h"

class Logo
{
public:
    Logo();
    const GLfloat *constData() const { return m_data.constData(); }
    int count() const { return m_count; }
    int vertexCount() const { return m_count / 8; }
	bool hasTex = false;
	QOpenGLTexture* texture;
	GLMmodel *model_ptr;
	QVector <GLMmodel *> modelList;
	GLMmodel * addModel(QString name);
	QVector<QOpenGLTexture*> texList;
	bool removeModel(GLMmodel * model);
	bool updateBuffer();
	void initGL();
	//void draw(QOpenGLTexture* texture); QVector<QOpenGLTexture*>
	void draw(QOpenGLShaderProgram *m_program, int m_hasTex, int m_mvMatrixLoc, int m_normalMatrixLoc, QMatrix4x4 m_camera, QMatrix4x4 m_world, QVector<QOpenGLTexture*> &texList,
		int matAmbientLoc, int matDiffuseLoc, int matSpecularLoc, int matShineLoc);
	void drawWorldCamera(QOpenGLShaderProgram *m_program, int m_mvMatrixLoc, QMatrix4x4 m_camera);


private:
    void add(const QVector3D &v, const QVector3D &n);
	void addTex(const QVector2D &t);
	QVector<GLfloat> m_data;
    int m_count;
	
	GLvoid glmVN(GLMmodel* model);
	
};

#endif // LOGO_H
