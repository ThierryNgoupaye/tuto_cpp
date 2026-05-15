//
// Created by thierry on 15/05/2026.
//

#include <gtest/gtest.h>
#include "Calculatrice.h"


TEST(CalculatriceTest, AdditionnerDeuxPositifs) {
    Calculatrice cal;
    EXPECT_EQ(cal.additionner(3,5), 8);
}

TEST(CalculatriceTest, AdditionnerAvecNegatif) {
    Calculatrice cal;
    EXPECT_EQ(cal.additionner(-3, 5), 2);
    EXPECT_EQ(cal.additionner(-3,-5), -8);
}

TEST(CalculatriceTest, SoustraireCorrectement) {
    Calculatrice calc;
    EXPECT_EQ(calc.soustraire(10, 3), 7);
    EXPECT_EQ(calc.soustraire(3, 10), -7);
}

TEST(CalculatriceTest, DiviserCorrectement) {
    Calculatrice calc;
    EXPECT_DOUBLE_EQ(calc.diviser(10.0, 2.0), 5.0);
    EXPECT_NEAR(calc.diviser(1.0, 3.0), 0.333, 0.001);
    // ↑ pour les flottants → NEAR avec tolérance !
}


TEST(CalculatriceTest, DiviserParZeroLeveException) {
    Calculatrice calc;
    EXPECT_THROW(
        calc.diviser(10.0, 0.0),
        std::invalid_argument
    );
}

TEST(CalculatriceTest, EstPairCorrectement) {
    Calculatrice calc;
    EXPECT_TRUE(calc.estPair(4));
    EXPECT_TRUE(calc.estPair(0));
    EXPECT_FALSE(calc.estPair(3));
    EXPECT_FALSE(calc.estPair(-1));
}


// // ─────────────────────────────────────
// // Égalité
// // ─────────────────────────────────────
// EXPECT_EQ(valeur, attendu);      // valeur == attendu
// EXPECT_NE(valeur, attendu);      // valeur != attendu
//
// // ─────────────────────────────────────
// // Comparaisons
// // ─────────────────────────────────────
// EXPECT_LT(valeur, attendu);      // valeur < attendu
// EXPECT_LE(valeur, attendu);      // valeur <= attendu
// EXPECT_GT(valeur, attendu);      // valeur > attendu
// EXPECT_GE(valeur, attendu);      // valeur >= attendu
//
// // ─────────────────────────────────────
// // Booléens
// // ─────────────────────────────────────
// EXPECT_TRUE(condition);
// EXPECT_FALSE(condition);
//
// // ─────────────────────────────────────
// // Flottants → JAMAIS EXPECT_EQ !
// // ─────────────────────────────────────
// EXPECT_FLOAT_EQ(valeur, attendu);   // float (6 chiffres)
// EXPECT_DOUBLE_EQ(valeur, attendu);  // double (15 chiffres)
// EXPECT_NEAR(valeur, attendu, delta);// |valeur - attendu| <= delta
//
// // ─────────────────────────────────────
// // Strings
// // ─────────────────────────────────────
// EXPECT_EQ(str1, str2);          // std::string → EXPECT_EQ suffit
// EXPECT_STREQ(cstr1, cstr2);     // const char* → utiliser STREQ
// EXPECT_STRNE(cstr1, cstr2);     // const char* différentes
//
// // ─────────────────────────────────────
// // Exceptions
// // ─────────────────────────────────────
// EXPECT_THROW(expression, TypeException);  // lève cette exception
// EXPECT_NO_THROW(expression);              // ne lève pas d'exception
// EXPECT_ANY_THROW(expression);             // lève n'importe quelle exception
//
// // ─────────────────────────────────────
// // Messages custom en cas d'échec
// // ─────────────────────────────────────
// EXPECT_EQ(calc.additionner(3, 5), 8)
//     << "3 + 5 devrait valoir 8 !";
// // ↑ << ajoute un message si le test échoue




struct ParamsAddition {
    int a, b, attendu;
};

class AdditionTest : public testing::TestWithParam<ParamsAddition> {};


TEST_P(AdditionTest, AdditionnerCorrectement) {
    Calculatrice calc;
    auto [a, b, attendu] = GetParam();
    EXPECT_EQ(calc.additionner(a,b), attendu);
}



// Définir tous les cas de tests

INSTANTIATE_TEST_SUITE_P(CasAddition, AdditionTest, ::testing::Values(
        ParamsAddition{3,   5,   8},
        ParamsAddition{0,   0,   0},
        ParamsAddition{-3,  5,   2},
        ParamsAddition{-3, -5,  -8},
        ParamsAddition{100, 200, 300}
    )
);




