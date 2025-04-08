#include "ProjetJeuxEchecs.h"
#include <QLabel>
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
	QPushButton* buttons[tailleEchiquier][tailleEchiquier];

	for (int i = 0; i < tailleEchiquier; ++i) {		//lignes
		for (int j = 0; j < tailleEchiquier; ++j) {	//colonnes
			QPushButton* button = new QPushButton(this);
			buttons[i][j] = button;
			buttons[i][j]->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

			//code pour afficher du texte sur des bouttons
			/*buttons[i][j]->setText("R");
			QFont font = buttons[i][j]->font();
			font.setPointSize(20);
			buttons[i][j]->setFont(font);*/

			if ((i + j) % 2 == 0) {
				buttons[i][j]->setStyleSheet("background-color: #f0d9b5; border: none;");

			}
			else {
				buttons[i][j]->setStyleSheet("background-color: #b58863; border: none;");

			}

			gridLayout->addWidget(buttons[i][j], i, j);
		}
	}
}

interfaceGraphique::ProjetJeuxEchecs::~ProjetJeuxEchecs()
{
	delete ui;
}