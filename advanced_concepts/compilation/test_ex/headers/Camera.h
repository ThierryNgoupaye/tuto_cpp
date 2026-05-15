//
// Created by thierry on 15/05/2026.
//

#ifndef TUTO_CPP_CAMERA_H
#define TUTO_CPP_CAMERA_H

#include <vector>
#include <cstdint>
#include <string>


struct T_Frame {
    int largeur;
    int hauteur;
    std::vector<uint8_t> pixels;  // données brutes RGB

    [[nodiscard]] bool empty() const { return pixels.empty(); }
};

class T_Camera {
public:
    T_Camera();

    void initialiser();
    void fermer();
    void setResolution(int largeur, int hauteur);

    [[nodiscard]] T_Frame capturerFrame() const;

    [[nodiscard]] int getLargeur()  const { return m_largeur; }
    [[nodiscard]] int getHauteur()  const { return m_hauteur; }
    [[nodiscard]] bool estOuverte()  const { return m_ouverte; }

private:
    int  m_largeur  = 1920;
    int  m_hauteur  = 1080;
    bool m_ouverte  = false;
};



#endif //TUTO_CPP_CAMERA_H
