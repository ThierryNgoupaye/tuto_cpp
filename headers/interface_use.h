//
// Created by thierry on 02/05/2026.
//

#ifndef TUTO_CPP_INTERFACE_USE_H
#define TUTO_CPP_INTERFACE_USE_H

#include <format>
#include <iostream>

class Drawable {
public:
    virtual ~Drawable() = default;

    virtual void draw() const = 0;
};


class Shape : public Drawable{
public:
    Shape(const float& x, const float& y) : m_x(x), m_y(y) {}
    void draw() const override {
        std::cout<< "Ceci est une forme indefinie" <<std::endl;
    }
    virtual void getShapeType() const = 0;


protected:
    float m_x, m_y;
};



class Cercle : public Shape {
public:
    Cercle(const float& x, const float& y, const float& rayon) : Shape(x, y), m_rayon(rayon) {}

    void getShapeType() const override {
        std::cout << "Je suis un cercle" << std::endl;
    }

    void draw() const override {
        std::cout << std::format("Position x={}, Position  y={}, Rayon du cercle ={};", m_x, m_y, m_rayon) << std::endl;
    }

protected:
    float m_rayon;
};


class Carre : public Shape {
public:
    Carre(const float& x, const float& y, const float& cote) : Shape(x, y), m_cote(cote) {}

    void getShapeType() const override {
        std::cout << "Je suis un carré" << std::endl;
    }

    void draw() const override {
        std::cout  << std::format("Position x={}, Position  y={}, Coté du carré ={};", m_x, m_y, m_cote) << std::endl;
    }

protected:
    float m_cote;
};



#endif //TUTO_CPP_INTERFACE_USE_H
