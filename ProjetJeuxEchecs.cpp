#include "ProjetJeuxEchecs.h"
#include <QLabel>
#include <QString>
#include <qpushbutton.h>
#include <QVBoxLayout> 
#include <QTimer>


interfaceGraphique::ProjetJeuxEchecs::ProjetJeuxEchecs(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::ProjetJeuxEchecsClass())
{
	ui->setupUi(this);
	this->resize(800, 800);
	QGridLayout* gridLayout = new QGridLayout();
	gridLayout->setSpacing(0);
	QWidget* centralWidget = new QWidget(this);
	//centralWidget->setLayout(gridLayout);
	setCentralWidget(centralWidget);

	const int tailleEchiquier = 8;

	for (int i = 0; i < tailleEchiquier; ++i) {		//lignes
		for (int j = 0; j < tailleEchiquier; ++j) {	//colonnes
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

	messageErreur_ = new QLabel(this);
	//messageErreur_->setStyleSheet("color: red");
	messageErreur_->setAlignment(Qt::AlignBottom | Qt::AlignRight);
	messageErreur_->setText("");

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
		std::tuple<bool, std::string> deplacement = jeu -> deplacerPiece(xSelectionne, ySelectionne, joueur, x, y);
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
