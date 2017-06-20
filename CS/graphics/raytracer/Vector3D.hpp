#include <cassert>          // assert
#include <iostream>         // << operator overload
#include <cmath>            // sqrt
// 3D vector class 

class Vector3D
{
    private:
        float x;
        float y;
        float z;
   
    public:
        // default constructor; all zeroes 
        Vector3D () : x(0), y(0), z(0) {}

        // three-argument constructor: copies three arguments into private data 
        Vector3D (float a, float b, float c) : x(a), y(b), z(c) {}

        // Copy constructor + copy assignment are included 



        // adds components of const Vector3D argument to Vector3D this 
        Vector3D & operator+= (const Vector3D &other);
        // subtracts components of const Vector3D argument from Vector3D this 
        Vector3D & operator-= (const Vector3D &other);
        // adds components of const Vector3D argument and const Vector3D this into const return Vector3D 
        inline const Vector3D operator+ (const Vector3D &other) const { return Vector3D(*this) += other; }
        // subtracts components of const Vector3D argument from const Vector3D this into const return Vector3D 
        inline const Vector3D operator- (const Vector3D &other) const { return Vector3D(*this) -= other; }

        // Scalar multiplication by float factor of Vector3D this 
        Vector3D & operator*= (float factor);
        /* Scalar division by float factor of Vector3D this
         *
         * Cannot divide by zero; assertion */
        Vector3D & operator/= (float factor);
        // multiplies by float factor components of Vector3D this and returns reference 
        inline const Vector3D operator* (float factor) const { return Vector3D(*this) *= factor; }
        // left-handed scalar multiplication
        inline friend const Vector3D operator* (float factor, const Vector3D &v) { return Vector3D(v) *= factor; } 
        // division by float factor components of Vector3D this and returns reference 
        inline const Vector3D operator/ (float factor) const { return Vector3D(*this) /= factor; }

        // negation of const this vector into const return Vector3D 
        inline const Vector3D & operator- () const { return Vector3D(*this) *= -1; }

        // accessor for individual elements 
        float operator[](int index) const;
        float & operator[](int index); // include non-const version for assignment 

        // stream-output
        friend std::ostream& operator<< (std::ostream& os, const Vector3D& v);



        // compute the dot product of two Vector3Ds; overloaded as (Vector3D * Vector3D)
        inline friend float operator* (const Vector3D &v1, const Vector3D &v2);
        // compute cross product of two Vector3Ds; no operator overload
        friend const Vector3D cross(const Vector3D &v1, const Vector3D &v2);
        // compute magnitude-squared of Vector3D this
        inline float normsq() const { return (*this) * (*this); }
        // compute magnitude of Vector3D this
        inline float norm() const { return sqrtf(normsq()); }
        // return normalized version of current Vector3D
        inline Vector3D normalize() const { return Vector3D(*this) / norm(); }
};
