//
// Created by thierry on 01/05/2026.
//

#ifndef TUTO_CPP_FILE_HANDLER_H
#define TUTO_CPP_FILE_HANDLER_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string>


inline void ecrire_fichier(const std::string& path,const  std::vector<std::string>& words) {

    if (std::ofstream my_file(path); my_file.is_open()) {
        for (const auto& word : words) {
            my_file << word << std::endl;
        }
    } else {
        std::cerr << "Erreur lors de l'ouverture du fichier \n";
    }
}



inline void lire_fichier(const std::string& path) {
    std::ifstream my_file(path);
    if (my_file.is_open()) {
        std::string line;
        while (std::getline(my_file, line)) {
            std::cout << line << std::endl;
        }
    }
    my_file.close();
}


#endif //TUTO_CPP_FILE_HANDLER_H
