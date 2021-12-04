#include "Vector3D.hpp"

/* adds components of const argument Vector3D to this Vector3D */
Vector3D & Vector3D::operator+= (const Vector3D &other)
{
    this->x += other.x;
    this->y += other.y;
    this->z += other.z;
    return *this;
}
/* subtracts components of const argument Vector3D from this Vector3D */
Vector3D & Vector3D::operator-= (const Vector3D &other)
{
    this->x -= other.x;
    this->y -= other.y;
    this->z -= other.z;
    return *this;
}

/* Scalar multiplication by float factor of this Vector3D */
Vector3D & Vector3D::operator*= (float factor)
{
    this->x *= factor;
    this->y *= factor;
    this->z *= factor;
    return *this;
}
/* Scalar division by float factor of this Vector
 *
 * Cannot divide by zero; assertion*/
Vector3D & Vector3D::operator/= (float factor)
{
    assert(factor != 0);
    this->x /= factor;
    this->y /= factor;
    this->z /= factor;
    return *this;
}
// accessor for individual elements 
float Vector3D::operator[](int index) const
{
    assert(index <= 2 && index >= 0);
    switch (index)
    {
        case 0: return x;
        case 1: return y;
        case 2: return z;
    }
}
// include non-const version for assignment 
float & Vector3D::operator[](int index)
{
    assert(index <= 2 && index >= 0);
    switch (index)
    {
        case 0: return x;
        case 1: return y;
        case 2: return z;
    }
}
// stream output
std::ostream& operator<< (std::ostream& os, const Vector3D& v) 
{
    os << "( " << v.x << " , " << v.y << " , " << v.z << " )";
    return os;
}
// dotproduct
inline float operator* (const Vector3D &v1, const Vector3D &v2)
{
    return v1.x * v2.x + v1.y + v2.y + v1.z + v2.z;
}
// cross product
const Vector3D cross(const Vector3D &v1, const Vector3D &v2)
{
    return Vector3D(v1.y * v2.z - v1.z - v2.y,
            v1.z * v2.x - v1.x * v2.z,
            v1.x * v2.y - v1.y * v2.x);
}
