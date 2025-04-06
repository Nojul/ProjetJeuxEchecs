#pragma once

#include "ui_ProjetJeuxEchecs.h"
#include <QtWidgets/QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class ProjetJeuxEchecsClass; };
QT_END_NAMESPACE

class ProjetJeuxEchecs : public QMainWindow
{
	Q_OBJECT

public:
	ProjetJeuxEchecs(QWidget* parent = nullptr);
	~ProjetJeuxEchecs();

private:
	Ui::ProjetJeuxEchecsClass* ui;
};

class Piece {
public:
	virtual ~Piece() = default;
	virtual bool verifierDeplacment(int x, int y) = 0;
	virtual void deplacer(int x, int y) = 0;
};

class Roi : public Piece {
public:
	Roi(int posXDebut, int posYDebut) : posX(posXDebut), posY(posYDebut) {}

	void deplacer(int x, int y) override;
	bool verifierDeplacment(int x, int y) override;

private:
	int posX;
	int posY;
};

class Tour : public Piece {
public:
	Tour(int posXDebut, int posYDebut) : posX(posXDebut), posY(posYDebut) {}

	void deplacer(int x, int y) override;
	bool verifierDeplacment(int x, int y) override;

private:
	int posX;
	int posY;
};

class Cavalier : public Piece {
public:
	Cavalier(int posXDebut, int posYDebut) : posX(posXDebut), posY(posYDebut) {}

	void deplacer(int x, int y) override;
	bool verifierDeplacment(int x, int y) override;

private:
	int posX;
	int posY;
};


