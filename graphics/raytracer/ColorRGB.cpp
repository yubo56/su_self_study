#include "ColorRGB.hpp"

ColorRGB & ColorRGB::operator+=(const ColorRGB &col)
{
    this->R += col.R;
    this->G += col.G;
    this->B += col.B;
    return *this;
}
ColorRGB & ColorRGB::operator-=(const ColorRGB &col)
{
    this->R -= col.R;
    this->G -= col.G;
    this->B -= col.B;
    return *this;
}
ColorRGB & ColorRGB::operator*=(const ColorRGB &col)
{
    this->R *= col.R;
    this->G *= col.G;
    this->B *= col.B;
    return *this;
}
ColorRGB & ColorRGB::operator*= (float f)
{
    this->R *= f;
    this->G *= f;
    this->B *= f;
    return *this;
}
ColorRGB & ColorRGB::operator/= (float f)
{
    assert(f != 0);
    this->R /= f;
    this->G /= f;
    this->B /= f;
    return *this;
}
std::ostream& operator<<(std::ostream& os, const ColorRGB &col)
{
    os << "( R:" << col.R << " , G:" << col.G << " , B:" << col.B << " )";
    return os;
}
