/**
* Definition des classes des composants du jeu d'echecs, pieces et echiquier
* \file   Pieces.h
* \author Latendresse et Julien
* \date   4 mai 2025
* Cree le 6 avril 2025
*/
#pragma once
#include "ui_ProjetJeuxEchecs.h"
#include <memory> 
#include <QtWidgets/QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class ProjetJeuxEchecsClass; };
QT_END_NAMESPACE

namespace ModeleJeu {

	const int tailleEchiquier = 8;

	enum class Couleur {
		Blanc,
		Noir
	};
	inline std::string couleurToString(Couleur couleur) {
		switch (couleur) {
		case Couleur::Blanc: return "Blanc";
		case Couleur::Noir: return "Noir";
		default: return "Inconnu";
		}
	}

	enum class TypePiece {
		Roi,
		Tour,
		Cavalier
	};

	inline std::string typeToString(TypePiece type) {
		switch (type) {
		case TypePiece::Roi: return "Roi";
		case TypePiece::Tour: return "Tour";
		case TypePiece::Cavalier: return "Cavalier";
		default: return "Inconnu";
		}
	}

	enum class Placement {
		LaBourdonaisMcDonnell1834,
		KarpovFtacnik1988,
		PolgarKasparov1996,
		AlekhineCablanca1927
	};

	struct Coordonnee {
		int x;
		int y;

		Coordonnee() : x(-1), y(-1) {}
		Coordonnee(int x, int y) : x(x), y(y) {}
		bool operator==(const Coordonnee& autre) const {
			return x == autre.x && y == autre.y;
		}
		bool estValide() const {
			return x >= 0 && x < 8 && y >= 0 && y < 8;
		}

	};

	class Piece {
	protected:
		Couleur couleur_;

	public:
		Piece(Couleur couleur) : couleur_(std::move(couleur)) {}
		virtual ~Piece() = default;

		virtual bool estMouvementValide(const Coordonnee& depart, const Coordonnee& arrivee) = 0;
		Couleur getCouleur() const;
	};

	class Roi : public Piece {
	public:
		Roi(Couleur couleur);
		~Roi();

		bool estMouvementValide(const Coordonnee& depart, const Coordonnee& arrivee) override;
		int getCompteurRoi() const;

	private:
		static int compteurRoi_;
	};

	class CompteurRoisException : public std::logic_error {
	public:
		using std::logic_error::logic_error;
	};

	class Tour : public Piece {
	public:
		Tour(Couleur couleur) : Piece(std::move(couleur)) {}
		bool estMouvementValide(const Coordonnee& depart, const Coordonnee& arrivee) override;
	};

	class Cavalier : public Piece {
	public:
		Cavalier(Couleur couleur) : Piece(std::move(couleur)) {}
		bool estMouvementValide(const Coordonnee& depart, const Coordonnee& arrivee) override;
	};


	class JeuPrincipal {
	public:
		JeuPrincipal(Placement placement);
		void miseEnPlacement(Placement placement);
		bool verifierContraintesEchiquier(const Coordonnee& ancienne, const Coordonnee& nouvelle);
		void ajouterPiece(const Coordonnee& position, Couleur couleur, TypePiece type);
		std::tuple<bool, std::string> deplacerPiece(const Coordonnee& depart, Couleur couleurJoueur, const Coordonnee& arrivee);
		Piece* getPiece(const Coordonnee& position);
		void setCaseSelectionnee(const Coordonnee& position);
		Coordonnee getCaseSelectionnee() const;
		friend class Temporaire;

	private:
		Coordonnee caseSelectione_;
		std::unique_ptr<Piece> echiquier_[tailleEchiquier][tailleEchiquier];
	};

	//Classe RAII permettant de bouger une piece temporairement
	class Temporaire {
	public:
		Temporaire(const Coordonnee& position, const Coordonnee& positionFutur, std::unique_ptr<Piece>(&echiquier)[ModeleJeu::tailleEchiquier][ModeleJeu::tailleEchiquier]);
		~Temporaire();
		Piece* getTemporaire();
		bool verifierEchec(Couleur couleurJouer);

	private:
		Coordonnee position_;
		Coordonnee positionFutur_;
		std::unique_ptr<Piece>(&echiquier_)[tailleEchiquier][tailleEchiquier];
		std::unique_ptr<Piece> piece_;
		std::unique_ptr<Piece> pieceCapturee_;
	};
}