#include "employee.h"
#include <QString>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlQuery>
#include <QString>
#include <QFile>
#include <QTextDocument>
#include <QTextStream>
#include <QIODevice>
#include <QDate>
#include <QSystemTrayIcon>

using namespace std;

employee::employee()
{
    nom = " ";
    prenom = " ";
    adresse = "";
    num = " ";
    genre = " ";
    id_entreprise = 0;  // Initialize id_entreprise
}

employee::employee(QString nom, QString prenom, QString adresse, QString num, QString genre, int id_entreprise)
{
    this->nom = nom;
    this->prenom = prenom;
    this->adresse = adresse;
    this->num = num;
    this->genre = genre;
    this->id_entreprise = id_entreprise;  // Initialize id_entreprise
}

int employee::get_id() { return id; }
void employee::Set_nom(QString val) { nom = val; }
QString employee::get_nom() { return nom; }
void employee::Set_prenom(QString val) { prenom = val; }
QString employee::get_prenom() { return prenom; }
void employee::Set_adresse(QString val) { adresse = val; }
QString employee::get_adresse() { return adresse; }
void employee::Set_num(QString val) { num = val; }
QString employee::get_num() { return num; }
void employee::Set_genre(QString val) { genre = val; }
QString employee::get_genre() { return genre; }
void employee::Set_id_entreprise(int val) { id_entreprise = val; }
int employee::get_id_entreprise() { return id_entreprise; }

bool employee::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO employee (nom, prenom, adresse, num, genre, id_entreprise)"
                  "VALUES (:nom, :prenom, :adresse, :num, :genre, :id_entreprise)");
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":adresse", adresse);
    query.bindValue(":num", num);
    query.bindValue(":genre", genre);
    query.bindValue(":id_entreprise", id_entreprise);  // Bind id_entreprise
    return query.exec();
}

QSqlQueryModel* employee::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM employee");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prenom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Num"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Genre"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("ID Entreprise"));  // New header for id_entreprise
    return model;
}

QSqlQueryModel* employee::trie_id()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM employee ORDER BY id");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prenom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Num"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Genre"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("ID Entreprise"));  // New header for id_entreprise
    return model;
}

QSqlQueryModel* employee::trie_nom()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM employee ORDER BY nom");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prenom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Num"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Genre"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("ID Entreprise"));  // New header for id_entreprise
    return model;
}

QSqlQueryModel* employee::trie_prenom()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM employee ORDER BY prenom");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prenom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Num"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Genre"));
    model->setHeaderData(6, Qt::Horizontal, QObject::tr("ID Entreprise"));  // New header for id_entreprise
    return model;
}

bool employee::supprimer(int idd)
{
    QSqlQuery query;
    query.prepare("DELETE FROM employee WHERE id = :id");
    query.bindValue(":id", idd);
    return query.exec();
}

bool employee::modifier(int id, QString nom, QString prenom, QString adresse, QString num, QString genre, int id_entreprise)
{
    QSqlQuery qry;
    qry.prepare("UPDATE employee SET nom = (?), prenom = (?), adresse = (?), num = (?), genre = (?), id_entreprise = (?) WHERE id = (?)");
    qry.addBindValue(nom);
    qry.addBindValue(prenom);
    qry.addBindValue(adresse);
    qry.addBindValue(num);
    qry.addBindValue(genre);
    qry.addBindValue(id_entreprise);  // Bind id_entreprise
    qry.addBindValue(id);
    return qry.exec();
}
