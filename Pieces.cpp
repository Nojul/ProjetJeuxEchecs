/**
* Definition des composants du jeu dechecs pieces et echiquier
* \file   Pieces.cpp
* \author Latendresse et Julien
* \date   4 mai 2025
* Cree le 6 avril 2025
*/
#include "Pieces.h"
#include "ProjetJeuxEchecs.h"
#include <cmath>
#include <iostream>

std::string ModeleJeu::Piece::getCouleur() { return couleur_; }
int ModeleJeu::Piece::getPositionX() const { return posX_; }
int ModeleJeu::Piece::getPositionY() const { return posY_; }

void ModeleJeu::Piece::deplacer(int x, int y) {
	posX_ = x;
	posY_ = y;
}

ModeleJeu::Roi::~Roi() {
	compteurRoi_--;
}

ModeleJeu::Roi::Roi(int posX_Debut, int posY_Debut, std::string couleur) :Piece(posX_Debut, posY_Debut, couleur) {
	if (compteurRoi_ < 2) {
		compteurRoi_++;
	}
	else {
		throw CompteurRoisException(" Plus de deux instancess de rois ");
	}
}

int ModeleJeu::Roi::compteurRoi_ = 0;
int ModeleJeu::Roi::getCompteurRoi() {
	return compteurRoi_;
}

bool ModeleJeu::Roi::estMouvementValide(int x, int y)
{
	int deltaX = abs(x - posX_);
	int deltaY = abs(y - posY_);
	return deltaX <= 1 && deltaY <= 1 && (deltaX != 0 || deltaY != 0);
}

bool ModeleJeu::Tour::estMouvementValide(int x, int y)
{
	return (x == posX_ || y == posY_) && (x != posX_ || y != posY_);
}

bool ModeleJeu::Cavalier::estMouvementValide(int x, int y)
{
	int deltaX = abs(x - posX_);
	int deltaY = abs(y - posY_);
	return (deltaX == 2 && deltaY == 1) || (deltaX == 1 && deltaY == 2);
}

void ModeleJeu::JeuPrincipal::ajouterPiece(int posX_, int posY_, std::string couleurDonne, std::string typePiece) {
	if (posX_ < 0 || posX_ >= 8 || posY_ < 0 || posY_ >= 8) {
		throw std::out_of_range("Position hors des limites du plateau");
	}
	if (echiquier_[posX_][posY_]) {
		echiquier_[posX_][posY_].reset();
	}

	if (typePiece == "Roi") {
		try {
			auto nouvellePiece = std::make_unique<Roi>(posX_, posY_, couleurDonne);
			echiquier_[posX_][posY_] = std::move(nouvellePiece);
		}
		catch (const CompteurRoisException& e) {
			std::cerr << "Erreur roi : " << e.what() << std::endl;
			echiquier_[posX_][posY_].reset();
			throw;
		}
	}
	else if (typePiece == "Tour") {
		echiquier_[posX_][posY_] = std::make_unique<Tour>(posX_, posY_, couleurDonne);
	}
	else if (typePiece == "Cavalier") {
		echiquier_[posX_][posY_] = std::make_unique<Cavalier>(posX_, posY_, couleurDonne);
	}

	std::cout << "Ajout de " << typePiece << " " << couleurDonne << " sur (" << posX_ << ", " << posY_ << ")" << std::endl;
}


bool ModeleJeu::JeuPrincipal::verifierContraintesEchiquier(int posX, int posY, int nouvPosX, int nouvPosY) {
	Piece* piece = echiquier_[posX][posY].get();
	if (!piece) return false;

	if (echiquier_[nouvPosX][nouvPosY] && echiquier_[nouvPosX][nouvPosY]->getCouleur() == piece->getCouleur()) {
		return false;
	}

	if (dynamic_cast<Tour*>(piece)) {
		int startX = std::min(posX, nouvPosX);
		int endX = std::max(posX, nouvPosX);
		int startY = std::min(posY, nouvPosY);
		int endY = std::max(posY, nouvPosY);

		if (posX == nouvPosX) {
			for (int i = startY + 1; i < endY; ++i) {
				if (echiquier_[posX][i] != nullptr) {
					return false;
				}
			}
		}
		else if (posY == nouvPosY) {
			for (int i = startX + 1; i < endX; ++i) {
				if (echiquier_[i][posY] != nullptr) {
					return false;
				}
			}
		}
	}

	return true;
}

