#include "ProjetJeuxEchecs.h"
#include <QLabel>
#include <QString>
#include <qpushbutton.h>
#include <QVBoxLayout> 

interfaceGraphique::ProjetJeuxEchecs::ProjetJeuxEchecs(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::ProjetJeuxEchecsClass())
{
	ui->setupUi(this);
	this->resize(800, 800);
	QGridLayout* gridLayout = new QGridLayout();
	gridLayout->setSpacing(0);
	QWidget* centralWidget = new QWidget(this);
	centralWidget->setLayout(gridLayout);
	setCentralWidget(centralWidget);

	const int tailleEchiquier = 8;
	//QPushButton* buttons[tailleEchiquier][tailleEchiquier];

	for (int i = 0; i < tailleEchiquier; ++i) {		//lignes
		for (int j = 0; j < tailleEchiquier; ++j) {	//colonnes
			//QPushButton* button = new QPushButton(this);
			//buttons[i][j] = button;
			this->buttons[i][j] = new QPushButton(this);
			this->buttons[i][j]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

			//code pour afficher du texte sur des bouttons
			/*buttons[i][j]->setText("R");
			QFont font = buttons[i][j]->font();
			font.setPointSize(20);
			buttons[i][j]->setFont(font);*/

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
			connect(this->buttons[i][j], &QPushButton::clicked, this, [=]() {
				clic(i, j);
				qDebug() << "Bouton clique :" << i << "," << j;
				});
		}
	}
	jeu = new ModeleJeu::JeuPrincipal(1);
	miseAJour();
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
		jeu->deplacerPiece(xSelectionne, ySelectionne, joueur, x, y);
		qDebug() << "Deplacement";
		miseAJour();
		//Sauf si deplacement invalide... Verifier
		if (joueur == "Blanc")
		{
			joueur = "Noir";
		}
		else
		{
			joueur = "Blanc";
		}
		xSelectionne = -1;
		ySelectionne = -1;
	}
}

void interfaceGraphique::ProjetJeuxEchecs::miseAJour()
{
	for (int i = 0; i < 8; ++i) {		//lignes
		for (int j = 0; j < 8; ++j) {	//colonnes
			ModeleJeu::Piece* piece = jeu->getPiece(i, j);
			if (piece)
			{
				QString imagePiece;
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
				if (dynamic_cast<ModeleJeu::Tour*>(piece))
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
				if (dynamic_cast<ModeleJeu::Cavalier*>(piece))
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
			}
			else
			{
				buttons[i][j]->setText("");
			}
		}
	}
}
