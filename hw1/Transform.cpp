// Transform.cpp: implementation of the Transform class.


#include "Transform.h"

//Please implement the following functions:

// Helper rotation function.  
mat3 Transform::rotate(const float degrees, const vec3& axis) {
    float cos_theta = cos(glm::radians(degrees));
    float sin_theta = sin(glm::radians(degrees));
    /*
    float x = axis.x, y = axis.y, z = axis.z;
    
    return glm::mat3(cos_theta + x*x*(1 - cos_theta), y*x*(1 - cos_theta) + z*sin_theta, z*x*(1 - cos_theta) - y*sin_theta,
                     x*y*(1 - cos_theta) - z*sin_theta, cos_theta + y*y*(1 - cos_theta), z*y*(1 - cos_theta) + x*sin_theta,
                     x*z*(1 - cos_theta) + y*sin_theta, y*z*(1 - cos_theta) - x*sin_theta, cos_theta + z*z*(1 - cos_theta));
    */
    
    mat3 I = glm::mat3(1.0,0.0,0.0,
                       0.0,1.0,0.0,
                       0.0,0.0,1.0);
    mat3 t_product = glm::mat3(axis.x*axis.x, axis.x*axis.y, axis.x*axis.z,
                               axis.x*axis.y, axis.y*axis.y, axis.y*axis.z,
                              
                               axis.x*axis.z, axis.y*axis.z, axis.z*axis.z);
    mat3 c_product = glm::mat3(0.0, axis.z, -axis.y,
                               -axis.z, 0.0, axis.x,
                               axis.y, -axis.x, 0.0);
    
    return cos_theta*I + (1.0 - cos_theta)*t_product + sin_theta*c_product;
     
}
// Transforms the camera left around the "crystal ball" interface
void Transform::left(float degrees, vec3& eye, vec3& up) {
    eye = Transform::rotate(degrees, up) * eye;
}

// Transforms the camera up around the "crystal ball" interface
void Transform::up(float degrees, vec3& eye, vec3& up) {
    vec3 orth = glm::normalize(glm::cross(glm::normalize(up), glm::normalize(eye)));
    mat3 rotMat = Transform::rotate(degrees, orth);
    eye = eye * rotMat;
    up = up * rotMat;
}

// Your implementation of the glm::lookAt matrix
mat4 Transform::lookAt(vec3 eye, vec3 up) {
    
}

Transform::Transform()
{

}

Transform::~Transform()
{

}
