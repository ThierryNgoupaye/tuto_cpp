//
// Created by thierry on 30/04/2026.
//

#ifndef TUTO_CPP_ITERATOR_H
#define TUTO_CPP_ITERATOR_H
#include <iostream>
#include <ostream>
#include <string>


class Container {

public:
    explicit Container(const unsigned int size) : m_size(size) {
        array = new int[m_size];
        for (int i = 0; i < m_size; i++) {
            array[i] = i + 1;
        }
    }

    ~Container() {
        delete[] array;
    }

    [[nodiscard]] unsigned int size() const {
        return m_size;
    }


    class iterator {
        public:
        explicit iterator(int* arr): it(arr){}

        int& operator*() const {
            return *this->it;
        }

        iterator& operator++() {
            ++this->it;
            return *this;
        }


        iterator& operator--() {
            --this->it;
            return *this;
        }

        iterator& operator+(const int i) {
            this->it += i;
            return *this;
        }

        iterator& operator-(const int i) {
            this->it -= i;
            return *this;
        }


        bool operator!=(const iterator& other) const {
            return it!= other.it;
        }


        private:
            int* it;

    };


    [[nodiscard]] iterator begin() const {
        return iterator(array);
    }

    [[nodiscard]] iterator end() const {
        return iterator(array +m_size);
    }




private:
    int* array;
    const unsigned int  m_size;
};





class M_Container {
    public:

        M_Container(const int size, std::string name): m_size(size), m_name(std::move(name)) {
            _array_ = new int[m_size];
            std::cout << m_name  << " crée. Taille = "<< m_size <<  std::endl;
        }

        M_Container(const M_Container& c) : m_size(c.m_size) {
            this->_array_ = new int[m_size];
            for (int i = 0; i < m_size; i++) {
                this->_array_[i] = c._array_[i];
            }
            std::cout << "Copie effectuée avec succes" << std::endl;
        }

        M_Container(M_Container&& c)  noexcept : m_size(c.m_size) , m_name(std::move(c.m_name)), _array_(c._array_)  {
            c._array_ = nullptr;
            c.m_size = 0;
            c.m_name = "";
        }

        M_Container& operator=(M_Container&& c) noexcept {
            if (this != &c) {
                delete[] this->_array_;
                m_size = c.m_size;
                m_name = std::move(c.m_name);
                _array_ = c._array_;

                c._array_ = nullptr;
                c.m_size = 0;
                c.m_name = "";
            }
            return *this;
        }


        ~M_Container() {
            delete[] this->_array_;
            std::cout << m_name << " détruit." << std::endl;
        }

        int size() const {
            return this->m_size;
        }



    private:
        mutable int m_size;
        mutable std::string m_name;
        mutable int* _array_;
};


#endif //TUTO_CPP_ITERATOR_H
