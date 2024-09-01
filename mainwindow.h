#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Candidat.h"
#include "OffreEmploi.h"
#include <QMainWindow>
#include <QSslSocket>
#include <QtNetwork/QAbstractSocket>
#include <QtNetwork/QSslSocket>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSortFilterProxyModel>
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    QSortFilterProxyModel *proxy;

    void makePlot(); // fontion statistiques


private slots:

    void on_addOE_clicked();

    void on_MODOE_clicked();

    void on_SupprOE_clicked();

    void on_RECHOE_clicked();

    void on_TRIOE2_2_clicked();

    void on_TRIOE2_clicked();

    void on_PdfSA_clicked();

    void on_TRISA_clicked();

    void on_qrcodegen_clicked();

    void on_AddC_clicked();

    void on_ModCa_clicked();

    void on_RECHca_clicked();

    void on_SupprCa_clicked();

    void on_TriCAnom_clicked();

    void on_Tri2CA_clicked();

    void on_TriIDca_clicked();

    void on_PDFCA_clicked();

    void on_qrcodegen1_clicked();

    void populateOfferComboBox();
//mailing
    void browse();
    void sendMail();
    void mailSent(QString status);

//finmailing
private:
    Ui::MainWindow *ui;
    OffreEmploi ptmp;
    Candidat ptmp2;
    QStringList files;


};
#endif // MAINWINDOW_H
