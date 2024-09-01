#include "OffreEmploi.h"
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QDebug>
#include <QSqlError>
OffreEmploi::OffreEmploi(int ide, QString titre, QString contrat, QString lieu, float salaire, QString statut)
    : ide(ide), titre(titre), contrat(contrat), lieu(lieu), salaire(salaire), statut(statut) {}

bool OffreEmploi::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO offre_emploi (IDE, TITRE, CONTRAT, LIEU, SALAIRE, STATUT) "
                  "VALUES (:ide, :titre, :contrat, :lieu, :salaire, :statut)");
    query.bindValue(":ide", ide); // INTEGER for IDE
    query.bindValue(":titre", titre); // STRING for TITRE
    query.bindValue(":contrat", contrat); // STRING for CONTRAT
    query.bindValue(":lieu", lieu); // STRING for LIEU
    query.bindValue(":salaire", QVariant(salaire).toDouble()); // DOUBLE for SALAIRE
    query.bindValue(":statut", statut); // STRING for STATUT

    qDebug() << "Executing query: " << query.executedQuery();
    qDebug() << "Bound values: " << query.boundValues();

    return query.exec();
}

QSqlQueryModel* OffreEmploi::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();

    model->setQuery("SELECT IDE, TITRE, CONTRAT, LIEU, SALAIRE, STATUT FROM offre_emploi");

    if (model->query().lastError().isValid()) {
        qDebug() << "Query Error:" << model->query().lastError().text();
    } else {
        qDebug() << "Number of rows:" << model->rowCount();
    }
    model->setHeaderData(0, Qt::Horizontal, QObject::tr("ID"));
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Titre"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Contrat"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Lieu"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Salaire"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Statut"));

    return model;
}
bool OffreEmploi::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM offre_emploi WHERE ide = :id");
    query.bindValue(":id", id);
    return query.exec();
}
bool OffreEmploi::exists(int id) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM offre_emploi WHERE ide = :id");
    query.bindValue(":id", id);
    query.exec();

    if (query.next()) {
        int count = query.value(0).toInt();
        return count > 0;
    }
    return false;
}

bool OffreEmploi::mettreAJour(int ide)
{
    QSqlQuery query;
    QString res = QString::number(ide);
    QString salaireFormatted = QString::number(salaire, 'f', 2);
    query.prepare("UPDATE offre_emploi SET TITRE = :titre, CONTRAT = :contrat, LIEU = :lieu, SALAIRE = :salaire, STATUT = :statut WHERE IDE = :ide");
    query.bindValue(":titre", titre);
    query.bindValue(":contrat", contrat);
    query.bindValue(":lieu", lieu);
    query.bindValue(":salaire", salaireFormatted);
    query.bindValue(":statut", statut);
    query.bindValue(":ide", res);

    return query.exec();
}

    /*
     *
     * RECHERCHE
     *
     * */
QSqlQueryModel* OffreEmploi::searchByTitre(const QString& titre)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;

    QString queryString = "SELECT * FROM offre_emploi WHERE TITRE LIKE :titre";
    query.prepare(queryString);
    query.bindValue(":titre", "%" + titre + "%"); // Use LIKE for partial matches

    if (query.exec())
    {
        model->setQuery(query);
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("IDE"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Titre"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Contrat"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("Lieu"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("Salaire"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("Statut"));
    }
    else
    {
        qDebug() << "Error executing query:" << query.lastError().text();
    }

    return model;
}

QSqlQueryModel* OffreEmploi::searchByLieu(const QString& lieu)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;

    QString queryString = "SELECT * FROM offre_emploi WHERE LIEU LIKE :lieu";
    query.prepare(queryString);
    query.bindValue(":lieu", "%" + lieu + "%"); // Use LIKE for partial matches

    if (query.exec())
    {
        model->setQuery(query);
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("IDE"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Titre"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Contrat"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("Lieu"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("Salaire"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("Statut"));
    }
    else
    {
        qDebug() << "Error executing query:" << query.lastError().text();
    }

    return model;
}

QSqlQueryModel* OffreEmploi::searchByStatut(const QString& statut)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;

    QString queryString = "SELECT * FROM offre_emploi WHERE STATUT LIKE :statut";
    query.prepare(queryString);
    query.bindValue(":statut", "%" + statut + "%");

    if (query.exec())
    {
        model->setQuery(query);
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("IDE"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Titre"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Contrat"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("Lieu"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("Salaire"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("Statut"));
    }
    else
    {
        qDebug() << "Error executing query:" << query.lastError().text();
    }

    return model;
}


QSqlQueryModel* OffreEmploi::trierParSalaireAsc()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM offre_emploi ORDER BY SALAIRE ASC");
    return model;
}

QSqlQueryModel* OffreEmploi::trierParSalaireDesc()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM offre_emploi ORDER BY SALAIRE DESC");
    return model;
}
QSqlQueryModel* OffreEmploi::trierParID(){
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT * FROM offre_emploi ORDER BY IDE ASC");
    return model;
}

