#include "logo.h"
#include <qmath.h>

#include <QVector3D>
#include <QVector4D>
#include <QDir>
#define T(x) (model->triangles[(x)])

Logo::Logo()
    : m_count(0)
{

	//addModel(QString("teapot"));
	addModel(QString("arakkoa"));
	//addModel(QString("soccerplayer"));
	//removeModel(modelList.at(2));
	initGL();
}

GLMmodel*  Logo::addModel(QString name){
	QString pathname = QString("Meshes/") + name +QString("/")+ name + QString(".obj");
	QByteArray ba = pathname.toLatin1();
	model_ptr = glmReadOBJ(ba.data());
	if (!model_ptr)
		exit(0);

	glmUnitize(model_ptr);
	glmFacetNormals(model_ptr);
	glmVertexNormals(model_ptr, 90.0);
	model_ptr->m_world.setToIdentity();
	modelList.append(model_ptr);

	int size = 0;
	for (int i = 0; i < modelList.size(); ++i) {
		size += modelList.at(i)->numtriangles * 8 * 3;
	}

	m_data.resize(size);
	//for (int i = 0; i < modelList.size(); ++i) {
	//	glmVN(modelList.at(i));
	//}
	glmVN(model_ptr);

	return model_ptr;
}

bool Logo::removeModel(GLMmodel * model){
	if (modelList.removeAll(model)){
		updateBuffer();
		return true;
	}
	else
		return false;
	
}
bool Logo::updateBuffer(){
	m_data.erase(m_data.begin(),m_data.end());
	m_count = 0;
	int size = 0;
	for (int i = 0; i < modelList.size(); ++i) {
		size += modelList.at(i)->numtriangles * 8 * 3;
	}

	m_data.resize(size);
	for (int i = 0; i < modelList.size(); ++i) {
		glmVN(modelList.at(i));
	}
	return true;
}


void Logo::add(const QVector3D &v, const QVector3D &n)
{
    GLfloat *p = m_data.data() + m_count;
    *p++ = v.x();
    *p++ = v.y();
    *p++ = v.z();
    *p++ = n.x();
    *p++ = n.y();
    *p++ = n.z();
    m_count += 6;
}

void Logo::addTex(const QVector2D &t)
{
	GLfloat *p = m_data.data() + m_count;
	*p++ = t.x();
	*p++ = t.y();
	m_count += 2;
}

GLvoid Logo::glmVN(GLMmodel* model)
{
	GLMgroup* group;
	group = model->groups;
	GLMtriangle* triangle;
	GLuint i;
	while (group) {
		group->arrayBegin = m_count / 8;
		for (i = 0; i < group->numtriangles; i++) {
			triangle = &T(group->triangles[i]);
			for (int j = 0; j < 3; j++)
			{
				QVector3D v(model->vertices[3 * triangle->vindices[j]], model->vertices[3 * triangle->vindices[j] + 1], model->vertices[3 * triangle->vindices[j] + 2]);
				QVector3D n(model->normals[3 * triangle->nindices[j]], model->normals[3 * triangle->nindices[j] + 1], model->normals[3 * triangle->nindices[j] + 2]);
				add(v, n);
				if (model->materials[group->material].hasTex){
					QVector2D t(model->texcoords[2 * triangle->tindices[j]], model->texcoords[2 * triangle->tindices[j] + 1]);
					addTex(t);
				}
				else
				{
					QVector2D t(0,0);
					addTex(t);
				}
			}
		}
		group = group->next;
	}
}

void Logo::initGL(){
//	texture = new QOpenGLTexture(QImage(QString("Meshes/cube/default.png")).mirrored());
}

void Logo::draw(QOpenGLShaderProgram *m_program, int m_hasTex, int m_mvMatrixLoc, int m_normalMatrixLoc, QMatrix4x4 m_camera, QMatrix4x4 m_world, QVector<QOpenGLTexture*> &texList, 
	int matAmbientLoc, int matDiffuseLoc, int matSpecularLoc, int matShineLoc){
	//texture->bind();
	//	glDrawArrays(GL_TRIANGLES, 0, vertexCount());
	m_program->setUniformValue("s_baseMap", 0);
	for (int j = 0; j < modelList.size(); ++j) {
		GLMmodel* model = modelList.at(j);
		
		model->m_world.setToIdentity();
		model->m_world.translate(model->m_xTrans, model->m_yTrans, model->m_zTrans);
		model->m_world.rotate(model->m_xRot / 16.0f, 1, 0, 0);
		model->m_world.rotate(model->m_yRot / 16.0f, 0, 1, 0);
		model->m_world.rotate(model->m_zRot / 16.0f, 0, 0, 1);
		
		GLMgroup* group;
		group = model->groups;
		GLMtriangle* triangle;
		GLuint i;
		while (group) {
			if (model->materials[group->material].hasTex)
			{
				//texture = new QOpenGLTexture(QImage("Meshes/arakkoa/Arakkoa_Gray.png").mirrored());
				texList[model->materials[group->material].texIndex]->bind();
				//QOpenGLTexture * haha = this->texList[model->materials[group->material].texIndex];
				//texture->bind();
			}
			m_program->setUniformValue(m_mvMatrixLoc, m_camera * m_world * model->m_world);
			m_program->setUniformValue(m_normalMatrixLoc, (/*m_world * */model->m_world).normalMatrix());
			m_program->setUniformValue(m_hasTex, model->materials[group->material].hasTex);
			m_program->setUniformValue(matAmbientLoc, QVector3D(model->materials[group->material].ambient[0], model->materials[group->material].ambient[1], model->materials[group->material].ambient[2]/*, model->materials[group->material].ambient[3]*/));
			m_program->setUniformValue(matDiffuseLoc, QVector3D(model->materials[group->material].diffuse[0], model->materials[group->material].diffuse[1], model->materials[group->material].diffuse[2]/*, model->materials[group->material].diffuse[3]*/));
			m_program->setUniformValue(matSpecularLoc, QVector3D(model->materials[group->material].specular[0], model->materials[group->material].specular[1], model->materials[group->material].specular[2]/*, model->materials[group->material].specular[3]*/));
			m_program->setUniformValue(matShineLoc, model->materials[group->material].shininess);

			glDrawArrays(GL_TRIANGLES, group->arrayBegin, group->numtriangles * 3);
			group = group->next;
		}
	}
}

void Logo::drawWorldCamera(QOpenGLShaderProgram *m_program, int m_mvMatrixLoc, QMatrix4x4 m_camera){
	for (int j = 0; j < modelList.size(); ++j) {
		//m_program->setUniformValue(m_mvMatrixLoc, m_camera * m_world);
	}
}