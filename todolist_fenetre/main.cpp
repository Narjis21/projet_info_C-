#include <QApplication>
#include "fenetre_mode.h"
#include "classe.h"


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    fenetreMode fenetre;
    fenetre.show();


    return app.exec();
}
