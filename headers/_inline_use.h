//
// Created by thierry on 29/04/2026.
//

#pragma once

#include <iostream>
#include <ostream>
#include <vector>
#include <functional>

using string = std::string;
using std::vector;


inline int sum(const int& a, const int& b) {
    return a + b;
}

inline vector<string> split(const string& sentence, const char separator) {
    vector<string> words;
    string word;
    for (const char c : sentence) {
        if (c != separator) {
            word += c;
        }
        else {
            if (!word.empty()) {
                words.emplace_back(std::move(word));
                word.clear();
            }
        }
    }
    if (!word.empty()) {
        words.emplace_back(std::move(word));
    }
    return words;
}


// Jeu avec les pointeurs (arithmétique des pointeurs)
inline void displayVecElements(const vector<int>& vec) {
    for (int i = 0; i< vec.size() ; ++i ) {
        std::cout << *(&vec[0] + i) << std::endl;
    }
}


// Pointeurs de fonctions
inline void  function( void(*func)(int, int), const int a, const int b) {
    func(a, b);
}


inline void function(const std::function<void(int, int)>& fun, const int a, const int b) {
    fun(a, b);
}




