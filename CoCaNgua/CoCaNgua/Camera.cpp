#include "Camera.h"


Camera::Camera(){
  at = Vector3(0.0,0.0,0.0);
	up = Vector3(0.0,1.0,0.0);
	theta = Math::PI/3;
	phi = Math::PI/3;
	R = 43.0;
  eye = transf_coord();
	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
    //gluLookAt(eye.x,eye.y,eye.z,at.x,at.y,at.z,up.x,up.y,up.z);
};


Camera& Camera::inst()
{
  static Camera Instance;
  return Instance;
}


Camera::~Camera(){};

void Camera::update(float x, float y){
  float new_phi = phi_old - (y-y_old)/50.0;	
  if (new_phi > (Math::PI/2.3))								
    new_phi = Math::PI/2.3 + 0.001;
  else if (new_phi < 0)
    new_phi = 0.001;

  float new_theta = theta_old - (x-x_old)/50.0; 

  theta = new_theta;
  phi = new_phi;
  //printf("phi: %f  theta: %f  R: %f\n",phi,theta,R);
};

void Camera::save(float x, float y){
  x_old = x;
  y_old = y;
  theta_old = theta;
  phi_old = phi;
}

Vector3 Camera::transf_coord(){
  Vector3 result;
  result.z = at.z + (GLdouble)R*sinf(phi)*cosf(theta);
  result.x = at.x + (GLdouble)R*sinf(phi)*sinf(theta);
  result.y = at.y + (GLdouble)R*cosf(phi);
  return result;
}

void Camera::zoom(float x){
  R += x;
  if (R < 3.0)
      R = 3.0;
  if (R > 47.0)
      R = 47.0;
  eye = transf_coord();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(eye.x,eye.y,eye.z,at.x,at.y,at.z,up.x,up.y,up.z);
  //printf("at: %f, %f, %f\n", at.x, at.y, at.z);
}

void Camera::pan(float x, float z){
//    at.x = (x_pan - x_old)/50.0;
//    at.z = (z_pan - y_old)/50.0;
//    glMatrixMode(GL_MODELVIEW);
//	glLoadIdentity();
//    gluLookAt(eye.x,eye.y,eye.z,at.x,at.y,at.z,up.x,up.y,up.z);
    //printf("PAN NAO IMPLEMENTADO");
}

void Camera::rotate(float x, float y){
  update(x,y);
  eye = transf_coord();
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(eye.x,eye.y,eye.z,at.x,at.y,at.z,up.x,up.y,up.z);
}
