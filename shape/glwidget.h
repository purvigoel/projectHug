#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <memory>

#include "GL/glew.h"
#include <QGLWidget>
#include "glwidget.h"
#include "Settings.h"
#include <math.h>
#include <iostream>
#include "ResourceLoader.h"
#include "openglshape.h"
//#include "cap.h"
#include "gl/shaders/ShaderAttribLocations.h"

class OpenGLShape;

class GLWidget
{

public:
    GLWidget();
    ~GLWidget();
    virtual void initializeShape(float x, float y) = 0;
    virtual void draw();

    int getShapeType();
    int getShapeParameterOne();
    int getShapeParameterTwo();
protected:
    /** This is called every time the window needs to be repainted. */
    /** Vertex array objects for each of the three shapes in this lab. */
    std::unique_ptr<OpenGLShape> m_shape;

    int m_shapeParameterOne;
    int m_shapeParameterTwo;
    int m_shapeType;
};

#endif // GLWIDGET_H
