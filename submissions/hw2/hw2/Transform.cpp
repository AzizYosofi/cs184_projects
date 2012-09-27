// Transform.cpp: implementation of the Transform class.


#include "Transform.h"


// Helper rotation function.  Please implement this.
mat3 Transform::rotate(const float degrees, const vec3& axis) {
    glm::normalize(axis);
    float cos_theta = cos(glm::radians(degrees));
    float sin_theta = sin(glm::radians(degrees));
    float x = axis.x, y = axis.y, z = axis.z;
    
    mat3 I = glm::mat3(1.0);
    
    mat3 t_product = glm::mat3(x*x, x*y, x*z,
                               x*y, y*y, y*z,
                               x*z, y*z, z*z);
    
    mat3 c_product = glm::mat3(0.0, -z, y,
                               z, 0.0, -x,
                               -y, x, 0.0);
    
    return I*cos_theta + (1.0 - cos_theta)*t_product + sin_theta*c_product;
}

void Transform::left(float degrees, vec3& eye, vec3& up) {
    eye = eye * Transform::rotate(degrees, up);
}

void Transform::up(float degrees, vec3& eye, vec3& up) {
    vec3 orth = glm::normalize(glm::cross(up, eye));
    mat3 rotMat = Transform::rotate(-degrees, orth);
    
    eye = eye * rotMat;
    up = glm::normalize(up * rotMat);
}

mat4 Transform::lookAt(const vec3 &eye, const vec3 &center, const vec3 &up) {
    vec3 a = eye - center;
    
    vec3 w = glm::normalize(a);
    vec3 u = glm::normalize(glm::cross(up, w));
    vec3 v = glm::normalize(glm::cross(w, u));
    
    return glm::mat4(u.x, u.y, u.z, -u.x*eye.x - u.y*eye.y - u.z*eye.z,
                     v.x, v.y, v.z, -v.x*eye.x - v.y*eye.y - v.z*eye.z,
                     w.x, w.y, w.z, -w.x*eye.x - w.y*eye.y - w.z*eye.z,
                     0.0, 0.0, 0.0, 1.0);
}

mat4 Transform::perspective(float fovy, float aspect, float zNear, float zFar){
    float d = cos(glm::radians(fovy)/2)/sin(glm::radians(fovy)/2);
    float A = -(zFar + zNear)/(zFar - zNear);
    float B = -(2*zFar*zNear)/(zFar - zNear);
    
    return glm::mat4(d/aspect, 0, 0, 0,
                     0, d, 0, 0,
                     0, 0, A, B,
                     0, 0, -1, 0);
}

mat4 Transform::scale(const float &sx, const float &sy, const float &sz) {
    return glm::mat4(sx, 0, 0, 0,
                     0, sy, 0, 0,
                     0, 0, sz, 0,
                     0, 0, 0, 1);
}

mat4 Transform::translate(const float &tx, const float &ty, const float &tz) {
    return glm::mat4(1,0,0,tx,
                     0,1,0,ty,
                     0,0,1,tz,
                     0,0,0,1);
}

// To normalize the up direction and construct a coordinate frame.
// As discussed in the lecture.  May be relevant to create a properly
// orthogonal and normalized up.
// This function is provided as a helper, in case you want to use it.
// Using this function (in readfile.cpp or display.cpp) is optional.

vec3 Transform::upvector(const vec3 &up, const vec3 & zvec) {
    vec3 x = glm::cross(up,zvec) ;
    vec3 y = glm::cross(zvec,x) ;
    vec3 ret = glm::normalize(y) ;
    return ret ;
}

Transform::Transform()
{
    
}

Transform::~Transform()
{
    
}
