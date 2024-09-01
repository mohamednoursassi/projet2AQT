#include "Candidat.h"
#include "qsqlerror.h"
#include <QSqlQuery>
#include <QSqlQueryModel>

Candidat::Candidat(int idc, QString nom, QString prenom, QString email, QString telephone, int offreId)
    : idc(idc), nom(nom), prenom(prenom), email(email), telephone(telephone), offreId(offreId) {}

bool Candidat::ajouter()
{
    QSqlQuery query;
    query.prepare("INSERT INTO candidat (IDC, NOM, PRENOM, EMAIL, TELEPHONE, OFFREID) "
                  "VALUES (:idc, :nom, :prenom, :email, :telephone, :offreId)");
    query.bindValue(":idc", idc); // bindvalue: pour des raisons de securite injection sql
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);
    query.bindValue(":offreId", offreId);
    return query.exec();
}

QSqlQueryModel* Candidat::afficher()
{
    QSqlQueryModel* model = new QSqlQueryModel();
    model->setQuery("SELECT c.IDC, c.NOM, c.PRENOM, c.EMAIL, c.TELEPHONE, o.TITRE "
                    "FROM candidat c "
                    "JOIN offre_emploi o ON c.OFFREID = o.IDE");
// join offreemloi ... : jointure entre candidat et son offre relatif choisi

    model->setHeaderData(0, Qt::Horizontal, QObject::tr("IDC")); // lignes et colonnes affichage
    model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
    model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
    model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
    model->setHeaderData(4, Qt::Horizontal, QObject::tr("Téléphone"));
    model->setHeaderData(5, Qt::Horizontal, QObject::tr("Titre Offre"));

    return model;
}
bool Candidat::mettreAJour(int id)
{
    QSqlQuery query;
    query.prepare("UPDATE candidat SET NOM = :nom, PRENOM = :prenom, EMAIL = :email, TELEPHONE = :telephone, OFFREID = :offreId WHERE IDC = :idc");
    query.bindValue(":nom", nom);
    query.bindValue(":prenom", prenom);
    query.bindValue(":email", email);
    query.bindValue(":telephone", telephone);
    query.bindValue(":offreId", offreId);
    query.bindValue(":idc", id);

    return query.exec();
}

bool Candidat::supprimer(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM candidat WHERE IDC = :id");
    query.bindValue(":id", id);
    return query.exec();
}
bool Candidat::exists(int id) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM candidat WHERE IDC = :id");
    query.bindValue(":id", id);
    query.exec();

    if (query.next()) {
        int count = query.value(0).toInt();
        return count > 0;
    }
    return false;
}

QSqlQueryModel* Candidat::chercherParNom(const QString& nom)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT * FROM candidat WHERE NOM LIKE :nom");
    query.bindValue(":nom", "%" + nom + "%");

    if (query.exec()) {
        model->setQuery(query);
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("IDC"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("Téléphone"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("Offre ID"));
    }
    return model;
}

QSqlQueryModel* Candidat::chercherParPrenom(const QString& prenom)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT * FROM candidat WHERE PRENOM LIKE :prenom");
    query.bindValue(":prenom", "%" + prenom + "%");

    if (query.exec()) {
        model->setQuery(query);
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("IDC"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("Téléphone"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("Offre ID"));
    }
    return model;
}

QSqlQueryModel* Candidat::chercherParEmail(const QString& email)
{
    QSqlQueryModel* model = new QSqlQueryModel();
    QSqlQuery query;
    query.prepare("SELECT * FROM candidat WHERE EMAIL LIKE :email");
    query.bindValue(":email", "%" + email + "%");

    if (query.exec()) {
        model->setQuery(query);
        model->setHeaderData(0, Qt::Horizontal, QObject::tr("IDC"));
        model->setHeaderData(1, Qt::Horizontal, QObject::tr("Nom"));
        model->setHeaderData(2, Qt::Horizontal, QObject::tr("Prénom"));
        model->setHeaderData(3, Qt::Horizontal, QObject::tr("Email"));
        model->setHeaderData(4, Qt::Horizontal, QObject::tr("Téléphone"));
        model->setHeaderData(5, Qt::Horizontal, QObject::tr("Offre ID"));
    }
    return model;
}
QMap<int, int> Candidat::getCandidateCountByOffreId() {
    QMap<int, int> offreToCandidateCountMap;

    QSqlQuery query;
    query.prepare("SELECT offreid, COUNT(*) as candidate_count FROM candidat GROUP BY offreid");
    if (!query.exec()) {
        return offreToCandidateCountMap;  // Return empty map in case of error
    }

    while (query.next()) {
        int offreId = query.value(0).toInt();
        int candidateCount = query.value(1).toInt();
        offreToCandidateCountMap[offreId] = candidateCount;
    }

    return offreToCandidateCountMap;
}