std::tuple<bool, std::string> ModeleJeu::JeuPrincipal::deplacerPiece(int posX_, int posY_, std::string couleurJoueur, int nouvposX_, int nouvposY_) {
	if (echiquier_[posX_][posY_] == nullptr)
	{
		std::cout << "Aucune piece selectionnee" << std::endl;
		return{ false, "Aucune piece selectionnee" };
	}
	if (echiquier_[posX_][posY_]->getCouleur() != couleurJoueur)
	{
		std::cout << "La piece a deplacer ne correspond pas avec la couleur du joueur." << std::endl;
		return{ false, "La piece a deplacer ne correspond pas avec la couleur du joueur." };
	}

	try {
		Piece* piece = echiquier_[posX_][posY_].get();

		if (piece->estMouvementValide(nouvposX_, nouvposY_)) {

			if (!verifierContraintesEchiquier(posX_, posY_, nouvposX_, nouvposY_)) {
				return { false, "Deplacement bloque par d'autres pieces" };
			}

			{
				Temporaire pieceTemporaire(posX_, posY_, nouvposX_, nouvposY_, echiquier_);
				if (pieceTemporaire.verifierEchec(couleurJoueur)) {
					return { false, "Ce deplacement place le joueur " + couleurJoueur + " en echec." };
				}
			}

			if (echiquier_[nouvposX_][nouvposY_] != nullptr) {
				std::cout << "Piece eliminee a la position (" << nouvposX_ << "," << nouvposY_ << ")" << std::endl;
				echiquier_[nouvposX_][nouvposY_] = nullptr;
			}

			echiquier_[posX_][posY_]->deplacer(nouvposX_, nouvposY_);
			echiquier_[nouvposX_][nouvposY_] = std::move(echiquier_[posX_][posY_]);
			echiquier_[posX_][posY_] = nullptr;
			std::cout << "Deplacement effectue de (" << posX_ << "," << posY_ << ") a (" << nouvposX_ << "," << nouvposY_ << ")" << std::endl;
			return{ true,"" };
		}
		else {
			std::cerr << "Deplacement invalide pour cette piece" << std::endl;
			return{ false , "Deplacement invalide pour cette piece" };
		}
	}
	catch (const std::exception& e) {
		return { false, "Erreur lors du deplacement: " + std::string(e.what()) };
	}
}

ModeleJeu::Piece* ModeleJeu::JeuPrincipal::getPiece(int x, int y)
{
	return echiquier_[x][y].get();
}

ModeleJeu::Temporaire::Temporaire(int positionX, int positionY, int nouvPositionX, int nouvPositionY, std::unique_ptr<Piece>(&echiquier)[8][8])
	: positionX_(positionX), positionY_(positionY), nouvPositionX_(nouvPositionX), nouvPositionY_(nouvPositionY), echiquier_(echiquier)
{
	piece_ = move(echiquier_[positionX_][positionY_]);
	pieceCapturee_ = move(echiquier_[nouvPositionX_][nouvPositionY_]);
	echiquier_[nouvPositionX_][nouvPositionY_] = move(piece_);
}

ModeleJeu::Temporaire::~Temporaire()
{
	piece_ = move(echiquier_[nouvPositionX_][nouvPositionY_]);
	echiquier_[nouvPositionX_][nouvPositionY_] = move(pieceCapturee_);
	echiquier_[positionX_][positionY_] = move(piece_);
}

ModeleJeu::Piece* ModeleJeu::Temporaire::getTemporaire()
{
	return echiquier_[nouvPositionX_][nouvPositionY_].get();
}

