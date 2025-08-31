#ifndef CLIENT_H
#define CLIENT_H

#include <QString>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQueryModel>
#include <QDebug>

class client
{
public:
    client();
    client(QString, QString, QString, QString, QString);

    int get_id();
    void set_nom(QString val);
    QString get_nom();
    void set_prenom(QString val);
    QString get_prenom();
    void set_adresse(QString val);
    QString get_adresse();
    void set_telephone(QString val);
    QString get_telephone();
    void set_email(QString val);
    QString get_email();

    ///////////// Opérations CRUD /////////////////
    bool ajouter();
    bool modifier(int, QString, QString, QString, QString, QString);
    bool supprimer(int idd);
    QSqlQueryModel* afficher();

    /////////// Méthodes de tri ///////////////
    QSqlQueryModel* trier_id();
    QSqlQueryModel* trier_nom();
    QSqlQueryModel* trier_prenom();

private:
    int id;
    QString nom;
    QString prenom;
    QString adresse;
    QString telephone;
    QString email;
};

#endif // CLIENT_H
