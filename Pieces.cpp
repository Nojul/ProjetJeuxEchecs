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

int ModeleJeu::JeuPrincipal::compteurRoi_ = 0;

ModeleJeu::Roi::~Roi() { JeuPrincipal::compteurRoi_--; }

bool ModeleJeu::Roi::estMouvementValide(const Coordonnee& depart, const Coordonnee& arrivee) const {
	int deltaX = abs(depart.x - arrivee.x);
	int deltaY = abs(depart.y - arrivee.y);
	return deltaX <= 1 && deltaY <= 1 && (deltaX != 0 || deltaY != 0);
}

bool ModeleJeu::Tour::estMouvementValide(const Coordonnee& depart, const Coordonnee& arrivee) const {
	return (depart.x == arrivee.x || depart.y == arrivee.y) &&
		(depart.x != arrivee.x || depart.y != arrivee.y);
}

bool ModeleJeu::Cavalier::estMouvementValide(const Coordonnee& depart, const Coordonnee& arrivee) const {
	int deltaX = abs(depart.x - arrivee.x);
	int deltaY = abs(depart.y - arrivee.y);
	return (deltaX == 2 && deltaY == 1) || (deltaX == 1 && deltaY == 2);
}

void ModeleJeu::JeuPrincipal::ajouterPiece(const Coordonnee& position, Couleur couleur, TypePiece type) {
	if (position.x < 0 || position.x >= tailleEchiquier || position.y < 0 || position.y >= tailleEchiquier) {
		throw std::out_of_range("Position hors des limites du plateau");
	}
	std::unique_ptr<Piece> nouvellePiece;
	switch (type) {
	case TypePiece::Roi:
		if (compteurRoi_ >= 2) {
			throw CompteurRoisException("Plus de deux instances de rois");
		}
		compteurRoi_++;
		try {
			nouvellePiece = std::make_unique<Roi>();
		}
		catch (const CompteurRoisException& e) {
			std::cerr << "Erreur roi : " << e.what() << std::endl;
			throw;
		}
		break;

	case TypePiece::Tour:
		nouvellePiece = std::make_unique<Tour>();
		break;

	case TypePiece::Cavalier:
		nouvellePiece = std::make_unique<Cavalier>();
		break;
	}

	echiquier_[position.x][position.y] = CaseEchiquier(std::move(nouvellePiece), couleur);

	std::cout << "  Ajout de " << typeToString(type) << " " << couleurToString(couleur)
		<< " sur (" << position.x << ", " << position.y << ")" << std::endl;
}

bool ModeleJeu::JeuPrincipal::verifierContraintesEchiquier(const Coordonnee& ancienne, const Coordonnee& nouvelle) const {
	const auto& caseDepart = echiquier_[ancienne.x][ancienne.y];
	if (!caseDepart.piece) return false;

	const auto& caseArrivee = echiquier_[nouvelle.x][nouvelle.y];
	if (caseArrivee.piece && caseArrivee.couleur == caseDepart.couleur) {
		return false;
	}

	if (dynamic_cast<Tour*>(caseDepart.piece.get())) {
		int startX = std::min(ancienne.x, nouvelle.x);
		int endX = std::max(ancienne.x, nouvelle.x);
		int startY = std::min(ancienne.y, nouvelle.y);
		int endY = std::max(ancienne.y, nouvelle.y);

		if (ancienne.x == nouvelle.x) {
			for (int i = startY + 1; i < endY; ++i) {
				if (echiquier_[ancienne.x][i].piece != nullptr) {
					return false;
				}
			}
		}
		else if (ancienne.y == nouvelle.y) {
			for (int i = startX + 1; i < endX; ++i) {
				if (echiquier_[i][ancienne.y].piece != nullptr) {
					return false;
				}
			}
		}
	}
	return true;
}
bool ModeleJeu::JeuPrincipal::verifierEchec(Couleur couleurJoueur) {
	int positionRoiX = -1;
	int positionRoiY = -1;
	for (int i = 0; i < ModeleJeu::tailleEchiquier; i++) {
		for (int j = 0; j < ModeleJeu::tailleEchiquier; j++) {
			CaseEchiquier& caseActuelle = echiquier_[i][j];
			if (caseActuelle.piece && dynamic_cast<ModeleJeu::Roi*>(caseActuelle.piece.get()) && caseActuelle.couleur == couleurJoueur) {
				positionRoiX = i;
				positionRoiY = j;
			}
		}
	}
	if (positionRoiX == -1 and positionRoiY == -1) {
		std::cout << "Le roi n'a pas ete trouve." << std::endl;
		return false;
	}

	for (int i = 0; i < tailleEchiquier; i++) {
		for (int j = 0; j < tailleEchiquier; j++) {
			CaseEchiquier& caseActuelle = echiquier_[i][j];
			if (caseActuelle.piece && caseActuelle.couleur != couleurJoueur) {
				Coordonnee depart(i, j);
				Coordonnee destination(positionRoiX, positionRoiY);

				if (caseActuelle.piece.get()->estMouvementValide(depart, destination) && verifierContraintesEchiquier(depart, destination)) {
					return true;
				}
			}
		}
	}
	return false;
}

