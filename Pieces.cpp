#include "ProjetJeuxEchecs.h"
#include <cmath>


//Commentaire de noe: il manque de savoir si il y a des pieces sur le chemin du deplacement pour voir si il faut arreter le mouvement de la piece et les namespaces
int ModelePieces::Roi::compteurRoi_ = 0;

ModelePieces::Roi::Roi(int posXDebut, int posYDebut) : posX(posXDebut), posY(posYDebut) {
	//	compteurRoi_++;

	if (compteurRoi_ < 2) {
		compteurRoi_++;
	}
	else {
		throw CompteurRoisException(" Plus de deux instancess de rois ");
	}
}

ModelePieces::Roi::~Roi() {
	compteurRoi_--;
}

void  ModelePieces::Roi::deplacer(int x, int y) {
	if (verifierDeplacment(x, y)) {
		posX = x;
		posY = y;
	}
}

bool ModelePieces::Roi::verifierDeplacment(int x, int y) {
	return (abs(posX - x) <= 1 && abs(posY - y) <= 1);
}

int ModelePieces::Roi::getCompteurRoi() {
	return compteurRoi_;
}


bool  ModelePieces::Tour::verifierDeplacment(int x, int y) {
	return (posX == x || posY == y);

}

bool  ModelePieces::Cavalier::verifierDeplacment(int x, int y) {
	int dx = abs(posX - x);
	int dy = abs(posY - y);
	return (dx == 2 && dy == 1) || (dx == 1 && dy == 2);
}

void  ModelePieces::Tour::deplacer(int x, int y) {
	if (verifierDeplacment(x, y)) {
		posX = x;
		posY = y;
	}
}

void  ModelePieces::Cavalier::deplacer(int x, int y) {
	if (verifierDeplacment(x, y)) {
		posX = x;
		posY = y;
	}
}