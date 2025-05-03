/**
* Implementation de la classe ProjetJeuxEchecs qui est le moddle de l'interface graphique
* \file   ProjetJeuxEchecs.cpp
* \author Latendresse et Julien
* \date   4 mai 2025
* Cree le 6 avril 2025
*/
#include "ProjetJeuxEchecs.h"
#include <iostream>
#include <QLabel>
#include <qpushbutton.h>
#include <QString>
#include <QTimer>
#include <QVBoxLayout> 

interfaceGraphique::ProjetJeuxEchecs::ProjetJeuxEchecs(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::ProjetJeuxEchecsClass())
	, jeu_(nullptr)
{
	ui->setupUi(this);
	const int windowSize = 800;
	this->setFixedSize(windowSize, windowSize);

	QGridLayout* gridLayout = new QGridLayout();
	gridLayout->setSpacing(0);

	QWidget* centralWidget = new QWidget(this);
	setCentralWidget(centralWidget);

	messageErreur_ = new QLabel(this);
	messageErreur_->setStyleSheet("color: red; font-weight: bold;");
	messageErreur_->setAlignment(Qt::AlignBottom | Qt::AlignRight);
	messageErreur_->setText("");

	for (int i = 0; i < ModeleJeu::tailleEchiquier; ++i) {		//lignes
		for (int j = 0; j < ModeleJeu::tailleEchiquier; ++j) {	//colonnes
			this->buttons[i][j] = new QPushButton(this);
			this->buttons[i][j]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);


			QFont taille = buttons[i][j]->font();
			taille.setPointSize(40);
			buttons[i][j]->setFont(taille);

			if ((i + j) % 2 == 0) {
				this->buttons[i][j]->setStyleSheet("background-color: #f0d9b5; border: none;");

			}
			else {
				this->buttons[i][j]->setStyleSheet("background-color: #b58863; border: none;");

			}


			gridLayout->addWidget(this->buttons[i][j], i, j);
			connect(this->buttons[i][j], &QPushButton::clicked, this, [this, i, j]() {
				clic(i, j);
				qDebug() << "Bouton clique :" << i << "," << j;
				});
		}
	}

	try {
		jeu_ = std::make_unique<ModeleJeu::JeuPrincipal>(1);
	}
	catch (const ModeleJeu::CompteurRoisException& e) {
		std::cerr << "Error creating game: " << e.what() << std::endl;
		messageErreur_->setText(QString::fromStdString(e.what()));
		messageErreur_->show();
	}
	catch (const std::exception& e) {
		std::cerr << "Caught exception: " << e.what() << std::endl;
		messageErreur_->setText(QString::fromStdString(e.what()));
		messageErreur_->show();
	}

	if (jeu_ != nullptr) {
		miseAJour();
	}

	QVBoxLayout* layoutPrincipal = new QVBoxLayout();
	layoutPrincipal->addLayout(gridLayout);
	layoutPrincipal->addWidget(messageErreur_, 0, Qt::AlignRight);
	centralWidget->setLayout(layoutPrincipal);
}

interfaceGraphique::ProjetJeuxEchecs::~ProjetJeuxEchecs()
{
	delete ui;
}

int xSelectionne = -1;
int ySelectionne = -1;
void interfaceGraphique::ProjetJeuxEchecs::clic(int x, int y)
{
	if (xSelectionne == -1)
	{
		xSelectionne = x;
		ySelectionne = y;
		qDebug() << "premier clic";
	}
	else
	{
		std::tuple<bool, std::string> deplacement = jeu_->deplacerPiece(xSelectionne, ySelectionne, joueur, x, y);
		qDebug() << "deuxieme clic";
		miseAJour();
		if (get<0>(deplacement))
		{
			if (joueur == "Blanc")
			{
				joueur = "Noir";
			}
			else
			{
				joueur = "Blanc";
			}
		}
		else
		{
			messageErreur_->setText(QString::fromStdString(get<1>(deplacement)));
			messageErreur_->show();
			QTimer::singleShot(2000, messageErreur_, &QLabel::hide);
		}
		xSelectionne = -1;
		ySelectionne = -1;
	}
}
void interfaceGraphique::ProjetJeuxEchecs::miseAJour()
{
	for (int i = 0; i < ModeleJeu::tailleEchiquier; ++i)
	{
		for (int j = 0; j < ModeleJeu::tailleEchiquier; ++j)
		{
			ModeleJeu::Piece* piece = jeu_->getPiece(i, j);
			QString imagePiece = "";
			QString styleBase;

			if ((i + j) % 2 == 0) {
				styleBase = "background-color: #f0d9b5; border: none;";
			}
			else {
				styleBase = "background-color: #b58863; border: none;";
			}

			if (piece)
			{
				QString colorStyle;
				if (piece->getCouleur() == "Blanc")
				{
					colorStyle = "white;";
				}
				else
				{
					colorStyle = "black;";
				}

				styleBase += " color: " + colorStyle;

				if (dynamic_cast<ModeleJeu::Roi*>(piece))
				{
					if (piece->getCouleur() == "Blanc")
					{
						imagePiece = "♔";
					}
					else
					{
						imagePiece = "♚";
					}
				}
				else if (dynamic_cast<ModeleJeu::Tour*>(piece))
				{
					if (piece->getCouleur() == "Blanc")
					{
						imagePiece = "♖";
					}
					else
					{
						imagePiece = "♜";
					}
				}
				else if (dynamic_cast<ModeleJeu::Cavalier*>(piece))
				{
					if (piece->getCouleur() == "Blanc")
					{
						imagePiece = "♘";
					}
					else
					{
						imagePiece = "♞";
					}
				}
				buttons[i][j]->setText(imagePiece);
				buttons[i][j]->setStyleSheet(styleBase);
			}
			else
			{
				buttons[i][j]->setText("");
				buttons[i][j]->setStyleSheet(styleBase);
			}
		}
	}
}

