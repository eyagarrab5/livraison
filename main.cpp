#include "setroles.h"
#include <QApplication>
#include <QMessageBox>
#include "connection.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Connection c;
    if (!c.createconnect()) {
        QMessageBox::critical(nullptr, "Erreur", "Connexion à la base de données échouée.");
        return -1;
    }

    SetRoles r;
    r.show();

    return a.exec();
}
