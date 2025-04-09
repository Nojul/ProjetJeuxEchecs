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
		virtual bool verifierDeplacment(int x, int y) = 0;
		virtual void deplacer(int x, int y) = 0;
	};

	class Roi : public Piece {
	public:
		Roi(int posXDebut, int posYDebut, std::string couleur);
		~Roi();

		void deplacer(int x, int y) override;
		bool verifierDeplacment(int x, int y) override;
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
		//~Tour();

		void deplacer(int x, int y) override;
		bool verifierDeplacment(int x, int y) override;
	};

	class Cavalier : public Piece {
	public:
		Cavalier(int posXDebut, int posYDebut, std::string couleur) : Piece(posXDebut, posYDebut, couleur) {}
		//~Cavalier();

		void deplacer(int x, int y) override;
		bool verifierDeplacment(int x, int y) override;
	};

	class JeuPrincipal {
	public:
		JeuPrincipal(int placement);
		~JeuPrincipal();

	private:
		std::unique_ptr<Piece> echiquier[8][8];
	};

}


