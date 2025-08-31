#ifndef ENTREPRISE_H
#define ENTREPRISE_H

#include <QString>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQueryModel>
#include <QDebug>

class entreprise
{
public:
    entreprise();
    entreprise(QString, QString, QString);

    int get_id();
    void set_nom(QString val);
    QString get_nom();
    void set_adresse(QString val);
    QString get_adresse();
    void set_secteur(QString val);
    QString get_secteur();

    ///////////// Opérations CRUD /////////////////
    bool ajouter();
    bool modifier(int, QString, QString, QString);
    bool supprimer(int idd);
    QSqlQueryModel* afficher();

    /////////// Méthodes de tri ///////////////
    QSqlQueryModel* trier_id();
    QSqlQueryModel* trier_nom();
    QSqlQueryModel* trier_secteur();

private:
    int id;
    QString nom;
    QString adresse;
    QString secteur;
};

#endif // ENTREPRISE_H
