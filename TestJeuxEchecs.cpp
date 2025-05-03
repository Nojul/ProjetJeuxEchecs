/**
* Programme permettant de tester le programme avec des test unitaires sans interface graphique
* \file   TestJeuxEchecs.cpp
* \author Latendresse et Julien
* \date   4 mai 2025
* Cree le 6 avril 2025
*/

// Fichier de test pour le projet Jeux d'echecs
// Ce fichier contient les tests unitaires pour le projet Jeux d'echecs.
// Pour le moment, il y a seulement des tests d'exemple.
// Vous devez ajouter vos propres tests pour verifier le bon fonctionnement de votre code.
//
// Include ici les headers de votre partie de code a tester
#include "Pieces.h"
#include "ProjetJeuxEchecs.h"

#if __has_include("gtest/gtest.h")
#include "gtest/gtest.h"
#endif
#ifdef TEST

TEST(Test, exempleDeTest)
{
	ModeleJeu::JeuPrincipal jeu(0);

	EXPECT_EQ(1, 1);
}

TEST(Test, exempleDeTest2)
{
	EXPECT_FALSE(1 == 2);
}
#endif
