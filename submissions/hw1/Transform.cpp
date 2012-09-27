// Transform.cpp: implementation of the Transform class.


#include "Transform.h"

//Please implement the following functions:

// Helper rotation function.  
mat3 Transform::rotate(const float degrees, const vec3& axis) {
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

// Transforms the camera left around the "crystal ball" interface
void Transform::left(float degrees, vec3& eye, vec3& up) {
    eye = eye * Transform::rotate(degrees, up);
}

// Transforms the camera up around the "crystal ball" interface
void Transform::up(float degrees, vec3& eye, vec3& up) {
    vec3 orth = glm::normalize(glm::cross(up, eye));
    mat3 rotMat = Transform::rotate(-degrees, orth);
    
    eye = eye * rotMat;
    up = glm::normalize(up * rotMat);
}

// Your implementation of the glm::lookAt matrix
mat4 Transform::lookAt(vec3 eye, vec3 up) {
    vec3 w = glm::normalize(eye);
    vec3 u = glm::normalize(glm::cross(up, w));
    vec3 v = glm::normalize(glm::cross(w, u));
    
    return glm::mat4(u.x, u.y, u.z, -u.x*eye.x - u.y*eye.y - u.z*eye.z,
                               v.x, v.y, v.z, -v.x*eye.x - v.y*eye.y - v.z*eye.z,
                               w.x, w.y, w.z, -w.x*eye.x - w.y*eye.y - w.z*eye.z,
                               0.0, 0.0, 0.0, 1.0);
}

Transform::Transform()
{

}

Transform::~Transform()
{

}
