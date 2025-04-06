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
	virtual void deplacer(int x, int y) = 0;
};

class Roi : public Piece {

};

class Tour : public Piece {

};

class Cavalier : public Piece {

};


