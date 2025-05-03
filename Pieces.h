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

	class Piece {
	protected:
		int posX_;
		int posY_;
		std::string couleur_;

	public:
		Piece(int x, int y, std::string couleur) : posX_(x), posY_(y), couleur_(couleur) {}
		virtual ~Piece() = default;
		virtual bool estMouvementValide(int x, int y) = 0;
		std::string getCouleur();
		int getPositionX() const;
		int getPositionY() const;
		void setPosition(int x, int y);

	};

	class Roi : public Piece {
	public:
		Roi(int posXDebut, int posYDebut, std::string couleur);
		~Roi();
		bool estMouvementValide(int x, int y) override;
		int getCompteurRoi();

	private:
		static int compteurRoi_;
	};

	class CompteurRoisException : public std::logic_error {
	public:
		using logic_error::logic_error;
	};

	class Tour : public Piece {
	public:
		Tour(int posXDebut, int posYDebut, std::string couleur) : Piece(posXDebut, posYDebut, couleur) {}
		bool estMouvementValide(int x, int y) override;
	};

	class Cavalier : public Piece {
	public:
		Cavalier(int posXDebut, int posYDebut, std::string couleur) : Piece(posXDebut, posYDebut, couleur) {}
		bool estMouvementValide(int x, int y) override;
	};

	class JeuPrincipal {
	public:
		JeuPrincipal(int placement);

		bool verifierContraintesEchiquier(int posX, int posY, int nouvPosX, int nouvPosY);
		void ajouterPiece(int posX, int posY, std::string couleurDonne, std::string typePiece);
		std::tuple<bool, std::string> deplacerPiece(int posX, int posY, std::string couleurJoueur, int nouvPosX, int nouvPosY);
		Piece* getPiece(int x, int y);
		void setPieceSelectionnee(Piece* piece);
		Piece* getPieceSelectionnee() const;
		friend class Temporaire;

	private:
		Piece* pieceSelectionnee_ = nullptr;
		std::unique_ptr<Piece> echiquier_[tailleEchiquier][tailleEchiquier];
	};

	//Classe RAII permettant de bouger une piece temporairement
	class Temporaire {
	public:
		Temporaire(int positionX, int positionY, int nouvPositionX, int nouvPositionY, std::unique_ptr<Piece>(&echiquier)[ModeleJeu::tailleEchiquier][ModeleJeu::tailleEchiquier]);
		~Temporaire();
		Piece* getTemporaire();
		bool verifierEchec(std::string couleurJouer);

	private:
		int positionX_;
		int positionY_;
		int nouvPositionX_;
		int nouvPositionY_;
		std::unique_ptr<Piece>(&echiquier_)[tailleEchiquier][tailleEchiquier];
		std::unique_ptr<Piece> piece_;
		std::unique_ptr<Piece> pieceCapturee_;
	};
}