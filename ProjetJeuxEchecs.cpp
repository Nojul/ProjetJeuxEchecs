/**
* Implementation de la classe ProjetJeuxEchecs qui est le moddle de l'interface graphique
* \file   ProjetJeuxEchecs.cpp
* \author Latendresse et Julien
* \date   4 mai 2025
* Cree le 6 avril 2025
*/

#include "ProjetJeuxEchecs.h"
#include <iostream>
#include <QComboBox>
#include <QLabel>
#include <qpushbutton.h>
#include <QString>
#include <QTimer>
#include <QVBoxLayout>

interfaceGraphique::ProjetJeuxEchecs::ProjetJeuxEchecs(QWidget* parent) : QMainWindow(parent), ui(new Ui::ProjetJeuxEchecsClass()), jeu_(nullptr) {
	ui->setupUi(this);
	const int tailleFenetre = 800;
	const int taillePanneau = 200;
	const int largeurErreur = 200;
	const int hauteurErreur = 50;

	this->setFixedSize(tailleFenetre + taillePanneau, tailleFenetre);

	QWidget* centralWidget = new QWidget(this);
	setCentralWidget(centralWidget);

	QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);

	QGridLayout* gridLayout = new QGridLayout();
	gridLayout->setSpacing(0);

	QVBoxLayout* rightLayout = new QVBoxLayout();

	choixPlacement_ = new QComboBox();
	choixPlacement_->addItem("La Bourdonnais vs. McDonnell, 1834", static_cast<int>(ModeleJeu::Placement::LaBourdonaisMcDonnell1834));
	choixPlacement_->addItem("Karpov vs. Ftacnik, 1988", static_cast<int>(ModeleJeu::Placement::KarpovFtacnik1988));
	choixPlacement_->addItem("J. Polgar vs. Kasparov, 1996", static_cast<int>(ModeleJeu::Placement::PolgarKasparov1996));
	choixPlacement_->addItem("Alekhine vs. Capablanca, 1927", static_cast<int>(ModeleJeu::Placement::AlekhineCablanca1927));
	rightLayout->addWidget(new QLabel("Choix de placement:"));
	rightLayout->addWidget(choixPlacement_);
	rightLayout->addSpacing(10);

	//Pour reinitialiser la partie
	QPushButton* reinitialiser = new QPushButton("Réinitialiser la partie");
	rightLayout->addWidget(reinitialiser);
	connect(reinitialiser, &QPushButton::clicked, this, &ProjetJeuxEchecs::reinitialiserPartie);

	//Pour savoir le tour de joueur
	tourLabel_ = new QLabel(this);
	tourLabel_->setFixedSize(largeurErreur, hauteurErreur);
	tourLabel_->setAlignment(Qt::AlignCenter);
	tourLabel_->setWordWrap(true);

	QHBoxLayout* tourContainer = new QHBoxLayout();
	rightLayout->addWidget(new QLabel("Tour:"));
	tourContainer->addWidget(tourLabel_);
	rightLayout->addLayout(tourContainer);
	rightLayout->addStretch();

	//Pour le message d'erreur
	messageErreur_ = new QLabel(this);
	messageErreur_->setFixedSize(largeurErreur, hauteurErreur);
	messageErreur_->setAlignment(Qt::AlignCenter);
	messageErreur_->setWordWrap(true);
	messageErreur_->setStyleSheet(
		"QLabel { "
		"color: white; "
		"background-color: #d9534f; "
		"padding: 6px; "
		"border-radius: 8px; "
		"font-weight: bold; "
		"font-size: 12px;"
		"}"
	);
	messageErreur_->setVisible(false);
	QHBoxLayout* errorContainer = new QHBoxLayout();
	errorContainer->addWidget(messageErreur_);
	rightLayout->addLayout(errorContainer);

	//Pour le message de succes
	messageSucces_ = new QLabel(this);
	messageSucces_->setFixedSize(largeurErreur, hauteurErreur);
	messageSucces_->setAlignment(Qt::AlignCenter);
	messageSucces_->setWordWrap(true);
	messageSucces_->setStyleSheet(
		"QLabel { "
		"color: white; "
		"background-color: #5cb85c; "
		"padding: 6px; "
		"border-radius: 8px; "
		"font-weight: bold; "
		"font-size: 12px;"
		"}"
	);
	messageSucces_->setVisible(false);
	QHBoxLayout* successContainer = new QHBoxLayout();
	successContainer->addWidget(messageSucces_);
	rightLayout->addLayout(successContainer);


	for (int i = 0; i < ModeleJeu::tailleEchiquier; ++i) {		//lignes
		for (int j = 0; j < ModeleJeu::tailleEchiquier; ++j) {	//colonnes
			this->boutons[i][j] = new QPushButton(this);
			this->boutons[i][j]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

			QFont taille = boutons[i][j]->font();
			taille.setPointSize(40);
			boutons[i][j]->setFont(taille);

			if ((i + j) % 2 == 0) {
				this->boutons[i][j]->setStyleSheet("background-color: #f0d9b5; border: none;");

			}
			else {
				this->boutons[i][j]->setStyleSheet("background-color: #b58863; border: none;");

			}
			gridLayout->addWidget(this->boutons[i][j], i, j);
		}
	}
	mainLayout->addLayout(gridLayout);
	mainLayout->addLayout(rightLayout);

	try {
		jeu_ = std::make_unique<ModeleJeu::JeuPrincipal>(static_cast<ModeleJeu::Placement>(choixPlacement_->currentData().toInt()));
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
	emit surTourChange(ModeleJeu::Couleur::Blanc);
	faireConnections();
}

