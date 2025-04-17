#pragma once
#include "ui_ProjetJeuxEchecs.h"
#include "Pieces.h"
#include <qpushbutton.h>
#include <memory> 
#include <QtWidgets/QMainWindow>
#include <QLabel>

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

		void clic(int x, int y);
		void miseAJour();

	private:
		Ui::ProjetJeuxEchecsClass* ui;
		QPushButton* buttons[8][8];
		ModeleJeu::JeuPrincipal* jeu = nullptr;
		std::string joueur = "Blanc";
		QLabel* messageErreur_ = nullptr;
	};
}

