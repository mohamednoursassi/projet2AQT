#ifndef OFFRE_EMPLOI_H
#define OFFRE_EMPLOI_H

#include <QString>
#include <QSqlQuery>
#include <QSqlQueryModel>

class OffreEmploi
{
    int ide;
    QString titre;
    QString contrat;
    QString lieu;
    float salaire;
    QString statut;
public:
    // Constructeurs
    OffreEmploi() {};
    OffreEmploi(int, QString, QString, QString, float, QString);

    // Getters
    int getIde() const { return ide; }
    QString getTitre() const { return titre; }
    QString getContrat() const { return contrat; }
    QString getLieu() const { return lieu; }
    float getSalaire() const { return salaire; }
    QString getStatut() const { return statut; }

    // Setters
    void setIde(int id) { ide = id; }
    void setTitre(const QString& t) { titre = t; }
    void setContrat(const QString& c) { contrat = c; }
    void setLieu(const QString& l) { lieu = l; }
    void setSalaire(float s) { salaire = s; }
    void setStatut(const QString& s) { statut = s; }

    // Fonctions relatives
    QSqlQueryModel* chercherOffre(const QString& searchTerm);
    QSqlQueryModel* searchByTitre(const QString& titre);
    QSqlQueryModel* searchByLieu(const QString& lieu);
    QSqlQueryModel* searchByStatut(const QString& statut);
    QSqlQueryModel* trierParSalaireAsc();
    QSqlQueryModel* trierParSalaireDesc();
    QSqlQueryModel* trierParID();
    QSqlQueryModel* afficher();
    bool ajouter();
    bool exists(int id) ;

    bool mettreAJour(int id);
    bool supprimer(int id);
};

#endif // OFFRE_EMPLOI_H
