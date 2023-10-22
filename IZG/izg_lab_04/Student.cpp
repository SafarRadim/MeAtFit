#include "Student.h"

#include <osg/Plane>
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>

osg::Vec3d projectOnPlane(const osg::Vec3d& point, const osg::Vec3d& planeNormal)
{
    return point - planeNormal * (point * planeNormal);
}

double dot(const osg::Vec3d& u, const osg::Vec3d& v)
{
    return u * v;
}

osg::Vec3d cross(const osg::Vec3d& u, const osg::Vec3d& v)
{
    return u ^ v;
}

void normalize(osg::Vec3d& v)
{
    v.normalize();
}

osg::Vec3d normalized(const osg::Vec3d& v)
{
    osg::Vec3d u(v);
    u.normalize();

    return u;
}

osg::Matrix inverse(const osg::Matrix& m)
{
    return osg::Matrix::inverse(m);
}

#if USE_STUDENT_SOLUTION
osg::Matrix getScaleMatrix(const osg::Vec3d& scale)
{
    //Task 1 - fill in the scale matrix
    double sx = scale[0];
    double sy = scale[1];
    double sz = scale[2];

    osg::Matrix m(
        sx,   0,   0,  0,
         0,  sy,   0,  0,
         0,   0,  sz,  0,
         0,   0,   0,  1
    );

    return m;
}

osg::Matrix getTranslationMatrix(const osg::Vec3d& translation)
{
    //Task 2 - fill in the translation matrix
    double vx = translation[0];
    double vy = translation[1];
    double vz = translation[2];

    osg::Matrix m(
         1,  0,  0,  0,
         0,  1,  0,  0,
         0,  0,  1,  0,
        vx, vy, vz,  1
    );
    return m;
}

osg::Matrix rotateAroundX(double angle)
{
    
    double sina = std::sin(angle);
    double cosa = std::cos(angle);

    //Task 3 - fill in the rotation matrix around X axis

    osg::Matrix m(
        1,    0,     0, 0,
        0, cosa, -sina, 0,
        0, sina,  cosa, 0,
        0,    0,     0, 1
    );

    return m;
}

osg::Matrix rotateAroundY(double angle)
{
    double sina = std::sin(angle);
    double cosa = std::cos(angle);

    //Task 4 - fill in the rotation matrix around Y axis

    osg::Matrix m(
         cosa, 0, sina, 0,
            0, 1,    0, 0,
        -sina, 0, cosa, 0,
            0, 0,    0, 1
    );

    return m;
}

osg::Matrix rotateAroundZ(double angle)
{
    double sina = std::sin(angle);
    double cosa = std::cos(angle);

    //Task 5 - fill in the rotation matrix around Z axis

    osg::Matrix m(
        cosa, -sina, 0, 0,
        sina,  cosa, 0, 0,
           0,     0, 1, 0,
           0,     0, 0, 1
    );

    return m;
}

double angleBetweenVectors(osg::Vec3d u, osg::Vec3d v)
{
    if (u.length2() == 0 || v.length2() == 0)
    {
        return 0.0;
    }

    //Task 6
    //Normalize both vectors - method normalize()
    //Compute cosine of the angle between the vectors using the dot product - function dot
    //Return the angle using arcus cosine - function std::acos()

    u.normalize();
    v.normalize();

    double dotProd = dot(u, v);
    return std::acos(dotProd);
}

osg::Matrix getRotationMatrix(const osg::Vec3d& fromVector, const osg::Vec3d& toVector)
{
    const osg::Vec3d xAxis(1, 0, 0);
    const osg::Vec3d yAxis(0, 1, 0);
    const osg::Vec3d zAxis(0, 0, 1);

    //Task 7 - compute the rotation matrix around arbitrary axis
    
    //Compute the angle between input vectors - function angleBetweenVectors
    //7a - compute the rotation axis using the cross product - function cross
    osg::Vec3d rotationVector = cross(fromVector, toVector);
       
    //7b - project rotation axis into XY plane
    osg::Vec3d xyProjection = projectOnPlane(rotationVector, yAxis);

    //7c - compute the angle between projected rotation axis and X axis
    double xRotationAngle = angleBetweenVectors(xyProjection, xAxis);

    //7d - compute rotation around Z axis
    osg::Matrix zRotation = rotateAroundZ(xRotationAngle);

    //Rotate axis of rotation into XZ plane
    
    //7e - compute angle between rotated rotation axis and X axis and rotate corrently, hint - for Pi use M_PI
    
    //7f - compute the desired rotation around X axis

    //7g - compute and return the final rotation - for matrix inverse use function inverse
    osg::Matrix result;

    return result;
}
#else

osg::Matrix getScaleMatrix(const osg::Vec3d& scale)
{
    return osg::Matrix::scale(scale);
}

osg::Matrix getTranslationMatrix(const osg::Vec3d& translation)
{
    return osg::Matrix::translate(translation);
}

osg::Matrix getRotationMatrix(const osg::Vec3d& fromVector, const osg::Vec3d& toVector)
{
    osg::Quat q;
    q.makeRotate(fromVector, toVector);

    return osg::Matrix(q);
}

osg::Matrix rotateAroundX(double angle)
{
    return {};
}

osg::Matrix rotateAroundY(double angle)
{
    return {};
}

osg::Matrix rotateAroundZ(double angle)
{
    return {};
}

double angleBetweenVectors(osg::Vec3d u, osg::Vec3d v)
{
    return 0.0;
}
#endif