#include "entreprise.h"
#include <QString>
#include <QtSql/QSqlQueryModel>
#include <QtSql/QSqlQuery>
#include <QString>

entreprise::entreprise()
{
    nom = "";
    adresse = "";
    secteur = "";
}

entreprise::entreprise(QString nom, QString adresse, QString secteur)
{
    this->nom = nom;
    this->adresse = adresse;
    this->secteur = secteur;
}

int entreprise::get_id() { return id; }
void entreprise::set_nom(QString val) { nom = val; }
QString entreprise::get_nom() { return nom; }
void entreprise::set_adresse(QString val) { adresse = val; }
QString entreprise::get_adresse() { return adresse; }
void entreprise::set_secteur(QString val) { secteur = val; }
QString entreprise::get_secteur() { return secteur; }

bool entreprise::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO entreprise (nom, adresse, secteur)"
                  "VALUES (:nom, :adresse, :secteur)");
    query.bindValue(":nom", nom);
    query.bindValue(":adresse", adresse);
    query.bindValue(":secteur", secteur);
    return query.exec();
}

QSqlQueryModel* entreprise::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM entreprise");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Secteur"));
    return model;
}

QSqlQueryModel* entreprise::trier_id()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM entreprise ORDER BY id");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Secteur"));
    return model;
}

QSqlQueryModel* entreprise::trier_nom()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM entreprise ORDER BY nom");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Secteur"));
    return model;
}

QSqlQueryModel* entreprise::trier_secteur()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM entreprise ORDER BY secteur");
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Adresse"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Secteur"));
    return model;
}

bool entreprise::supprimer(int idd)
{
    QSqlQuery query;
    query.prepare("DELETE FROM entreprise WHERE id = :id");
    query.bindValue(":id", idd);
    return query.exec();
}

bool entreprise::modifier(int id, QString nom, QString adresse, QString secteur)
{
    QSqlQuery qry;
    qry.prepare("UPDATE entreprise SET nom = (?), adresse = (?), secteur = (?) WHERE id = (?)");
    qry.addBindValue(nom);
    qry.addBindValue(adresse);
    qry.addBindValue(secteur);
    qry.addBindValue(id);
    return qry.exec();
}
