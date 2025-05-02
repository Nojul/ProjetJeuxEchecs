#include "Pieces.h"
#include "ui_ProjetJeuxEchecs.h"
#include <memory> 
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


		void clic(int x, int y);
		void miseAJour();

	private:
		Ui::ProjetJeuxEchecsClass* ui;
		QPushButton* buttons[8][8];
		ModeleJeu::JeuPrincipal* jeu;
		std::string joueur = "Blanc";
		QLabel* messageErreur_ = nullptr;
	};

}