void interfaceGraphique::ProjetJeuxEchecs::faireConnections() {
	connect(choixPlacement_, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &ProjetJeuxEchecs::surChangementPlacement);
	connect(this, &ProjetJeuxEchecs::tourChange, this, &ProjetJeuxEchecs::surTourChange);
	connect(this, &ProjetJeuxEchecs::pieceSelectionnee, this, &ProjetJeuxEchecs::surPieceSelectionnee);

	for (int i = 0; i < ModeleJeu::tailleEchiquier; ++i) {
		for (int j = 0; j < ModeleJeu::tailleEchiquier; ++j) {
			connect(boutons[i][j], &QPushButton::clicked, this, [this, i, j]() {
				ModeleJeu::Coordonnee coordonnee(i, j);
				emit pieceSelectionnee(coordonnee);
				surPieceClic(coordonnee);
				});
		}
	}

}

void interfaceGraphique::ProjetJeuxEchecs::surPieceSelectionnee(const ModeleJeu::Coordonnee& coordonnee) const {
	for (int i = 0; i < ModeleJeu::tailleEchiquier; ++i) {
		for (int j = 0; j < ModeleJeu::tailleEchiquier; ++j) {
			QString styleBase;

			if ((i + j) % 2 == 0) {
				styleBase = "background-color: #f0d9b5;";
			}
			else {
				styleBase = "background-color: #b58863;";
			}

			if (ModeleJeu::Coordonnee(i, j) == coordonnee) {
				styleBase += " border: 4px solid green;";
			}
			else {
				styleBase += " border: none;";
			}

			boutons[i][j]->setStyleSheet(styleBase);
		}
	}
}

void interfaceGraphique::ProjetJeuxEchecs::reinitialiserPartie()
{
	int placementChoisi = choixPlacement_->currentData().toInt();
	jeu_->miseEnPlacement(static_cast<ModeleJeu::Placement>(placementChoisi));

	ModeleJeu::Coordonnee aucunePieceSelectionnee(-1, -1);
	jeu_->setCaseSelectionnee(aucunePieceSelectionnee);
	emit pieceSelectionnee(aucunePieceSelectionnee);

	joueur = ModeleJeu::Couleur::Blanc;
	emit tourChange(ModeleJeu::Couleur::Blanc);

	miseAJour();

	for (int i = 0; i < ModeleJeu::tailleEchiquier; ++i) {
		for (int j = 0; j < ModeleJeu::tailleEchiquier; ++j) {
			boutons[i][j]->setEnabled(true);
		}
	}
}

void interfaceGraphique::ProjetJeuxEchecs::finPartie(const QString& message)
{
	messageSucces_->setText(message);
	messageSucces_->show();

	for (int i = 0; i < ModeleJeu::tailleEchiquier; ++i) {
		for (int j = 0; j < ModeleJeu::tailleEchiquier; ++j) {
			boutons[i][j]->setEnabled(false);
		}
	}
}

void interfaceGraphique::ProjetJeuxEchecs::surDeplacementValide(bool success, const QString& message)
{
	if (!success) {
		messageErreur_->setText(message);
		messageErreur_->show();
		QTimer::singleShot(2000, messageErreur_, &QLabel::hide);
	}
	else if (success && !message.isEmpty())
	{
		if (message.contains("la partie se termine", Qt::CaseInsensitive)) {
			finPartie(message);
		}
		messageSucces_->setText(message);
		messageSucces_->show();
		QTimer::singleShot(2000, messageSucces_, &QLabel::hide);
	}
}