bool ModeleJeu::Temporaire::verifierEchec(std::string couleurJoueur)
{
	int positionRoiX = -1;
	int positionRoiY = -1;
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Piece* piece = echiquier_[i][j].get();
			if (piece and dynamic_cast<ModeleJeu::Roi*>(piece) and piece->getCouleur() == couleurJoueur)
			{
				positionRoiX = i;
				positionRoiY = j;
			}
		}
	}
	if (positionRoiX == -1 and positionRoiY == -1)
	{
		std::cout << "Le roi n'a pas ete trouve." << std::endl;
		return false;
	}
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			Piece* pieceAdverse = echiquier_[i][j].get();
			if (pieceAdverse and pieceAdverse->estMouvementValide(positionRoiX, positionRoiY) and ModeleJeu::JeuPrincipal::verifierContraintesEchiquier(i, j, positionRoiX, positionRoiX) and pieceAdverse->getCouleur() != couleurJoueur)
			{
				return true;
			}
		}
	}
	return false;
}

ModeleJeu::JeuPrincipal::JeuPrincipal(int placement) {
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			echiquier_[i][j] = nullptr;
		}
	}
	//QtWidgets.QComboBox.currentIndex()
	//Return type :
	//int
	//un combobox au debut pour choisir quel position faire, le combobox donne un int dependant du choix
	//https://en.wikipedia.org/wiki/Pawnless_chess_endgame
	/*    0 1 2 3 4 5 6 7
		0 . . . . . . . .
		1 . . . . . . . .
		2 . . . . . . . .
		3 . . . . . . . .
		4 . . . . . . . .
		5 . . . . . . . .
		6 . . . . . . . .
		7 . . . . . . . .
		  0 1 2 3 4 5 6 7*/

	try {
		switch (placement) {
		case 0: // La Bourdonnais vs. McDonnell, 1834
			std::cout << "(0) Debut de la partie : La Bourdonnais vs. McDonnell, 1834" << std::endl;
			ajouterPiece(0, 1, "Blanc", "Tour");
			ajouterPiece(1, 0, "Noir", "Cavalier");
			ajouterPiece(3, 1, "Noir", "Roi");
			ajouterPiece(3, 2, "Blanc", "Roi");
			break;

		case 1: // Karpov vs. Ftacnik, 1988
			std::cout << "(1) Debut de la partie : Karpov vs. Ftacnik, 1988" << std::endl;
			ajouterPiece(1, 5, "Blanc", "Tour");
			ajouterPiece(2, 1, "Noir", "Cavalier");
			ajouterPiece(5, 7, "Noir", "Roi");
			ajouterPiece(2, 4, "Blanc", "Roi");
			break;

		case 2: // J. Polgar vs. Kasparov, 1996
			std::cout << "Debut de la partie : J. Polgar vs. Kasparov, 1996" << std::endl;
			ajouterPiece(0, 4, "Blanc", "Tour");
			ajouterPiece(7, 6, "Noir", "Tour");
			ajouterPiece(6, 4, "Noir", "Cavalier");
			ajouterPiece(5, 5, "Noir", "Roi");
			ajouterPiece(4, 7, "Blanc", "Roi");
			break;

		case 3: // Alekhine vs. Capablanca, 1927
			std::cout << "Debut de la partie : Alekhine vs. Capablanca, 1927" << std::endl;
			ajouterPiece(6, 1, "Blanc", "Tour");
			ajouterPiece(1, 0, "Noir", "Tour");
			ajouterPiece(1, 3, "Noir", "Cavalier");
			ajouterPiece(2, 3, "Noir", "Roi");
			ajouterPiece(4, 3, "Blanc", "Roi");
			break;
		}
	}
	catch (const CompteurRoisException& e) {
		std::cerr << "Erreur : " << e.what() << std::endl;
		for (int i = 0; i < 8; ++i) {
			for (int j = 0; j < 8; ++j) {
				if (echiquier_[i][j]) {
					echiquier_[i][j].reset();
				}
			}
		}
		throw;
	}
}
