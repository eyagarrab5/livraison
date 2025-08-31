#ifndef COMMANDE_H
#define COMMANDE_H

#include <QString>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQueryModel>
#include <QDebug>

class commande
{
public:
    commande();
    commande(QString, QString, QString, QString, QString);

    int get_id();
    void Set_nom_commande(QString val);
    QString get_nom_commande();
    void Set_reference(QString val);
    QString get_reference();
    void Set_date_commande(QString val); // stockée en texte ou QDate selon ton implémentation
    QString get_date_commande();
    void Set_adresse_livraison(QString val);
    QString get_adresse_livraison();
    void Set_statut(QString val);
    QString get_statut();

    ///////////// crud /////////////////
    bool ajouter();
    bool modifier(int, QString, QString, QString, QString, QString);
    bool supprimer(int idd);
    QSqlQueryModel* afficher();

    /////////// metier ///////////////
    QSqlQueryModel* trie_id();
    QSqlQueryModel* trie_nom_commande();
    QSqlQueryModel* trie_reference();

private:
    int id;
    QString nom_commande;
    QString reference;
    QString date_commande;
    QString adresse_livraison;
    QString statut;
};

#endif // COMMANDE_H
