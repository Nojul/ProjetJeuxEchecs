/**
* Definition de la classe ProjetJeuxEchecs qui est le modele de l'interface graphique
* \file   ProjetJeuxEchecs.h
* \author Latendresse et Julien
* \date   4 mai 2025
* Cree le 6 avril 2025
*/
#include "Pieces.h"
#include "ui_ProjetJeuxEchecs.h"
#include <memory> 
#include <QComboBox>
#include <QLabel>
#include <qpushbutton.h>
#include <QtWidgets/QMainWindow>


QT_BEGIN_NAMESPACE
namespace Ui { class ProjetJeuxEchecsClass; };
QT_END_NAMESPACE

namespace interfaceGraphique {
	class ProjetJeuxEchecs : public QMainWindow
	{
		Q_OBJECT

	public:
		ProjetJeuxEchecs(QWidget* parent = nullptr);
		~ProjetJeuxEchecs();
		ProjetJeuxEchecs(const ProjetJeuxEchecs&) = delete;
		ProjetJeuxEchecs& operator=(const ProjetJeuxEchecs&) = delete;
		void faireConnections();

	public slots:
		void miseAJour();
		void surChangementPlacement();
		void surPieceClic(const ModeleJeu::Coordonnee& coordonnee);
		void surDeplacementValide(bool success, const QString& message);
		void surPieceSelectionnee(const ModeleJeu::Coordonnee& coordonnee) const;
		void surTourChange(ModeleJeu::Couleur nouveauJoueur);

	signals:
		void pieceSelectionnee(const ModeleJeu::Coordonnee& coordonnee);
		void tourChange(ModeleJeu::Couleur nouveauJoueur);
		void miseEnEchec();

	private:
		Ui::ProjetJeuxEchecsClass* ui;
		QPushButton* boutons[ModeleJeu::tailleEchiquier][ModeleJeu::tailleEchiquier];
		std::unique_ptr<ModeleJeu::JeuPrincipal> jeu_;
		ModeleJeu::Couleur joueur = ModeleJeu::Couleur::Blanc;
		QLabel* messageErreur_ = nullptr;
		QLabel* messageSucces_ = nullptr;
		QComboBox* choixPlacement_ = nullptr;
		QLabel* tourLabel_ = nullptr;
	};

}

