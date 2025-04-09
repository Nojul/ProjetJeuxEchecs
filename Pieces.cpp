#include "ProjetJeuxEchecs.h"
#include <cmath>


//Commentaire de noe: il manque de savoir si il y a des pieces sur le chemin du deplacement pour voir si il faut arreter le mouvement de la piece et les namespaces
int ModeleJeu::Roi::compteurRoi_ = 0;

ModeleJeu::Roi::Roi(int posXDebut, int posYDebut, std::string couleur) :Piece(posXDebut, posYDebut, couleur) {
	if (compteurRoi_ < 2) {
		compteurRoi_++;
	}
	else {
		throw CompteurRoisException(" Plus de deux instancess de rois ");
	}
}

ModeleJeu::Roi::~Roi() {
	compteurRoi_--;
}

void  ModeleJeu::Roi::deplacer(int x, int y) {
	if (verifierDeplacment(x, y)) {
		posX = x;
		posY = y;
	}
}

bool ModeleJeu::Roi::verifierDeplacment(int x, int y) {
	return (abs(posX - x) <= 1 && abs(posY - y) <= 1);
}

int ModeleJeu::Roi::getCompteurRoi() {
	return compteurRoi_;
}


bool  ModeleJeu::Tour::verifierDeplacment(int x, int y) {
	return (posX == x || posY == y);

}

bool  ModeleJeu::Cavalier::verifierDeplacment(int x, int y) {
	int dx = abs(posX - x);
	int dy = abs(posY - y);
	return (dx == 2 && dy == 1) || (dx == 1 && dy == 2);
}

void  ModeleJeu::Tour::deplacer(int x, int y) {
	if (verifierDeplacment(x, y)) {
		posX = x;
		posY = y;
	}
}

void  ModeleJeu::Cavalier::deplacer(int x, int y) {
	if (verifierDeplacment(x, y)) {
		posX = x;
		posY = y;
	}
}

ModeleJeu::JeuPrincipal::JeuPrincipal(int placement) {
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			echiquier[i][j] = nullptr;
		}
	}
	//QtWidgets.QComboBox.currentIndex()¶
	//Return type :
	//int
	//un combobox au debut pour choisir quel position faire, le combobox donne un int dependant du choix
	//https://en.wikipedia.org/wiki/Pawnless_chess_endgame
	/*    0 1 2 3 4 5 6 7
		0 . . . . . . . .
		1 . . . . . . . .
		2 . . . . . . . .
		3 . . . . . . . .
		4 . . . . . . . .
		5 . . . . . . . .
		6 . . . . . . . .
		7 . . . . . . . .
		  0 1 2 3 4 5 6 7*/

	switch (placement) {

	case 0: //La Bourdonnais vs. McDonnell, 1834

		echiquier[0][1] = std::make_unique<Tour>(0, 1, "Blanc");
		echiquier[1][0] = std::make_unique<Cavalier>(1, 0, "Noir");
		echiquier[3][0] = std::make_unique<Roi>(3, 0, "Noir");
		echiquier[3][2] = std::make_unique<Roi>(3, 2, "Blanc");
		break;

	case 1: //Karpov vs. Ftacnik, 1988

		break;

	case 2: //J. Polgar vs. Kasparov, 1996[40]

		break;

	case 3: //Alekhine vs. Capablanca, 1927[41]

		break;

	}

}