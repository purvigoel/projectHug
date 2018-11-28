/**
 * @file   CamtransCamera.cpp
 *
 * This is the perspective camera class you will need to fill in for the Camtrans lab.  See the
 * lab handout for more details.
 */

#include "CamtransCamera.h"
#include <Settings.h>
#include <iostream>
CamtransCamera::CamtransCamera()
{
    // @TODO Task 3: [CAMTRANS] Set up the default camera settings to match the demo...
    m_near = 1.0f;
    m_far = 30.0f;
    m_thetaH = 60.0f;
    m_thetaW = 60.0f;
    m_aspectRatio = 1.0f;
    m_eye = glm::vec4(2.0f,2.0f,2.0f,1.0f);
    m_look = glm::vec4(-1.0f,-1.0f,-1.0f,0.0f);
    m_up = glm::vec4(0.0f,1.0f,0.0f, 0.0f);
    m_position = glm::vec4(2.0f,2.0f,2.0f,1.0f);
std::cout << "HELLO FROM CAMERA" << std::endl;
    orientLook(m_eye, m_look, m_up);

    updateProjectionMatrix();
    updateViewMatrix();
}

glm::mat4x4 CamtransCamera::getProjectionMatrix() const {
    // TODO Task 3: Camtrans
    return (m_perspectiveTransformation * m_scaleMatrix);
}

glm::mat4x4 CamtransCamera::getViewMatrix() const {
    // TODO Task 3: Camtrans
    return (m_rotationMatrix * m_translationMatrix);
}

glm::mat4x4 CamtransCamera::getScaleMatrix() const {
    // TODO Task 1: Camtrans
    return m_scaleMatrix;
}

glm::mat4x4 CamtransCamera::getPerspectiveMatrix() const {
    // TODO Task 1: Camtrans
    return m_perspectiveTransformation;
}

glm::vec4 CamtransCamera::getPosition() const {
    // TODO Task 1: Camtrans
    return m_eye;
}

glm::vec4 CamtransCamera::getU() const {
    // TODO Task 1: Camtrans
    return m_u;
}

glm::vec4 CamtransCamera::getV() const {
    // TODO Task 1: Camtrans
    return m_v;
}

glm::vec4 CamtransCamera::getW() const {
    // TODO Task 1: Camtrans
    return m_w;
}

glm::vec4 CamtransCamera::getLook() const {
    // TODO Task 1: Camtrans
    return -m_w;
}

float CamtransCamera::getAspectRatio() const {
    // TODO Task 1: Camtrans
    return m_aspectRatio;
}

float CamtransCamera::getHeightAngle() const {
    // TODO Task 1: Camtrans
    return m_thetaH;
}

void CamtransCamera::orientLook(const glm::vec4 &eye, const glm::vec4 &look, const glm::vec4 &up) {
  // TODO Task 3: Camtrans
    m_look = glm::normalize(look);

    m_up = glm::normalize(up);
    m_up.w = 0.0f;

    m_eye = eye;

    glm::vec4 w =  -glm::normalize(look);

    glm::vec4 v = glm::normalize(up - (glm::dot(up, w) * w));
    glm::vec4 vPrime = glm::normalize(up - (glm::dot(up, w) * w));
    glm::vec3 cr = glm::cross(glm::vec3(vPrime.x, vPrime.y, vPrime.z), glm::vec3(w.x, w.y, w.z));
    glm::vec4 u = (glm::vec4(cr.x,cr.y,cr.z , 1.0f));
    m_w = w;
    m_v = v;
    m_u = u;
    m_u.w = 0.0f;
    m_w.w = 0.0f;
    m_v.w = 0.0f;

  //  m_up = m_v;


//    std::cout << "W: "<< m_w.x << " " << m_w.y << " " << m_w.z << " " << glm::dot(m_v, m_w) << std::endl;

//    std::cout << "U: "<< m_u.x << " " << m_u.y<< " " << m_u.z << std::endl;

 //   std::cout << "look: "<< m_look.x << " " << m_look.y << " " << m_look.z << std::endl;
    updateViewMatrix();
    updateProjectionMatrix();
}

void CamtransCamera::setHeightAngle(float h) {
    // TODO Task 3: Camtrans
    m_thetaH = h;
    float thetaR = glm::radians(m_thetaH);
    m_thetaW = m_aspectRatio * tan(thetaR/2.0f);
    updateProjectionMatrix();
}

