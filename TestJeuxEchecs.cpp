/**
 * Programme permettant de tester le programme avec des tests unitaires sans interface graphique
 * \file   TestJeuxEchecs.cpp
 * \author Latendresse et Julien
 * \date   4 mai 2025
 * Cree le 6 avril 2025
 */

#include "Pieces.h"
#include "ProjetJeuxEchecs.h"
#if __has_include("gtest/gtest.h")
#include "gtest/gtest.h"
#endif
#ifdef TEST

TEST(Test, exempleDeTest)
{
	/*    0 1 2 3 4 5 6 7
	 *  0 . . . . . . . .
	 *  1 . . . . . . . .
	 *  2 . . . . . . . .
	 *  3 . . . . . . . .
	 *  4 . . . . . . . .
	 *  5 . . . . . . . .
	 *  6 . . . . . . . .
	 *  7 . . . . . . . .
	 *    0 1 2 3 4 5 6 7
	 */

	std::cout << "\nPartie de test avec le controleur seulement:" << std::endl;
	ModeleJeu::JeuPrincipal jeu(0);
	jeu.deplacerPiece(ModeleJeu::Coordonnee(0, 1), "Blanc", ModeleJeu::Coordonnee(0, 0));
	jeu.deplacerPiece(ModeleJeu::Coordonnee(3, 0), "Noir", ModeleJeu::Coordonnee(2, 0));
	jeu.deplacerPiece(ModeleJeu::Coordonnee(0, 0), "Blanc", ModeleJeu::Coordonnee(0, 2));
	jeu.deplacerPiece(ModeleJeu::Coordonnee(1, 0), "Noir", ModeleJeu::Coordonnee(0, 2));
	std::cout << "\nFin de la partie de test avec controleur seulement\n" << std::endl;

	EXPECT_EQ(1, 1);
}

TEST(Test, exempleDeTest2)
{
	EXPECT_FALSE(1 == 2);
}

#endif