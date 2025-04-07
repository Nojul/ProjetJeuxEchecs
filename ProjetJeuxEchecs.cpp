#include "ProjetJeuxEchecs.h"
#include <QLabel>
#include <qpushbutton.h>
#include <QVBoxLayout> 

interfaceGraphique::ProjetJeuxEchecs::ProjetJeuxEchecs(QWidget* parent)
	: QMainWindow(parent)
	, ui(new Ui::ProjetJeuxEchecsClass())
{
	ui->setupUi(this);

	QGridLayout* gridLayout = new QGridLayout();
	gridLayout->setSpacing(0);
	QWidget* centralWidget = new QWidget(this);
	centralWidget->setLayout(gridLayout);
	setCentralWidget(centralWidget);

	QPushButton* buttons[8][8];

	for (int i = 0; i < 8; ++i) {		//lignes
		for (int j = 0; j < 8; ++j) {	//colonnes
			QPushButton* button = new QPushButton(this);
			buttons[i][j] = button;
			buttons[i][j]->setFixedSize(70, 70);

			if ((i + j) % 2 == 0) {
				buttons[i][j]->setStyleSheet("background-color: #eeeed2;");
			}
			else {
				buttons[i][j]->setStyleSheet("background-color: #769656;");
			}

			gridLayout->addWidget(buttons[i][j], i, j);
		}
	}
}

interfaceGraphique::ProjetJeuxEchecs::~ProjetJeuxEchecs()
{
	delete ui;
}