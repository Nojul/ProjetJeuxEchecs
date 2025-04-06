#include "ProjetJeuxEchecs.h"
#include <cmath>

//Commentaire de noe: il manque de savoir si il y a des pieces sur le chemin du deplacement pour voir si il faut arreter le mouvement de la piece

bool Roi::verifierDeplacment(int x, int y) {
	return (abs(posX - x) <= 1 && abs(posY - y) <= 1);
}

bool Tour::verifierDeplacment(int x, int y) {
	return (posX == x || posY == y);

}

bool Cavalier::verifierDeplacment(int x, int y) {
	int dx = abs(posX - x);
	int dy = abs(posY - y);
	return (dx == 2 && dy == 1) || (dx == 1 && dy == 2);
}

void Roi::deplacer(int x, int y) {
	if (verifierDeplacment(x, y)) {
		posX = x;
		posY = y;
	}
}

void Tour::deplacer(int x, int y) {
	if (verifierDeplacment(x, y)) {
		posX = x;
		posY = y;
	}
}

void Cavalier::deplacer(int x, int y) {
	if (verifierDeplacment(x, y)) {
		posX = x;
		posY = y;
	}
}