void interfaceGraphique::ProjetJeuxEchecs::surPieceClic(const ModeleJeu::Coordonnee& coordonnee)
{
	if (jeu_->getPiece(coordonnee) != nullptr && jeu_.get()->getCouleurPiece(coordonnee) == joueur) {
		jeu_.get()->setCaseSelectionnee(coordonnee);
		emit pieceSelectionnee(coordonnee);
	}
	else if (!jeu_.get()->getCaseSelectionnee().estValide()) {
		messageErreur_->setText("Aucune piece selectionnee");
		messageErreur_->show();
		QTimer::singleShot(2000, messageErreur_, &QLabel::hide);
	}
	else {
		ModeleJeu::Coordonnee source = jeu_.get()->getCaseSelectionnee();
		std::tuple<bool, std::string> deplacement = jeu_->deplacerPiece(source, joueur, coordonnee);

		if (get<0>(deplacement)) {
			jeu_.get()->setCaseSelectionnee(ModeleJeu::Coordonnee(-1, -1));

			ModeleJeu::Couleur nouveauJoueur;
			if (joueur != ModeleJeu::Couleur::Blanc)
				nouveauJoueur = ModeleJeu::Couleur::Blanc;
			else nouveauJoueur = ModeleJeu::Couleur::Noir;

			joueur = nouveauJoueur;
			emit tourChange(nouveauJoueur);
		}

		surDeplacementValide(get<0>(deplacement), QString::fromStdString(get<1>(deplacement)));
	}
	miseAJour();
}

void interfaceGraphique::ProjetJeuxEchecs::surTourChange(ModeleJeu::Couleur nouveauJoueur)
{
	if (nouveauJoueur == ModeleJeu::Couleur::Blanc) {
		tourLabel_->setText("Blanc");
		tourLabel_->setStyleSheet(
			"QLabel { "
			"color: black; "
			"background-color: white; "
			"padding: 6px; "
			"border-radius: 8px; "
			"font-weight: bold; "
			"font-size: 12px; "
			"}"
		);
	}
	else {
		tourLabel_->setText("Noir");
		tourLabel_->setStyleSheet(
			"QLabel { "
			"color: white; "
			"background-color: #343a40; "
			"padding: 6px; "
			"border-radius: 8px; "
			"font-weight: bold; "
			"font-size: 12px; "
			"}"
		);
	}
}

void interfaceGraphique::ProjetJeuxEchecs::surChangementPlacement() {
	try {
		int placementType = choixPlacement_->currentData().toInt();
		jeu_.get()->miseEnPlacement(static_cast<ModeleJeu::Placement>(placementType));

		ModeleJeu::Coordonnee aucunePieceSelectionnee(-1, -1);
		jeu_.get()->setCaseSelectionnee(aucunePieceSelectionnee);
		emit pieceSelectionnee(aucunePieceSelectionnee);

		joueur = ModeleJeu::Couleur::Blanc;
		emit tourChange(ModeleJeu::Couleur::Blanc);

		miseAJour();
		messageErreur_->setText("");
	}
	catch (const std::exception& e) {
		messageErreur_->setText(QString::fromStdString(e.what()));
		messageErreur_->show();
		QTimer::singleShot(2000, messageErreur_, &QLabel::hide);
	}
}

interfaceGraphique::ProjetJeuxEchecs::~ProjetJeuxEchecs() {
	delete ui;
}

void interfaceGraphique::ProjetJeuxEchecs::miseAJour() {

	for (int i = 0; i < ModeleJeu::tailleEchiquier; ++i) {
		for (int j = 0; j < ModeleJeu::tailleEchiquier; ++j) {
			ModeleJeu::Piece* piece = jeu_->getPiece(ModeleJeu::Coordonnee(i, j));
			ModeleJeu::Couleur couleur = jeu_->getCouleurPiece(ModeleJeu::Coordonnee(i, j));
			QString imagePiece = "";
			QString styleBase;

			if ((i + j) % 2 == 0) {
				styleBase = "background-color: #f0d9b5;";
			}
			else {
				styleBase = "background-color: #b58863;";
			}

			if (piece) {
				if (couleur == ModeleJeu::Couleur::Blanc) {
					styleBase += " color: white;";
				}
				else {
					styleBase += " color: black;";
				}

				if (dynamic_cast<ModeleJeu::Roi*>(piece)) {
					if (couleur == ModeleJeu::Couleur::Blanc) {
						imagePiece = "♔";
					}
					else {
						imagePiece = "♚";
					}
				}
				else if (dynamic_cast<ModeleJeu::Tour*>(piece)) {
					if (couleur == ModeleJeu::Couleur::Blanc) {
						imagePiece = "♖";
					}
					else {
						imagePiece = "♜";
					}
				}
				else if (dynamic_cast<ModeleJeu::Cavalier*>(piece)) {
					if (couleur == ModeleJeu::Couleur::Blanc) {
						imagePiece = "♘";
					}
					else {
						imagePiece = "♞";
					}
				}
			}

			if (jeu_.get()->getCaseSelectionnee() == ModeleJeu::Coordonnee(i, j)) {
				styleBase += " border: 4px solid green;";
			}
			else {
				styleBase += " border: none;";
			}

			boutons[i][j]->setText(imagePiece);
			boutons[i][j]->setStyleSheet(styleBase);
		}
	}
}
