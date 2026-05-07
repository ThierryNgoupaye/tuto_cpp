//
// Created by thierry on 02/05/2026.
//

#ifndef TUTO_CPP_OPERATOR_REDEFINITION_H
#define TUTO_CPP_OPERATOR_REDEFINITION_H

#include <ostream>


class vec2D {
public:
    int x, y;
    vec2D();
    vec2D(const int X, const int Y) : x(X), y(Y) {}

    friend std::ostream& operator<<(std::ostream& flux, const vec2D& vec)  {
        flux << "(" << vec.x << "," << vec.y << ")" << std::endl;
        return flux;
    }

    vec2D operator+(const vec2D& other) const {
        const vec2D v (this->x + other.x, this->y + other.y);
        return v;
    }

    vec2D operator-(const vec2D& other) const {
        const vec2D v (this->x - other.x, this->y - other.y);
        return v;
    }

    vec2D operator*(const int& number) const {
        const vec2D v (this->x * number, this->y * number);
        return v;
    }
};







#endif //TUTO_CPP_OPERATOR_REDEFINITION_H
