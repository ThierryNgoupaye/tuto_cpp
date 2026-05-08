//
// Created by thierry on 05/05/2026.
//

#ifndef TUTO_CPP_STRINGVIEW_USE_H
#define TUTO_CPP_STRINGVIEW_USE_H

#include <iostream>
#include <ostream>
#include <string_view>


inline void print_str(const std::string_view s) {
    std::cout << s << std::endl;
}

inline std::string_view create_view() {
    const std::string s = "salut";
    return std::string_view(s);   // pas possible car la string value ne sera detruite à la fin de la fonction
}


inline void apply() {
    std::string_view s("Bonjour");
    print_str(std::string("Bonjour tout le monde"));

    const char* s1 = "Bonjour";
    print_str(s1);
}


#endif //TUTO_CPP_STRINGVIEW_USE_H