std::tuple<bool, std::string> ModeleJeu::JeuPrincipal::deplacerPiece(const Coordonnee& depart, Couleur couleurJoueur, const Coordonnee& arrivee) {
	const auto& caseDepart = echiquier_[depart.x][depart.y];
	if (!caseDepart.piece) {
		std::cout << "Aucune piece selectionnee" << std::endl;
		return { false, "Aucune piece selectionnee" };
	}
	if (caseDepart.couleur != couleurJoueur) {
		std::cout << "La piece a deplacer ne correspond pas avec la couleur du joueur." << std::endl;
		return { false, "La piece a deplacer ne correspond pas avec la couleur du joueur." };
	}

	try {
		if (caseDepart.piece->estMouvementValide(depart, arrivee)) {
			if (!verifierContraintesEchiquier(depart, arrivee)) {
				return { false, "Deplacement bloque par d'autres pieces" };
			}

			{
				Temporaire pieceTemporaire(depart, arrivee, echiquier_, this);
				if (verifierEchec(couleurJoueur)) {
					return { false, "Ce deplacement place le joueur " + couleurToString(couleurJoueur) + " en echec." };
				}
			}

			if (echiquier_[arrivee.x][arrivee.y].piece) {
				std::cout << "Piece eliminee a la position (" << arrivee.x << "," << arrivee.y << ")" << std::endl;
			}
			echiquier_[arrivee.x][arrivee.y] = CaseEchiquier(std::move(echiquier_[depart.x][depart.y].piece), echiquier_[depart.x][depart.y].couleur);
			echiquier_[depart.x][depart.y] = CaseEchiquier();

			std::cout << "Deplacement effectue de (" << depart.x << "," << depart.y
				<< ") a (" << arrivee.x << "," << arrivee.y << ")" << std::endl;

			if (verifierEchec(couleurAdverse(couleurJoueur))) {
				std::cout << "Ce deplacement a place le joueur " + couleurToString(couleurAdverse(couleurJoueur)) + " en echec." << std::endl;
				return { true, "Ce deplacement a place le joueur " + couleurToString(couleurAdverse(couleurJoueur)) + " en echec." };
			}
			else {
				return { true, "" };
			}
		}
		else {
			return { false, "Deplacement invalide pour cette piece" };
		}
	}
	catch (const std::exception& e) {
		return { false, "Erreur lors du deplacement: " + std::string(e.what()) };
	}
}

ModeleJeu::Piece* ModeleJeu::JeuPrincipal::getPiece(const Coordonnee& position) const {
	return echiquier_[position.x][position.y].piece.get();
}

int ModeleJeu::JeuPrincipal::getCompteurRoi() const {
	return compteurRoi_;
}

ModeleJeu::Couleur ModeleJeu::JeuPrincipal::getCouleurPiece(const Coordonnee& position) const {
	return echiquier_[position.x][position.y].couleur;
}

void ModeleJeu::JeuPrincipal::setCaseSelectionnee(const Coordonnee& position) {
	caseSelectione_ = position;
}

ModeleJeu::Coordonnee ModeleJeu::JeuPrincipal::getCaseSelectionnee() const {
	return caseSelectione_;
}

ModeleJeu::Temporaire::Temporaire(const Coordonnee& position, const Coordonnee& positionFutur, CaseEchiquier(&echiquier)[tailleEchiquier][tailleEchiquier], JeuPrincipal* jeu)
	: position_(position), positionFutur_(positionFutur), echiquier_(echiquier), jeu_(jeu) {
	caseTemporaire_ = CaseEchiquier(std::move(echiquier_[position.x][position.y].piece), echiquier_[position.x][position.y].couleur);
	caseCapturee_ = CaseEchiquier(std::move(echiquier_[positionFutur.x][positionFutur.y].piece), echiquier_[positionFutur.x][positionFutur.y].couleur);
	echiquier_[positionFutur.x][positionFutur.y] = CaseEchiquier(std::move(caseTemporaire_.piece), caseTemporaire_.couleur);
}

ModeleJeu::Temporaire::~Temporaire() {
	caseTemporaire_ = CaseEchiquier(std::move(echiquier_[positionFutur_.x][positionFutur_.y].piece), echiquier_[positionFutur_.x][positionFutur_.y].couleur);
	echiquier_[positionFutur_.x][positionFutur_.y] = CaseEchiquier(std::move(caseCapturee_.piece), caseCapturee_.couleur);
	echiquier_[position_.x][position_.y] = CaseEchiquier(std::move(caseTemporaire_.piece), caseTemporaire_.couleur);
}

ModeleJeu::Piece* ModeleJeu::Temporaire::getTemporaire()
{
	return echiquier_[positionFutur_.x][positionFutur_.y].piece.get();
}

ModeleJeu::JeuPrincipal::JeuPrincipal(Placement placement) {
	miseEnPlacement(placement);
}

void ModeleJeu::JeuPrincipal::miseEnPlacement(Placement placement) {
	caseSelectione_ = Coordonnee(-1, -1);
	for (int i = 0; i < tailleEchiquier; ++i) {
		for (int j = 0; j < tailleEchiquier; ++j) {
			echiquier_[i][j].piece.reset();
		}
	}

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
				if (echiquier_[i][j].piece) {
					echiquier_[i][j].piece.reset();
				}
			}
		}
		throw;
	}


}