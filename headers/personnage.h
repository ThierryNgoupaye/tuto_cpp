//
// Created by thierry on 29/04/2026.
//

#ifndef TUTO_CPP_PERSONNAGE_H
#define TUTO_CPP_PERSONNAGE_H

class Personnage {

public:
    virtual ~Personnage() = default;

    Personnage(const unsigned int& pdv, const unsigned int& vitesse, const unsigned int& level);

    virtual void attaquer(Personnage& other);

    virtual void displayAttributes();

    void setPdv(const unsigned int& pdv);

    [[nodiscard]] unsigned int getPdv() const;


protected:
    unsigned int m_pdv;
    unsigned int m_vitesse;
    unsigned int m_level;
};



class Guerrier : public Personnage {

    public:
        Guerrier(const unsigned int& pdv, const unsigned int&  vitesse, const unsigned int& level, const unsigned int& force);

        void attaquer(Personnage &other) override;

        void displayAttributes() override;

    protected:
        unsigned int m_force;

};


class GuerrierDef: Guerrier {

    public:
        GuerrierDef(const unsigned int& pdv, const unsigned int&  vitesse, const unsigned int& level, const unsigned int& force, const unsigned int& armor);
        void attaquer(Personnage& other) override;
        void displayAttributes() override;

    private:
        unsigned m_armor;
};


#endif //TUTO_CPP_PERSONNAGE_H
