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

//TEST(Test, exempleDeTest)
//{
//	/*    0 1 2 3 4 5 6 7
//	 *  0 . . . . . . . .
//	 *  1 . . . . . . . .
//	 *  2 . . . . . . . .
//	 *  3 . . . . . . . .
//	 *  4 . . . . . . . .
//	 *  5 . . . . . . . .
//	 *  6 . . . . . . . .
//	 *  7 . . . . . . . .
//	 *    0 1 2 3 4 5 6 7
//	 */
//
//	std::cout << "\nPartie de test avec le controleur seulement:" << std::endl;
//	auto jeu = ModeleJeu::JeuPrincipal(ModeleJeu::Placement::LaBourdonaisMcDonnell1834);
//	jeu.deplacerPiece(ModeleJeu::Coordonnee(0, 1), ModeleJeu::Couleur::Blanc, ModeleJeu::Coordonnee(0, 0));
//	jeu.deplacerPiece(ModeleJeu::Coordonnee(3, 0), ModeleJeu::Couleur::Noir, ModeleJeu::Coordonnee(2, 0));
//	jeu.deplacerPiece(ModeleJeu::Coordonnee(0, 0), ModeleJeu::Couleur::Blanc, ModeleJeu::Coordonnee(0, 2));
//	jeu.deplacerPiece(ModeleJeu::Coordonnee(1, 0), ModeleJeu::Couleur::Noir, ModeleJeu::Coordonnee(0, 2));
//	std::cout << "\nFin de la partie de test avec controleur seulement\n" << std::endl;
//
//	EXPECT_EQ(1, 1);
//}
//
//TEST(Test, exempleDeTest2)
//{
//	EXPECT_FALSE(1 == 2);
//}

TEST(Test, RoiMouvementValide) {
	ModeleJeu::JeuPrincipal jeu(ModeleJeu::Placement::LaBourdonaisMcDonnell1834);
	ModeleJeu::Coordonnee positionRoi(3, 2);
	auto* piece = jeu.getPiece(positionRoi);
	ASSERT_NE(piece, nullptr);

	auto* roi = dynamic_cast<ModeleJeu::Roi*>(piece);
	ASSERT_NE(roi, nullptr);

	EXPECT_TRUE(roi->estMouvementValide(positionRoi, ModeleJeu::Coordonnee(3, 3)));
	EXPECT_TRUE(roi->estMouvementValide(positionRoi, ModeleJeu::Coordonnee(4, 3)));
	EXPECT_TRUE(roi->estMouvementValide(positionRoi, ModeleJeu::Coordonnee(4, 2)));
	EXPECT_FALSE(roi->estMouvementValide(positionRoi, ModeleJeu::Coordonnee(3, 4)));
}

TEST(Test, TourMouvementValide) {
	ModeleJeu::JeuPrincipal jeu(ModeleJeu::Placement::LaBourdonaisMcDonnell1834);
	ModeleJeu::Coordonnee positionTour(0, 1);
	auto* piece = jeu.getPiece(positionTour);
	ASSERT_NE(piece, nullptr);

	auto* tour = dynamic_cast<ModeleJeu::Tour*>(piece);
	ASSERT_NE(tour, nullptr);

	EXPECT_TRUE(tour->estMouvementValide(positionTour, ModeleJeu::Coordonnee(0, 6)));
	EXPECT_TRUE(tour->estMouvementValide(positionTour, ModeleJeu::Coordonnee(4, 1)));
	EXPECT_FALSE(tour->estMouvementValide(positionTour, ModeleJeu::Coordonnee(2, 4)));
}

TEST(Test, CavalierMouvementValide) {
	ModeleJeu::JeuPrincipal jeu(ModeleJeu::Placement::LaBourdonaisMcDonnell1834);
	ModeleJeu::Coordonnee positionCavalier(1, 0);
	auto* piece = jeu.getPiece(positionCavalier);
	ASSERT_NE(piece, nullptr);

	auto* cavalier = dynamic_cast<ModeleJeu::Cavalier*>(piece);
	ASSERT_NE(cavalier, nullptr);

	EXPECT_TRUE(cavalier->estMouvementValide(positionCavalier, ModeleJeu::Coordonnee(3, 1)));
	EXPECT_TRUE(cavalier->estMouvementValide(positionCavalier, ModeleJeu::Coordonnee(0, 2)));
	EXPECT_FALSE(cavalier->estMouvementValide(positionCavalier, ModeleJeu::Coordonnee(2, 4)));
}

