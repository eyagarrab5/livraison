#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include <QString>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQueryModel>
#include <QDebug>

class employee
{
public:
    employee();
    employee(QString, QString, QString, QString, QString, int);

    int get_id();
    void Set_nom(QString val);
    QString get_nom();
    void Set_prenom(QString val);
    QString get_prenom();
    void Set_adresse(QString val);
    QString get_adresse();
    void Set_num(QString val);
    QString get_num();
    void Set_genre(QString val);
    QString get_genre();
    void Set_id_entreprise(int val);
    int get_id_entreprise();

    ///////////// crud /////////////////
    bool ajouter();
    bool modifier(int, QString, QString, QString, QString, QString, int);
    bool supprimer(int idd);
    QSqlQueryModel* afficher();

    /////////// metier ///////////////
    QSqlQueryModel* trie_id();
    QSqlQueryModel* trie_nom();
    QSqlQueryModel* trie_prenom();

private:
    int id;
    QString nom;
    QString prenom;
    QString adresse;
    QString num;
    QString genre;
    int id_entreprise;
};

#endif // EMPLOYEE_H
