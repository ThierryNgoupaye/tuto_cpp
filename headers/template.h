//
// Created by thierry on 30/04/2026.
//

#ifndef TUTO_CPP_TEMPLATE_H
#define TUTO_CPP_TEMPLATE_H
#include <iostream>
#include <ostream>


template <typename T, typename K>
class Generic {
public:

    explicit Generic(const T& data, const K& data2): m_data(data), m_data2(data2) {};

    void displayData() const {
        std::cout << "Valeur de data = " << m_data << std::endl;
        std::cout << "Valeur de data  2= " << m_data2 << std::endl;
    }


private:
    T m_data;
    K m_data2;
};





template <typename T>
class Generic<T,std::string> {
public:

    explicit Generic(const T& data, std::string data2): m_data(data), m_data2(std::move(data2)) {};

    void displayData() const {
        std::cout << "Valeur de data = " << m_data << std::endl;
        std::cout << "Valeur de data2 = " << m_data2 << std::endl;
    }

    [[nodiscard]] std::string concatenate(const std::string& other) const {
        return this->m_data2 + other;
    }


private:
    T m_data;
    std::string m_data2;


};



#endif //TUTO_CPP_TEMPLATE_H
