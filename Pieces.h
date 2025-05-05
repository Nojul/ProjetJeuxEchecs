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
#include <stdexcept>
#include <string>
#include <tuple>

QT_BEGIN_NAMESPACE
namespace Ui { class ProjetJeuxEchecsClass; };
QT_END_NAMESPACE

namespace ModeleJeu {

	const int tailleEchiquier = 8;

	enum class Couleur {
		Blanc,
		Noir
	};

	inline Couleur couleurAdverse(Couleur couleurActuelle)
	{
		switch (couleurActuelle) {
		case Couleur::Blanc: return Couleur::Noir;
		case Couleur::Noir: return Couleur::Blanc;
		default:
			throw std::invalid_argument("Couleur inconnue dans couleurAdverse");
		}
	}


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
	public:
		Piece() = default;
		virtual ~Piece() = default;
		virtual bool estMouvementValide(const Coordonnee& depart, const Coordonnee& arrivee) const = 0;
	};

	struct CaseEchiquier {
		std::unique_ptr<Piece> piece;
		Couleur couleur;

		CaseEchiquier() : piece(nullptr), couleur(Couleur::Blanc) {}
		CaseEchiquier(std::unique_ptr<Piece> p, Couleur c)
			: piece(std::move(p)), couleur(c) {
		}
		CaseEchiquier(const CaseEchiquier&) = delete;
		CaseEchiquier& operator=(const CaseEchiquier&) = delete;
		CaseEchiquier(CaseEchiquier&&) = default;
		CaseEchiquier& operator=(CaseEchiquier&&) = default;
	};

	class Roi : public Piece {
	public:
		Roi() = default;
		~Roi();
		bool estMouvementValide(const Coordonnee& depart, const Coordonnee& arrivee)const  override;
	};

	class Tour : public Piece {
	public:
		Tour() = default;
		bool estMouvementValide(const Coordonnee& depart, const Coordonnee& arrivee) const  override;
	};

	class Cavalier : public Piece {
	public:
		Cavalier() = default;
		bool estMouvementValide(const Coordonnee& depart, const Coordonnee& arrivee) const override;
	};

	class CompteurRoisException : public std::logic_error {
	public:
		using std::logic_error::logic_error;
	};

	class JeuPrincipal {
	public:
		JeuPrincipal(Placement placement);
		void miseEnPlacement(Placement placement);
		bool verifierContraintesEchiquier(const Coordonnee& ancienne, const Coordonnee& nouvelle) const;
		void ajouterPiece(const Coordonnee& position, Couleur couleur, TypePiece type);
		std::tuple<bool, std::string> deplacerPiece(const Coordonnee& depart, Couleur couleurJoueur, const Coordonnee& arrivee);
		Piece* getPiece(const Coordonnee& position) const;
		Couleur getCouleurPiece(const Coordonnee& position) const;
		void setCaseSelectionnee(const Coordonnee& position);
		Coordonnee getCaseSelectionnee() const;
		int getCompteurRoi() const;
		friend class Temporaire;
		friend class Roi;
		bool verifierEchec(Couleur couleurJoueur);

	private:
		Coordonnee caseSelectione_;
		static int compteurRoi_;
		CaseEchiquier echiquier_[tailleEchiquier][tailleEchiquier];
	};

	class Temporaire {
	public:
		Temporaire(const Coordonnee& position, const Coordonnee& positionFutur, CaseEchiquier(&echiquier)[tailleEchiquier][tailleEchiquier], JeuPrincipal* jeu);
		~Temporaire();
		Piece* getTemporaire();

	private:
		const Coordonnee position_;
		const Coordonnee positionFutur_;
		CaseEchiquier(&echiquier_)[tailleEchiquier][tailleEchiquier];
		CaseEchiquier caseTemporaire_;
		CaseEchiquier caseCapturee_;
		JeuPrincipal* jeu_;
	};
}