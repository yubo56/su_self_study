#include <cassert>      // assert
#include <iostream>     // ostream
class ColorRGB 
{
    private:
        float R;
        float G;
        float B;// three colors

    public:
        // three-value constructor: order is (R, G, B)
        ColorRGB (float x, float y, float z) : R(x), G(y), B(z){}
        // default constructor: sets to all zeroes
        ColorRGB () : R(0), G(0), B(0){}
        // copy constructor, copy assignment, destructor by default



        // accessors and mutators for colors
        inline float get_R() const { return R; }
        inline float get_G() const { return G; }
        inline float get_B() const { return B; }
        inline void set_R(float v) { R = v; }
        inline void set_G(float v) { G = v; }
        inline void set_B(float v) { B = v; }

        // compound assignment + simple arithmetic operators
        ColorRGB & operator+=(const ColorRGB &col);
        ColorRGB & operator-=(const ColorRGB &col);
        ColorRGB & operator*=(const ColorRGB &col);
        inline const ColorRGB operator+(const ColorRGB &col) const { return ColorRGB(*this) += col; }
        inline const ColorRGB operator-(const ColorRGB &col) const { return ColorRGB(*this) -= col; }
        inline const ColorRGB operator*(const ColorRGB &col) const { return ColorRGB(*this) *= col; }

        // scalar operations *= /= and simple arithmetic versions
        ColorRGB & operator*= (float f);
        ColorRGB & operator/= (float f);
        inline const ColorRGB operator*(float f) const { return ColorRGB(*this) *= f; }
        friend inline const ColorRGB operator*(float f, const ColorRGB &col) { return ColorRGB(col) *= f; }
        inline const ColorRGB operator/(float f) const { return ColorRGB(*this) /= f; }

        // stream-output
        friend std::ostream& operator<<(std::ostream& os, const ColorRGB &col);
};
