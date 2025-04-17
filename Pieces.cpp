#include "Pieces.h"
#include "ProjetJeuxEchecs.h"
#include <cmath>
#include <iostream>

std::string ModeleJeu::Piece::getCouleur()
{
	return couleur_;
}

void ModeleJeu::Piece::deplacer(int x, int y) {
	posX = x;
	posY = y;
}
ModeleJeu::Roi::~Roi() {
	compteurRoi_--;
}

ModeleJeu::Roi::Roi(int posXDebut, int posYDebut, std::string couleur) :Piece(posXDebut, posYDebut, couleur) {
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

bool ModeleJeu::Roi::verifierDeplacement(int x, int y, std::unique_ptr<Piece> echiquier[8][8]) {
	if (abs(posX - x) <= 1 && abs(posY - y) <= 1) {
		if (echiquier[x][y] != nullptr && echiquier[x][y]->getCouleur() == couleur_) {
			return false;
		}
		return true;
	}
	return false;
}

bool ModeleJeu::Tour::verifierDeplacement(int x, int y, std::unique_ptr<Piece> echiquier[8][8]) {
	if (posX == x || posY == y) {
		int startX = std::min(posX, x);
		int endX = std::max(posX, x);
		int startY = std::min(posY, y);
		int endY = std::max(posY, y);

		if (posX == x) {
			for (int i = startY + 1; i < endY; ++i) {
				if (echiquier[posX][i] != nullptr) {
					return false;
				}
			}
		}
		else if (posY == y) {
			for (int i = startX + 1; i < endX; ++i) {
				if (echiquier[i][posY] != nullptr) {
					return false;
				}
			}
		}
		if (echiquier[x][y] != nullptr && echiquier[x][y]->getCouleur() == couleur_) {
			return false;
		}
		return true;
	}
	return false;
}

bool ModeleJeu::Cavalier::verifierDeplacement(int x, int y, std::unique_ptr<Piece> echiquier[8][8]) {
	int dx = abs(posX - x);
	int dy = abs(posY - y);
	if ((dx == 2 && dy == 1) || (dx == 1 && dy == 2)) {
		if (echiquier[x][y] != nullptr && echiquier[x][y]->getCouleur() == couleur_) {
			return false;
		}
		return true;
	}
	return false;
}

void ModeleJeu::JeuPrincipal::ajouterPiece(int posX, int posY, std::string couleurDonne, std::string typePiece) {
	if (posX < 0 || posX >= 8 || posY < 0 || posY >= 8) {
		std::cerr << "Position (" << posX << ", " << posY << ") est hors des limites du plateau." << std::endl;
		return;
	}
	if (typePiece == "Roi")
		try {
		echiquier[posX][posY] = std::make_unique<Roi>(posX, posY, couleurDonne);
	}
	catch (CompteurRoisException e) {
		std::cerr << "Erreur lors de l'ajout : " << e.what() << std::endl;
		return;
	}
	else if (typePiece == "Tour")
		echiquier[posX][posY] = std::make_unique<Tour>(posX, posY, couleurDonne);
	else if (typePiece == "Cavalier")
		echiquier[posX][posY] = std::make_unique<Cavalier>(posX, posY, couleurDonne);
	std::cout << "Ajout de " << typePiece << " " << couleurDonne << " sur (" << posX << ", " << posY << ")" << std::endl;
}

std::tuple<bool,std::string> ModeleJeu::JeuPrincipal::deplacerPiece(int posX, int posY, std::string couleurJoueur, int nouvPosX, int nouvPosY) {
	if (echiquier[posX][posY] == nullptr) 
	{
		std::cout << "Aucune piece selectionnee" << std::endl;
		return{ false, "Aucune piece selectionnee" };
	}
	if (echiquier[posX][posY]->getCouleur() != couleurJoueur) 
	{
		std::cout << "La piece a deplacer ne correspond pas avec la couleur du joueur." << std::endl;
		return{false, "La piece a deplacer ne correspond pas avec la couleur du joueur."};
	}
	try {
		if (echiquier[posX][posY].get()->verifierDeplacement(nouvPosX, nouvPosY, echiquier))
		{
			{
				Temporaire pieceTemporaire(posX, posY, nouvPosX, nouvPosY, echiquier);
				if (pieceTemporaire.verifierEchec(couleurJoueur))
				{
					std::cout << "Ce deplacement place le joueur " << couleurJoueur << " en echec." << std::endl;
					return {false, "Ce deplacement place le joueur " + couleurJoueur + " en echec." };
				}
			}
			if (echiquier[nouvPosX][nouvPosY] != nullptr)
			{
				std::cout << "Piece eliminee a la position (" << nouvPosX << "," << nouvPosY << ")" << std::endl;
				echiquier[nouvPosX][nouvPosY] = nullptr;
			}

			echiquier[posX][posY]->deplacer(nouvPosX, nouvPosY);
			echiquier[nouvPosX][nouvPosY] = std::move(echiquier[posX][posY]);
			echiquier[posX][posY] = nullptr;
			std::cout << "Deplacement effectue de (" << posX << "," << posY << ") a (" << nouvPosX << "," << nouvPosY << ")" << std::endl;
			return{true,""};
		}
		else {
			std::cerr << "Deplacement invalide pour cette piece" << std::endl;
			return{ false , "Deplacement invalide pour cette piece"};
		}
	}
	catch (const std::exception& e) {
		std::cerr << "Erreur lors du deplacement : " << e.what() << std::endl;
		return{ false, "Erreur lors du deplacement." };
	}
}

ModeleJeu::JeuPrincipal::JeuPrincipal(int placement) {
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			echiquier[i][j] = nullptr;
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

	switch (placement) {
	case 0: // La Bourdonnais vs. McDonnell, 1834
		std::cout << "Debut de la partie : La Bourdonnais vs. McDonnell, 1834" << std::endl;
		ajouterPiece(0, 1, "Blanc", "Tour");
		ajouterPiece(1, 0, "Noir", "Cavalier");
		ajouterPiece(3, 1, "Noir", "Roi");
		ajouterPiece(3, 2, "Blanc", "Roi");
		break;

	case 1: // Karpov vs. Ftacnik, 1988
		std::cout << "Debut de la partie : Karpov vs. Ftacnik, 1988" << std::endl;
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

ModeleJeu::Piece* ModeleJeu::JeuPrincipal::getPiece(int x, int y)
{
	return echiquier[x][y].get();
}

ModeleJeu::Temporaire::Temporaire(int positionX, int positionY, int nouvPositionX, int nouvPositionY, std::unique_ptr<Piece>(&echiquier)[8][8])
	: positionX_(positionX), positionY_(positionY), nouvPositionX_(nouvPositionX), nouvPositionY_(nouvPositionY), echiquier_(echiquier)
{
	std::unique_ptr<Piece> piece_ = move(echiquier_[positionX_][positionY_]);
	std::unique_ptr<Piece> pieceCapturee_ = move(echiquier_[nouvPositionX_][nouvPositionY_]);
	echiquier_[nouvPositionX_][nouvPositionY_] = move(piece_);
}

ModeleJeu::Temporaire::~Temporaire()
{
	std::unique_ptr<Piece> piece_ = move(echiquier_[nouvPositionX_][nouvPositionY_]);
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
	for (int i=0; i < 8; i++)
	{
		for (int j=0; j < 8; j++)
		{
			Piece* piece = echiquier_[i][j].get();
			if (piece and dynamic_cast<ModeleJeu::Roi*>(piece) and piece->getCouleur() == couleurJoueur)
			{
				Piece* monRoi = piece;
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
	for (int i=0; i < 8; i++) 
	{
		for (int j=0; j < 8; j++)
		{
			Piece* pieceAdverse = echiquier_[i][j].get();
			if (pieceAdverse and pieceAdverse->verifierDeplacement(positionRoiX, positionRoiY, echiquier_) and pieceAdverse->getCouleur() != couleurJoueur) 
			{
				return true;
			}
		}
	}
	return false;
}