void CamtransCamera::setAspectRatio(float a) {
    // TODO Task 3: Camtrans
    m_aspectRatio = a;
   updateProjectionMatrix();
}

void CamtransCamera::translate(const glm::vec4 &v) {
    // TODO: Camtrans
    glm::vec4 move = v;
    move.w = 0.0f;
    m_eye += move;
    updateViewMatrix();
    updateProjectionMatrix();

    // orientLook(m_eye, m_look,m_up);
}

void CamtransCamera::rotateU(float degrees) { //1
    // TODO Task 3: Camtrans
    float rad = glm::radians(degrees);    
    glm::vec4 v = m_v;
    glm::vec4 w = m_w;
    m_v = v * glm::cos(rad) + w * glm::sin(rad);
    m_w = -v * glm::sin(rad) + w * glm::cos(rad);

    updateViewMatrix();
    updateProjectionMatrix();
}

void CamtransCamera::rotateV(float degrees) {//2
    // TODO Task 3: Camtrans
    float rad = glm::radians(degrees);

    glm::vec4 u = m_u;
    glm::vec4 w = m_w;
    m_u = u * glm::cos(rad) - w * glm::sin(rad);
    m_w = u * glm::sin(rad) + w * glm::cos(rad);

    updateViewMatrix();
    updateProjectionMatrix();
}

void CamtransCamera::rotateW(float degrees) {//3
    // TODO Task 3: Camtrans
    float rad = glm::radians(degrees);   
    glm::vec4 u = m_u;
    glm::vec4 v = m_v;
    m_u = v * glm::sin(rad) + u * glm::cos(rad);
    m_v = v * glm::cos(rad) - u * glm::sin(rad);

    updateViewMatrix();
    updateProjectionMatrix();

}

void CamtransCamera::setClip(float nearPlane, float farPlane) {
    // TODO Task 3: Camtrans
    m_near = nearPlane;
    m_far = farPlane;
    updateProjectionMatrix();
}

// @TODO Task 1: Define the helper methods for updating the matrices here...
void CamtransCamera::updateProjectionMatrix(){
//clipping * unhinging
    updateScaleMatrix();
    updatePerspectiveMatrix();
}

void CamtransCamera::updatePerspectiveMatrix(){
   // std::cout << "update perspective matrix" << std::endl;
    float c = -m_near/m_far;
    m_perspectiveTransformation = glm::transpose(glm::mat4x4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,0.0f, -1.0f/(1.0f+c), c/(1.0f+c), 0.0f, 0.0f, -1.0f, 0.0f));
}

void CamtransCamera::updateScaleMatrix(){
   // std::cout << "update scale matrix" << std::endl;
    float rad = glm::radians(m_thetaH);
    m_thetaW = m_aspectRatio * glm::tan(rad/2.0f);
    float xScale = 1.0f/(m_far * m_aspectRatio * glm::tan(rad/2.0f));
    float yScale = 1.0f/ (m_far * glm::tan(rad / 2.0f));
    float zScale = 1.0f/m_far;
    m_scaleMatrix = glm::transpose(glm::mat4x4(xScale, 0.0f, 0.0f, 0.0f,0.0f, yScale, 0.0, 0.0f, 0.0f,0.0f,zScale, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f));
}

void CamtransCamera::updateViewMatrix(){
    updateRotationMatrix();
    updateTranslationMatrix();
}

void CamtransCamera::updateRotationMatrix(){
  //  std::cout << "update rotation matrix" << std::endl;
    m_rotationMatrix =  glm::transpose(glm::mat4x4(m_u.x, m_u.y, m_u.z, 0.0f, m_v.x, m_v.y, m_v.z,0.0f, m_w.x, m_w.y, m_w.z, 0.0f, 0.0f,0.0f,0.0f,1.0f));
}

void CamtransCamera::updateTranslationMatrix(){
    //std::cout << "update translation matrix" << std::endl;
    m_translationMatrix = glm::transpose(glm::mat4x4(1.0f, 0.0f, 0.0f, -m_eye.x, 0.0f,1.0f,0.0f,-m_eye.y, 0.0f,0.0f,1.0f,-m_eye.z, 0.0f,0.0f,0.0f,1.0f));
}

// @TODO Task 2: Fill in the helper methods you created for updating the matrices...
