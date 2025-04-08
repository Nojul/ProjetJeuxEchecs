#pragma once
#include "ui_ProjetJeuxEchecs.h"
#include <iostream>
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
	public:
		virtual ~Piece() = default;
		virtual bool verifierDeplacment(int x, int y) = 0;
		virtual void deplacer(int x, int y) = 0;
	};

	class Roi : public Piece {
	public:
		Roi() = default;
		Roi(int posXDebut, int posYDebut);
		~Roi();

		void deplacer(int x, int y) override;
		bool verifierDeplacment(int x, int y) override;
		int getCompteurRoi();

	private:
		static int compteurRoi_;
		int posX;
		int posY;
	};

	class CompteurRoisException : public std::logic_error {
	public:
		using logic_error::logic_error;
	};

	class Tour : public Piece {
	public:
		Tour() = default;
		Tour(int posXDebut, int posYDebut) : posX(posXDebut), posY(posYDebut) {}
		//~Tour();

		void deplacer(int x, int y) override;
		bool verifierDeplacment(int x, int y) override;

	private:
		int posX;
		int posY;
	};

	class Cavalier : public Piece {
	public:
		Cavalier() = default;
		Cavalier(int posXDebut, int posYDebut) : posX(posXDebut), posY(posYDebut) {}
		//~Cavalier();

		void deplacer(int x, int y) override;
		bool verifierDeplacment(int x, int y) override;

	private:
		int posX;
		int posY;
	};

	class ApplicationPrincipale {
	public:
		ApplicationPrincipale() = default;
		ApplicationPrincipale(std::string placement);

		void changerPlacement(std::string placement);

	private:
		Piece* echiquier[8][8] = {};
	};

}


