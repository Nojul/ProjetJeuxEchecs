#pragma once
#include "ui_ProjetJeuxEchecs.h"
#include <memory> 
#include <QtWidgets/QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class ProjetJeuxEchecsClass; };
QT_END_NAMESPACE

namespace ModeleJeu {
	class Piece {
	protected:
		int posX;
		int posY;
		std::string couleur_;

	public:
		Piece(int x, int y, std::string couleur) : posX(x), posY(y), couleur_(couleur) {}
		virtual ~Piece() = default;
		virtual bool verifierDeplacement(int x, int y, std::unique_ptr<Piece> echiquier[8][8]) = 0;
		virtual std::string getCouleur();
		void deplacer(int x, int y);

	};

	class Roi : public Piece {
	public:
		Roi(int posXDebut, int posYDebut, std::string couleur);
		~Roi();

		bool verifierDeplacement(int x, int y, std::unique_ptr<Piece> echiquier[8][8]) override;
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

		bool verifierDeplacement(int x, int y, std::unique_ptr<Piece> echiquier[8][8]) override;

	};

	class Cavalier : public Piece {
	public:
		Cavalier(int posXDebut, int posYDebut, std::string couleur) : Piece(posXDebut, posYDebut, couleur) {}

		bool verifierDeplacement(int x, int y, std::unique_ptr<Piece> echiquier[8][8]) override;
	};

	class JeuPrincipal {
	public:
		JeuPrincipal(int placement);

		void ajouterPiece(int posX, int posY, std::string couleurDonne, std::string typePiece);
		void deplacerPiece(int posX, int posY, std::string couleurJoueur, int nouvPosX, int nouvPosY);

	private:
		std::unique_ptr<Piece> echiquier[8][8];
	};

	//Classe RAII permettant de bouger une piece temporairement
	class Temporaire {
	public:
		Temporaire(int positionX, int positionY, int nouvPositionX, int nouvPositionY, std::unique_ptr<Piece>(&echiquier)[8][8]);

		~Temporaire();

	private:
		int positionX_;
		int positionY_;
		int nouvPositionX_;
		int nouvPositionY_;
		std::unique_ptr<Piece> (&echiquier_)[8][8];
		std::unique_ptr<Piece> piece_;
		std::unique_ptr<Piece> pieceCapturee_;
	};
}