TEST(Test, VerifierContraintesEchiquier) {
	ModeleJeu::JeuPrincipal jeu(ModeleJeu::Placement::LaBourdonaisMcDonnell1834);
	ModeleJeu::Coordonnee positionDebut1(1, 0);
	ModeleJeu::Coordonnee positionFin1(2, 0);
	ModeleJeu::Coordonnee positionDebut2(0, 0);

	EXPECT_TRUE(jeu.verifierContraintesEchiquier(positionDebut1, positionFin1));
	EXPECT_FALSE(jeu.verifierContraintesEchiquier(positionDebut2, positionFin1));
	
	//Tour
	jeu.deplacerPiece({ 0,1 }, ModeleJeu::Couleur::Blanc, { 0,2 });
	ModeleJeu::Coordonnee positionDebut3(0, 2);
	ModeleJeu::Coordonnee positionFin2(6, 2);
	EXPECT_FALSE(jeu.verifierContraintesEchiquier(positionDebut3, positionFin2));
	//Arriver sur la meme couleur
	ModeleJeu::Coordonnee positionFin3(3, 2);
	EXPECT_FALSE(jeu.verifierContraintesEchiquier(positionDebut3, positionFin3));
}

TEST(Test, RoiNePeutPlusBouger) {
	ModeleJeu::JeuPrincipal jeu(ModeleJeu::Placement::LaBourdonaisMcDonnell1834);
	jeu.deplacerPiece({ 0,1 }, ModeleJeu::Couleur::Blanc, { 2,1 });
	jeu.deplacerPiece({ 3,0 }, ModeleJeu::Couleur::Noir, { 4,0 });
	jeu.deplacerPiece({ 3,2 }, ModeleJeu::Couleur::Blanc, { 4,2 });
	jeu.deplacerPiece({ 4,0 }, ModeleJeu::Couleur::Noir, { 5,0 });
	jeu.deplacerPiece({ 4,2 }, ModeleJeu::Couleur::Blanc, { 5,2 });
	jeu.deplacerPiece({ 5,0 }, ModeleJeu::Couleur::Noir, { 6,0 });
	jeu.deplacerPiece({ 5,2 }, ModeleJeu::Couleur::Blanc, { 6,2 });
	jeu.deplacerPiece({ 6,0 }, ModeleJeu::Couleur::Noir, { 7,0 });
	jeu.deplacerPiece({ 2,1 }, ModeleJeu::Couleur::Blanc, { 2,0 });

	EXPECT_TRUE(jeu.roiNePeutPlusBouger(ModeleJeu::Couleur::Noir));
	EXPECT_FALSE(jeu.roiNePeutPlusBouger(ModeleJeu::Couleur::Blanc));
}

TEST(Test, VerifierEchec) {
	ModeleJeu::JeuPrincipal jeu(ModeleJeu::Placement::LaBourdonaisMcDonnell1834);
	jeu.deplacerPiece({ 0,1 }, ModeleJeu::Couleur::Blanc, { 3,1 });

	EXPECT_TRUE(jeu.verifierEchec(ModeleJeu::Couleur::Noir));
	EXPECT_FALSE(jeu.verifierEchec(ModeleJeu::Couleur::Blanc));
}

TEST(Test, DeplacerPiece)
{
	auto jeu = ModeleJeu::JeuPrincipal(ModeleJeu::Placement::LaBourdonaisMcDonnell1834);
	jeu.deplacerPiece(ModeleJeu::Coordonnee(3, 2), ModeleJeu::Couleur::Blanc, ModeleJeu::Coordonnee(3, 6));
	jeu.deplacerPiece(ModeleJeu::Coordonnee(1, 0), ModeleJeu::Couleur::Noir, ModeleJeu::Coordonnee(1, 6));
	jeu.deplacerPiece(ModeleJeu::Coordonnee(0, 1), ModeleJeu::Couleur::Blanc, ModeleJeu::Coordonnee(0, 6));

	EXPECT_EQ(jeu.getPiece(ModeleJeu::Coordonnee(3, 6)), nullptr);
	EXPECT_EQ(jeu.getPiece(ModeleJeu::Coordonnee(1, 6)), nullptr);
	EXPECT_NE(jeu.getPiece(ModeleJeu::Coordonnee(0, 6)), nullptr);
}

#endif