#include "ProjetJeuxEchecs.h"
#include <cmath>


//Commentaire de noe: il manque de savoir si il y a des pieces sur le chemin du deplacement pour voir si il faut arreter le mouvement de la piece et les namespaces
int ModeleJeu::Roi::compteurRoi_ = 0;

ModeleJeu::Roi::Roi(int posXDebut, int posYDebut) : posX(posXDebut), posY(posYDebut) {
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