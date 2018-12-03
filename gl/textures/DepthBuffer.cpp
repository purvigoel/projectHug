#include "DepthBuffer.h"

#include "GL/glew.h"
#include <iostream>
using namespace CS123::GL;

DepthBuffer::DepthBuffer(int width, int height) :
    m_width(width),
    m_height(height)
{
    std::cout << "hi im a depth buffer" << id() << std::endl;
    // TODO [Task 8] bind() the render buffer and call glRenderbufferStorageEXT
    bind();
    glRenderbufferStorageEXT(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
    unbind();
    // TODO Don't forget to unbind()!
}
