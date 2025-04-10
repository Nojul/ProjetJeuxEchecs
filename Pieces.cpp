#include "Pieces.h"
#include "ProjetJeuxEchecs.h"
#include <cmath>
#include <iostream>

//Commentaire de noe: il manque de savoir si il y a des pieces sur le chemin du deplacement pour voir si il faut arreter le mouvement de la piece et les namespaces

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

void ModeleJeu::JeuPrincipal::deplacerPiece(int posX, int posY, std::string couleurJoueur, int nouvPosX, int nouvPosY) {
	if (echiquier[posX][posY] != nullptr && echiquier[posX][posY]->getCouleur() == couleurJoueur) {
		try {
			if (echiquier[posX][posY].get()->verifierDeplacement(nouvPosX, nouvPosY, echiquier)) {

				if (echiquier[nouvPosX][nouvPosY] != nullptr) {
					std::cout << "Piece eliminee a la position (" << nouvPosX << "," << nouvPosY << ")" << std::endl;
					echiquier[nouvPosX][nouvPosY] = nullptr;
				}

				echiquier[posX][posY]->deplacer(nouvPosX, nouvPosY);
				echiquier[nouvPosX][nouvPosY] = std::move(echiquier[posX][posY]);
				echiquier[posX][posY] = nullptr;
				std::cout << "Deplacement effectue de (" << posX << "," << posY << ") a (" << nouvPosX << "," << nouvPosY << ")" << std::endl;
			}
			else {
				std::cerr << "Deplacement invalide pour cette piece" << std::endl;
			}
		}
		catch (const std::exception& e) {
			std::cerr << "Erreur lors du deplacement : " << e.what() << std::endl;
		}
	}
	else {
		std::cout << "Erreur : aucune piece a cette position ou mauvaise couleur." << std::endl;
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

	case 0: //La Bourdonnais vs. McDonnell, 1834
		echiquier[0][1] = std::make_unique<Tour>(0, 1, "Blanc");
		echiquier[1][0] = std::make_unique<Cavalier>(1, 0, "Noir");
		echiquier[3][1] = std::make_unique<Roi>(3, 1, "Noir");
		echiquier[3][2] = std::make_unique<Roi>(3, 2, "Blanc");
		break;

	case 1: //Karpov vs. Ftacnik, 1988

		break;

	case 2: //J. Polgar vs. Kasparov, 1996[40]

		break;

	case 3: //Alekhine vs. Capablanca, 1927[41]

		break;

	}
}


