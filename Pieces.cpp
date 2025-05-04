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

ModeleJeu::Couleur ModeleJeu::Piece::getCouleur() const { return couleur_; }

ModeleJeu::Roi::~Roi() {
	compteurRoi_--;
}

ModeleJeu::Roi::Roi(Couleur couleur) : Piece(couleur) {
	if (compteurRoi_ < 2) {
		compteurRoi_++;
	}
	else {
		throw CompteurRoisException(" Plus de deux instancess de rois ");
	}
}

int ModeleJeu::Roi::compteurRoi_ = 0;
int ModeleJeu::Roi::getCompteurRoi() const { return compteurRoi_; }

bool ModeleJeu::Roi::estMouvementValide(const Coordonnee& depart, const Coordonnee& arrivee) {
	int deltaX = abs(depart.x - arrivee.x);
	int deltaY = abs(depart.y - arrivee.y);
	return deltaX <= 1 && deltaY <= 1 && (deltaX != 0 || deltaY != 0);
}

bool ModeleJeu::Tour::estMouvementValide(const Coordonnee& depart, const Coordonnee& arrivee) {
	return (depart.x == arrivee.x || depart.y == arrivee.y) &&
		(depart.x != arrivee.x || depart.y != arrivee.y);
}

bool ModeleJeu::Cavalier::estMouvementValide(const Coordonnee& depart, const Coordonnee& arrivee) {
	int deltaX = abs(depart.x - arrivee.x);
	int deltaY = abs(depart.y - arrivee.y);
	return (deltaX == 2 && deltaY == 1) || (deltaX == 1 && deltaY == 2);
}

void ModeleJeu::JeuPrincipal::ajouterPiece(const Coordonnee& position, Couleur couleur, TypePiece type) {
	if (position.x < 0 || position.x >= tailleEchiquier || position.y < 0 || position.y >= tailleEchiquier) {
		throw std::out_of_range("Position hors des limites du plateau");
	}

	switch (type) {
	case TypePiece::Roi:
		try {
			auto nouvellePiece = std::make_unique<Roi>(couleur);
			echiquier_[position.x][position.y] = std::move(nouvellePiece);
		}
		catch (const CompteurRoisException& e) {
			std::cerr << "Erreur roi : " << e.what() << std::endl;
			echiquier_[position.x][position.y].reset();
			throw;
		}
		break;

	case TypePiece::Tour:
		echiquier_[position.x][position.y] = std::make_unique<Tour>(couleur);
		break;

	case TypePiece::Cavalier:
		echiquier_[position.x][position.y] = std::make_unique<Cavalier>(couleur);
		break;
	}

	std::cout << "  Ajout de " << typeToString(type) << " " << couleurToString(couleur)
		<< " sur (" << position.x << ", " << position.y << ")" << std::endl;
}


bool ModeleJeu::JeuPrincipal::verifierContraintesEchiquier(const Coordonnee& ancienne, const Coordonnee& nouvelle) {
	Piece* piece = echiquier_[ancienne.x][ancienne.y].get();
	if (!piece) return false;

	if (echiquier_[nouvelle.x][nouvelle.y] && echiquier_[nouvelle.x][nouvelle.y]->getCouleur() == piece->getCouleur()) {
		return false;
	}

	if (dynamic_cast<Tour*>(piece)) {
		int startX = std::min(ancienne.x, nouvelle.x);
		int endX = std::max(ancienne.x, nouvelle.x);
		int startY = std::min(ancienne.y, nouvelle.y);
		int endY = std::max(ancienne.y, nouvelle.y);

		if (ancienne.x == nouvelle.x) {
			for (int i = startY + 1; i < endY; ++i) {
				if (echiquier_[ancienne.x][i] != nullptr) {
					return false;
				}
			}
		}
		else if (ancienne.y == nouvelle.y) {
			for (int i = startX + 1; i < endX; ++i) {
				if (echiquier_[i][ancienne.y] != nullptr) {
					return false;
				}
			}
		}
	}

	return true;
}

