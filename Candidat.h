#ifndef CANDIDAT_H
#define CANDIDAT_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class Candidat
{
    int idc;
    QString nom;
    QString prenom;
    QString email;
    QString telephone;
    int offreId;
public:
    // Constructeurs
    Candidat() {};
    Candidat(int, QString, QString, QString, QString, int);

    // Getters
    int getIdc() const { return idc; }
    QString getNom() const { return nom; }
    QString getPrenom() const { return prenom; }
    QString getEmail() const { return email; }
    QString getTelephone() const { return telephone; }
    int getOffreId() const { return offreId; }

    // Setters
    void setIdc(int id) { idc = id; }
    void setNom(const QString& n) { nom = n; }
    void setPrenom(const QString& p) { prenom = p; }
    void setEmail(const QString& e) { email = e; }
    void setTelephone(const QString& t) { telephone = t; }
    void setOffreId(int id) { offreId = id; }
    // Fonctions relatives
    QSqlQueryModel* chercherCandidat(int id, const QString& nom, const QString& prenom);
    QSqlQueryModel* afficher();
    bool ajouter();
    bool mettreAJour(int id);
    bool supprimer(int id);
    bool exists(int id);
    QMap<int, int>getCandidateCountByOffreId();

    QSqlQueryModel* chercherParNom(const QString& nom);
    QSqlQueryModel* chercherParPrenom(const QString& prenom);
    QSqlQueryModel* chercherParEmail(const QString& email);


};

#endif // CANDIDAT_H
