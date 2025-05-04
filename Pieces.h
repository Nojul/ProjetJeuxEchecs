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
		std::string couleur_;

	public:
		Piece(std::string couleur) : couleur_(std::move(couleur)) {}
		virtual ~Piece() = default;

		virtual bool estMouvementValide(const Coordonnee& depart, const Coordonnee& arrivee) = 0;
		std::string getCouleur() const;
	};

	class Roi : public Piece {
	public:
		Roi(std::string couleur);
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
		Tour(std::string couleur) : Piece(std::move(couleur)) {}
		bool estMouvementValide(const Coordonnee& depart, const Coordonnee& arrivee) override;
	};

	class Cavalier : public Piece {
	public:
		Cavalier(std::string couleur) : Piece(std::move(couleur)) {}
		bool estMouvementValide(const Coordonnee& depart, const Coordonnee& arrivee) override;
	};


	class JeuPrincipal {
	public:
		JeuPrincipal(int placement);

		bool verifierContraintesEchiquier(const Coordonnee& ancienne, const Coordonnee& nouvelle);
		void ajouterPiece(const Coordonnee& position, std::string couleurDonne, std::string typePiece);
		std::tuple<bool, std::string> deplacerPiece(const Coordonnee& depart, std::string couleurJoueur, const Coordonnee& arrivee);
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
		bool verifierEchec(std::string couleurJouer);

	private:
		Coordonnee position_;
		Coordonnee positionFutur_;
		std::unique_ptr<Piece>(&echiquier_)[tailleEchiquier][tailleEchiquier];
		std::unique_ptr<Piece> piece_;
		std::unique_ptr<Piece> pieceCapturee_;
	};
}