std::tuple<bool, std::string> ModeleJeu::JeuPrincipal::deplacerPiece(const Coordonnee& depart, Couleur couleurJoueur, const Coordonnee& arrivee) {
	if (echiquier_[depart.x][depart.y] == nullptr)
	{
		std::cout << "Aucune piece selectionnee" << std::endl;
		return{ false, "Aucune piece selectionnee" };
	}
	if (echiquier_[depart.x][depart.y]->getCouleur() != couleurJoueur)
	{
		std::cout << "La piece a deplacer ne correspond pas avec la couleur du joueur." << std::endl;
		return{ false, "La piece a deplacer ne correspond pas avec la couleur du joueur." };
	}

	try {
		Piece* piece = echiquier_[depart.x][depart.y].get();

		if (piece->estMouvementValide(depart, arrivee)) {

			if (!verifierContraintesEchiquier(depart, arrivee)) {
				return { false, "Deplacement bloque par d'autres pieces" };
			}

			{
				Temporaire pieceTemporaire(depart, arrivee, echiquier_);
				if (pieceTemporaire.verifierEchec(couleurJoueur)) {
					return { false, "Ce deplacement place le joueur " + couleurToString(couleurJoueur) + " en echec." };
				}
			}

			if (echiquier_[arrivee.x][arrivee.y] != nullptr) {
				std::cout << "Piece eliminee a la position (" << arrivee.x << "," << arrivee.y << ")" << std::endl;
				echiquier_[arrivee.x][arrivee.y] = nullptr;
			}

			echiquier_[arrivee.x][arrivee.y] = std::move(echiquier_[depart.x][depart.y]);
			echiquier_[depart.x][depart.y] = nullptr;
			std::cout << "Deplacement effectue de (" << depart.x << "," << depart.y << ") a (" << arrivee.x << "," << arrivee.y << ")" << std::endl;
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

ModeleJeu::Piece* ModeleJeu::JeuPrincipal::getPiece(const Coordonnee& position) {
	return echiquier_[position.x][position.y].get();
}

void ModeleJeu::JeuPrincipal::setCaseSelectionnee(const Coordonnee& position) {
	caseSelectione_ = position;
}

ModeleJeu::Coordonnee ModeleJeu::JeuPrincipal::getCaseSelectionnee() const {
	return caseSelectione_;
}

ModeleJeu::Temporaire::Temporaire(const Coordonnee& position, const Coordonnee& positionFutur, std::unique_ptr<Piece>(&echiquier)[ModeleJeu::tailleEchiquier][ModeleJeu::tailleEchiquier])
	: position_(position), positionFutur_(positionFutur), echiquier_(echiquier) {
	piece_ = std::move(echiquier_[position.x][position.y]);
	pieceCapturee_ = std::move(echiquier_[positionFutur.x][positionFutur.y]);
	echiquier_[positionFutur.x][positionFutur.y] = std::move(piece_);
}

ModeleJeu::Temporaire::~Temporaire()
{
	piece_ = std::move(echiquier_[positionFutur_.x][positionFutur_.y]);
	echiquier_[positionFutur_.x][positionFutur_.y] = std::move(pieceCapturee_);
	echiquier_[position_.x][position_.y] = std::move(piece_);
}


ModeleJeu::Piece* ModeleJeu::Temporaire::getTemporaire()
{
	return echiquier_[positionFutur_.x][positionFutur_.y].get();
}

bool ModeleJeu::Temporaire::verifierEchec(Couleur couleurJoueur)
{
	int positionRoiX = -1;
	int positionRoiY = -1;
	for (int i = 0; i < ModeleJeu::tailleEchiquier; i++)
	{
		for (int j = 0; j < ModeleJeu::tailleEchiquier; j++)
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
	for (int i = 0; i < tailleEchiquier; i++)
	{
		for (int j = 0; j < tailleEchiquier; j++)
		{
			Piece* pieceAdverse = echiquier_[i][j].get();
			if (pieceAdverse && pieceAdverse->estMouvementValide(Coordonnee(i, j), Coordonnee(positionRoiX, positionRoiY)) && pieceAdverse->getCouleur() != couleurJoueur)
				return true;
		}
	}
	return false;
}

ModeleJeu::JeuPrincipal::JeuPrincipal(Placement placement) {
	for (int i = 0; i < tailleEchiquier; ++i) {
		for (int j = 0; j < tailleEchiquier; ++j) {
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
		case Placement::LaBourdonaisMcDonnell1834: // La Bourdonnais vs. McDonnell, 1834
			std::cout << "\n========= La Bourdonnais vs. McDonnell, 1834 =========" << std::endl;
			ajouterPiece(Coordonnee(0, 1), Couleur::Blanc, TypePiece::Tour);
			ajouterPiece(Coordonnee(1, 0), Couleur::Noir, TypePiece::Cavalier);
			ajouterPiece(Coordonnee(3, 0), Couleur::Noir, TypePiece::Roi);
			ajouterPiece(Coordonnee(3, 2), Couleur::Blanc, TypePiece::Roi);
			break;

		case Placement::KarpovFtacnik1988: // Karpov vs. Ftacnik, 1988
			std::cout << "\n=========     Karpov vs. Ftacnik, 1988      =========" << std::endl;
			ajouterPiece(Coordonnee(1, 5), Couleur::Blanc, TypePiece::Tour);
			ajouterPiece(Coordonnee(2, 1), Couleur::Noir, TypePiece::Cavalier);
			ajouterPiece(Coordonnee(5, 7), Couleur::Noir, TypePiece::Roi);
			ajouterPiece(Coordonnee(2, 4), Couleur::Blanc, TypePiece::Roi);
			break;

		case Placement::PolgarKasparov1996: // J. Polgar vs. Kasparov, 1996
			std::cout << "\n=========   J. Polgar vs. Kasparov, 1996    =========" << std::endl;
			ajouterPiece(Coordonnee(0, 4), Couleur::Blanc, TypePiece::Tour);
			ajouterPiece(Coordonnee(7, 6), Couleur::Noir, TypePiece::Tour);
			ajouterPiece(Coordonnee(6, 4), Couleur::Noir, TypePiece::Cavalier);
			ajouterPiece(Coordonnee(5, 5), Couleur::Noir, TypePiece::Roi);
			ajouterPiece(Coordonnee(4, 7), Couleur::Blanc, TypePiece::Roi);
			break;

		case Placement::AlekhineCablanca1927: // Alekhine vs. Capablanca, 1927
			std::cout << "\n=========  Alekhine vs. Capablanca, 1927    =========" << std::endl;
			ajouterPiece(Coordonnee(6, 1), Couleur::Blanc, TypePiece::Tour);
			ajouterPiece(Coordonnee(1, 0), Couleur::Noir, TypePiece::Tour);
			ajouterPiece(Coordonnee(1, 3), Couleur::Noir, TypePiece::Cavalier);
			ajouterPiece(Coordonnee(2, 3), Couleur::Noir, TypePiece::Roi);
			ajouterPiece(Coordonnee(4, 3), Couleur::Blanc, TypePiece::Roi);
			break;
		}
	}
	catch (const CompteurRoisException& e) {
		std::cerr << "Erreur : " << e.what() << std::endl;
		for (int i = 0; i < tailleEchiquier; ++i) {
			for (int j = 0; j < tailleEchiquier; ++j) {
				if (echiquier_[i][j]) {
					echiquier_[i][j].reset();
				}
			}
		}
		throw;
	}
}
