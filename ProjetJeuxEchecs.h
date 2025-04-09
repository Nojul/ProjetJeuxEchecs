#pragma once
#include "ui_ProjetJeuxEchecs.h"
#include <memory> 
#include <QtWidgets/QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class ProjetJeuxEchecsClass; };
QT_END_NAMESPACE

namespace interfaceGraphique {
	class ProjetJeuxEchecs : public QMainWindow
	{
		Q_OBJECT

	public:
		ProjetJeuxEchecs(QWidget* parent = nullptr);
		~ProjetJeuxEchecs();

	private:
		Ui::ProjetJeuxEchecsClass* ui;
	};
}

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

		void deplacerPiece(int posX, int posY, std::string couleurJoueur, int nouvPosX, int nouvPosY);

	private:
		std::unique_ptr<Piece> echiquier[8][8];
	};

}


