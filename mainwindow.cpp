#include "mainwindow.h"
#include "qsqlerror.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include "qpainter.h"
#include "qpdfwriter.h"
#include <QTextTableFormat>
#include <QtPrintSupport/QPrintDialog>
#include <QtPrintSupport/QPrinter>
#include "qcustomplot.h"
#include "QrCode.h"
#include <QListWidget>
#include <QHBoxLayout>
#include <QSqlRecord>
#include "smtp.h"

#include <QPushButton>
using namespace qrcodegen;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{

    ui->setupUi(this);
    populateOfferComboBox();
    MainWindow::makePlot(); //stats
    connect(ui->sendBtn, SIGNAL(clicked()),this, SLOT(sendMail()));
    connect(ui->browseBtn, SIGNAL(clicked()), this, SLOT(browse()));
    connect(ui->sendBtn, SIGNAL(clicked()),this, SLOT(sendMail()));
    connect(ui->browseBtn, SIGNAL(clicked()), this, SLOT(browse()));

}

MainWindow::~MainWindow()
{
    delete ui;
}



void MainWindow::on_addOE_clicked()
{
QString titre, contrat, lieu, statut;
float salaire;
int ide;

titre = ui->titre->text().trimmed();
contrat = ui->contrat->currentText().trimmed(); // ComboBox for contract type
lieu = ui->Lieu->text().trimmed();
statut = ui->stts->currentText().trimmed(); // ComboBox for status
salaire = ui->salaire->text().toFloat();
ide = ui->idof->text().toInt();

OffreEmploi offre(ide, titre, contrat, lieu, salaire, statut);

if (titre.isEmpty() || contrat.isEmpty() || lieu.isEmpty() || statut.isEmpty()) {
    QMessageBox::critical(0, qApp->tr("ERREUR"), qApp->tr("Veuillez remplir les champs vides."), QMessageBox::Cancel);
} else {

    bool test = offre.ajouter();
    if (test) {
        ui->tableView->setModel(ptmp.afficher()); // Assuming 'ptmp' is an instance of a model class
        QMessageBox::information(nullptr, QObject::tr("Enregistrer"), tr("Ajout Effectué Avec Succès"));
        makePlot();
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Erreur!"), tr("Ajout Non Effectué!"));
    }
}

}



void MainWindow::on_MODOE_clicked()
{
    QString titre, contrat, lieu, statut;
    float salaire;
    int ide;

    titre = ui->titre->text().trimmed();
    contrat = ui->contrat->currentText().trimmed();
    lieu = ui->Lieu->text().trimmed();
    statut = ui->stts->currentText().trimmed();
    salaire = ui->salaire->text().toFloat();
    ide = ui->idof->text().toInt();

    OffreEmploi offre(ide, titre, contrat, lieu, salaire, statut);

    if (!offre.exists(ide)) {
        QMessageBox::critical(nullptr, QObject::tr("Erreur!"), tr("L'ID spécifié n'existe pas!"));
        return;
    }

    if (titre.isEmpty() || contrat.isEmpty() || lieu.isEmpty() || statut.isEmpty()) {
        QMessageBox::critical(0, qApp->tr("ERREUR"), qApp->tr("Veuillez remplir les champs vides."), QMessageBox::Cancel);
    } else {
        bool test = offre.mettreAJour(ide);  // Assuming 'mettreAJour' is the method for updating an offer

        if (test) {
            ui->tableView->setModel(ptmp.afficher()); // Assuming 'ptmp' is an instance of a model class
            QMessageBox::information(nullptr, QObject::tr("Enregistrer"), tr("Modification effectuée avec succès"));
            makePlot();
        } else {
            QMessageBox::critical(nullptr, QObject::tr("Erreur!"), tr("Modification non effectuée!"));
        }
    }
}



void MainWindow::on_SupprOE_clicked()
{
    int ide = ui->supproe->text().toInt();

    if (!ptmp.exists(ide)) {
        QMessageBox::critical(nullptr, QObject::tr("Erreur!"), tr("L'ID spécifié n'existe pas!"));
        return;
    }

    bool test = ptmp.supprimer(ide);

    if (test) {
        ui->tableView->setModel(ptmp.afficher());
        QMessageBox::information(nullptr, QObject::tr("Success"), tr("Suppression effectuée avec succès!"));
    makePlot();
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Erreur!"), tr("Suppression non effectuée!"));
    }
}



void MainWindow::on_RECHOE_clicked()
{

    QString titre = ui->titlerech->text().trimmed();
    QString lieu = ui->lrech->text().trimmed();// pourque la valeur arrive a la base de donnees sans dechets type varchar20
    QString statut = ui->strech->text().trimmed();

    QSqlQueryModel* model = nullptr;

    if (!titre.isEmpty())
    {
        model = ptmp.searchByTitre(titre);
    }
    else if (!lieu.isEmpty())
    {
        model = ptmp.searchByLieu(lieu);
    }
    else if (!statut.isEmpty())
    {
        model = ptmp.searchByStatut(statut);
    }

    if (model)
    {
        ui->tableView->setModel(model);
    }
    else
    {
        qDebug() << "No search criteria provided.";
    }
}


void MainWindow::on_TRIOE2_2_clicked()
{
   OffreEmploi e;
    ui->tableView->setModel(ptmp.trierParSalaireAsc());
}


void MainWindow::on_TRIOE2_clicked()
{
    OffreEmploi e;
    ui->tableView->setModel(ptmp.trierParSalaireDesc());
}

void MainWindow::on_TRISA_clicked()
{

    OffreEmploi e;
    ui->tableView->setModel(ptmp.trierParID());
}
/*
─────▄───▄
─▄█▄─█▀█▀█─▄█▄
▀▀████▄█▄████▀▀
─────▀█▀█▀

  */

void MainWindow::on_PdfSA_clicked()
{
    QPdfWriter pdf("mypdf.pdf");
    QPainter painter(&pdf);

    // POSITIONS
    int i = 4000; // position de texte
    painter.setPen(Qt::darkBlue);
    painter.setFont(QFont("Arial", 25, QFont::Bold));
    int pageWidth = pdf.width();

    // centrer le titre
    int textWidth = painter.fontMetrics().width("Liste Des Offres d'Emploi");
    int xPosition = (pageWidth - textWidth) / 2;
    painter.drawText(xPosition, 1400, "Liste Des Offres d'Emploi");

    // titre
    painter.setPen(Qt::black);
    painter.setBrush(QBrush(Qt::lightGray)); // Background color for title
    painter.drawRect(100, 100, 9400, 2500); // Outer border

    // ligne
    painter.setBrush(QBrush(Qt::darkBlue)); // Background color for headers
    painter.drawRect(100, 3000, 9400, 500); // Header row

    // texte des titres
    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 9, QFont::Bold));
    painter.drawText(400, 3300, "IDE");
    painter.drawText(1000, 3300, "Titre");
    painter.drawText(2200, 3300, "Contrat");
    painter.drawText(3900, 3300, "Lieu");
    painter.drawText(5200, 3300, "Salaire");
    painter.drawText(7000, 3300, "Statut");

    // border des lignes
    painter.setPen(Qt::black);
    painter.setBrush(Qt::NoBrush); // No fill for row borders
    painter.drawRect(100, 3000, 9400, 9000);

    // retrieve
    QSqlQuery query;
    query.prepare("SELECT * FROM offre_emploi");
    if (!query.exec())
    {
        qDebug() << "Error executing query:" << query.lastError().text();
        return;
    }

    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 9));
    while (query.next())
    {
        painter.drawText(400, i, query.value("IDE").toString());
        painter.drawText(1000, i, query.value("TITRE").toString());
        painter.drawText(2200, i, query.value("CONTRAT").toString());
        painter.drawText(3900, i, query.value("LIEU").toString());
        painter.drawText(5200, i, QString::number(query.value("SALAIRE").toFloat(), 'f', 2)); // Format float with 2 decimal places
        painter.drawText(7000, i, query.value("STATUT").toString());

        i += 350;
    }

    QMessageBox::information(this, QObject::tr("PDF Enregistré!"),
                             QObject::tr("PDF Enregistré!.\n" "Click Cancel to exit."), QMessageBox::Cancel);

}

/*
─────▄───▄
─▄█▄─█▀█▀█─▄█▄
▀▀████▄█▄████▀▀
─────▀█▀█▀

  */

void MainWindow::on_qrcodegen_clicked()
{
    int row = ui->tableView->currentIndex().row();
    QVariant idVariant = ui->tableView->model()->data(ui->tableView->model()->index(row, 0));
    int ide = idVariant.toInt(); // Convert QVariant to int

    QSqlQuery qry;
    qry.prepare("SELECT * FROM offre_emploi WHERE IDE = :ide");
    qry.bindValue(":ide", ide);
    qry.exec();

    QString titre, contrat, lieu, statut;
    float salaire;

    if (qry.next())
    {
        titre = qry.value("TITRE").toString();
        contrat = qry.value("CONTRAT").toString();
        lieu = qry.value("LIEU").toString();
        salaire = qry.value("SALAIRE").toFloat();
        statut = qry.value("STATUT").toString();
    }
    else
    {
        QMessageBox::warning(this, "Error", "No data found for the selected job offer.");
        return;
    }

    // Prepare the text for the QR code
    QString info = QString("IDE:\t%1\nTITRE:\t%2\nCONTRAT:\t%3\nLIEU:\t%4\nSALAIRE:\t%5\nSTATUT:\t%6")
                       .arg(ide)
                       .arg(titre)
                       .arg(contrat)
                       .arg(lieu)
                       .arg(salaire)
                       .arg(statut);

    // Generate the QR code
    QrCode qr = QrCode::encodeText(info.toUtf8().constData(), QrCode::Ecc::HIGH);

    // Create an image to hold the QR code
    QImage im(qr.getSize(), qr.getSize(), QImage::Format_RGB888);
    for (int y = 0; y < qr.getSize(); y++) {
        for (int x = 0; x < qr.getSize(); x++) {
            int color = qr.getModule(x, y);  // 0 for white, 1 for black
            if (color == 0)
                im.setPixel(x, y, qRgb(255, 255, 255)); // White
            else
                im.setPixel(x, y, qRgb(0, 0, 0)); // Black
        }
    }
    im = im.scaled(200, 200);
    ui->qrcodecommande->setPixmap(QPixmap::fromImage(im));

}
/*
 * #################################
 * #################################
 * ######GESTION DES CANDIDATS######
 * #################################
 * #################################
 * */

/*
 * get titre from id
 * */

void MainWindow::populateOfferComboBox()
{
    ui->ofr->clear();
    QSqlQuery query("SELECT IDE, TITRE FROM offre_emploi");

    while (query.next())
    {
        int id = query.value("IDE").toInt();
        QString titre = query.value("TITRE").toString();
        ui->ofr->addItem(titre, id);
    }}
/*
 * basic crud
 * */

void MainWindow::on_AddC_clicked()
{

 int idc = ui->idc->text().toInt();
    QString nom = ui->nom->text().trimmed();
    QString prenom = ui->prnm->text().trimmed();
    QString email = ui->email->text().trimmed();
    QString telephone = ui->tlf->text().trimmed();
    int offreId = ui->ofr->currentData().toInt();

    Candidat candidat(idc, nom, prenom, email, telephone, offreId);

    if (nom.isEmpty() || prenom.isEmpty() || email.isEmpty() || telephone.isEmpty())
    {
        QMessageBox::critical(0, qApp->tr("ERREUR"), qApp->tr("Veuillez remplir les champs vides."), QMessageBox::Cancel);
    }
    else
    {
        bool test = candidat.ajouter();
        if (test)
        {
            ui->tableView_1->setModel(candidat.afficher()); // Refresh the view
            QMessageBox::information(nullptr, QObject::tr("Enregistrer"), tr("Ajout Effectué Avec Succès"));
makePlot();
        }
        else
        {
            QMessageBox::critical(nullptr, QObject::tr("Erreur!"), tr("Ajout Non Effectué!"));
        }
    }
}

void MainWindow::on_ModCa_clicked(){
    int idc = ui->idc->text().toInt();
    QString nom = ui->nom->text().trimmed();
    QString prenom = ui->prnm->text().trimmed();
    QString email = ui->email->text().trimmed();
    QString telephone = ui->tlf->text().trimmed();
    int offreId = ui->ofr->currentData().toInt(); // Get selected offer ID

    Candidat candidat(idc, nom, prenom, email, telephone, offreId);

    if (!candidat.exists(idc)) {
        QMessageBox::critical(nullptr, QObject::tr("Erreur!"), tr("L'ID spécifié n'existe pas!"));
        return;
    }

    if (nom.isEmpty() || prenom.isEmpty() || email.isEmpty() || telephone.isEmpty())
    {
        QMessageBox::critical(0, qApp->tr("ERREUR"), qApp->tr("Veuillez remplir les champs vides."), QMessageBox::Cancel);
    }
    else
    {
        bool test = candidat.mettreAJour(idc);
        if (test)
        {
            ui->tableView_1->setModel(candidat.afficher());
            QMessageBox::information(nullptr, QObject::tr("Enregistrer"), tr("Modification effectuée avec succès"));
            makePlot();
      }
        else
        {
            QMessageBox::critical(nullptr, QObject::tr("Erreur!"), tr("Modification non effectuée!"));
        }
    }
}


void MainWindow::on_RECHca_clicked()
{

    QString nom = ui->nomsrc->text().trimmed();
    QString prenom = ui->presrc->text().trimmed();
    QString email = ui->emsrc->text().trimmed();
    QSqlQueryModel* model = nullptr;
    Candidat candidat;

    if (!nom.isEmpty())
    {
        model = candidat.chercherParNom(nom);
    }
    else if (!prenom.isEmpty())
    {
        model = candidat.chercherParPrenom(prenom);
    }
    else if (!email.isEmpty())
    {
        model = candidat.chercherParEmail(email);
    }

    if (model)
    {
        ui->tableView_1->setModel(model);
    }
    else
    {
        QMessageBox::warning(this, "No Results", "No results found for the search criteria.");
    }
}


void MainWindow::on_SupprCa_clicked()
{
    int idc = ui->supprCa->text().toInt();

    if (!ptmp2.exists(idc)) {
        // If the idc doesn't exist, show an error message
        QMessageBox::critical(nullptr, QObject::tr("Erreur!"), tr("L'ID spécifié n'existe pas!"));
        return;
    }

    bool test = ptmp2.supprimer(idc);

    if (test) {
        ui->tableView_1->setModel(ptmp2.afficher());
        QMessageBox::information(nullptr, QObject::tr("Success"), tr("Suppression effectuée avec succès!"));
                                 makePlot();
    } else {
        QMessageBox::critical(nullptr, QObject::tr("Erreur!"), tr("Suppression non effectuée!"));
    }
}


void MainWindow::on_PDFCA_clicked()
{
 QPdfWriter pdf("candidats_report.pdf");
    QPainter painter(&pdf);

    int y = 4000;
    painter.setPen(Qt::darkBlue);
    painter.setFont(QFont("Arial", 25, QFont::Bold));
    int pageWidth = pdf.width();

    int textWidth = painter.fontMetrics().width("Liste Des Candidats");
    int xPosition = (pageWidth - textWidth) / 2;
    painter.drawText(xPosition, 1400, "Liste Des Candidats");

    painter.setPen(Qt::black);
    painter.setBrush(QBrush(Qt::lightGray));
    painter.drawRect(100, 100, 9400, 2500);

    painter.setBrush(QBrush(Qt::darkBlue));
    painter.drawRect(100, 3000, 9400, 500);

    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 9, QFont::Bold));
    painter.drawText(400, 3300, "IDC");
    painter.drawText(1000, 3300, "Nom");
    painter.drawText(2200, 3300, "Prénom");
    painter.drawText(3900, 3300, "Email");
    painter.drawText(5200, 3300, "Téléphone");
    painter.drawText(7000, 3300, "Offre ID");

    painter.setPen(Qt::black);
    painter.setBrush(Qt::NoBrush);
    painter.drawRect(100, 3000, 9400, 9000);

    QSqlQuery query;
    query.prepare("SELECT * FROM candidat");
    if (!query.exec())
    {
        qDebug() << "Error executing query:" << query.lastError().text();
        return;
    }

    painter.setPen(Qt::black);
    painter.setFont(QFont("Arial", 9));
    while (query.next())
    {
        painter.drawText(400, y, query.value("IDC").toString());
        painter.drawText(1000, y, query.value("NOM").toString());
        painter.drawText(2200, y, query.value("PRENOM").toString());
        painter.drawText(3900, y, query.value("EMAIL").toString());
        painter.drawText(5200, y, query.value("TELEPHONE").toString());
        painter.drawText(7000, y, query.value("OFFREID").toString());

        y += 350; // Move down for the next row
    }

    QMessageBox::information(this, QObject::tr("PDF Enregistré!"),
                             QObject::tr("PDF Enregistré!.\n" "Click Cancel to exit."), QMessageBox::Cancel);

    painter.end();

}

void MainWindow::on_TriCAnom_clicked()
{
    QSqlQueryModel* model = Candidat().afficher();
    model->setQuery("SELECT * FROM candidat ORDER BY NOM ASC");
    ui->tableView_1->setModel(model);
}

void MainWindow::on_Tri2CA_clicked()
{
    QSqlQueryModel* model = Candidat().afficher();
    model->setQuery("SELECT * FROM candidat ORDER BY NOM DESC");
    ui->tableView_1->setModel(model);
}

void MainWindow::on_TriIDca_clicked()
{
    QSqlQueryModel* model = Candidat().afficher();
    model->setQuery("SELECT * FROM candidat ORDER BY IDC ASC");
    ui->tableView_1->setModel(model);
}
void MainWindow::on_qrcodegen1_clicked()
{
    int row = ui->tableView_1->currentIndex().row();
    QVariant idVariant = ui->tableView_1->model()->data(ui->tableView_1->model()->index(row, 0));
    int idc = idVariant.toInt(); // Convert QVariant to int

    QSqlQuery qry;
    qry.prepare("SELECT * FROM candidat WHERE IDC = :idc");
    qry.bindValue(":idc", idc);
    qry.exec();

    QString nom, prenom, email, telephone;
    int offreId;

    if (qry.next())
    {
        nom = qry.value("NOM").toString();
        prenom = qry.value("PRENOM").toString();
        email = qry.value("EMAIL").toString();
        telephone = qry.value("TELEPHONE").toString();
        offreId = qry.value("OFFREID").toInt();
    }
    else
    {
        QMessageBox::warning(this, "Error", "No data found for the selected candidate.");
        return;
    }

    QString info = QString("IDC:\t%1\nNOM:\t%2\nPRENOM:\t%3\nEMAIL:\t%4\nTELEPHONE:\t%5\nOFFREID:\t%6")
                       .arg(idc)
                       .arg(nom)
                       .arg(prenom)
                       .arg(email)
                       .arg(telephone)
                       .arg(offreId);

    QrCode qr = QrCode::encodeText(info.toUtf8().constData(), QrCode::Ecc::HIGH);

    QImage im(qr.getSize(), qr.getSize(), QImage::Format_RGB888);
    for (int y = 0; y < qr.getSize(); y++) {
        for (int x = 0; x < qr.getSize(); x++) {
            int color = qr.getModule(x, y);  // 0 for white, 1 for black
            if (color == 0)
                im.setPixel(x, y, qRgb(255, 255, 255)); // White
            else
                im.setPixel(x, y, qRgb(0, 0, 0)); // Black
        }
    }
    im = im.scaled(200, 200); // Resize QR code image
    ui->qrcodecommande2->setPixmap(QPixmap::fromImage(im)); // Set the pixmap of the label
}
/*
─────▄───▄
─▄█▄─█▀█▀█─▄█▄
▀▀████▄█▄████▀▀
─────▀█▀█▀

  */
void MainWindow::makePlot() {

    QMap<int, int> offreToCandidateCountMap = ptmp2.getCandidateCountByOffreId();

    QVector<double> offreIds;
    QVector<double> candidateCounts;

    for (auto it = offreToCandidateCountMap.begin(); it != offreToCandidateCountMap.end(); ++it) {
        int offreId = it.key();
        int candidateCount = it.value();

        offreIds.push_back(static_cast<double>(offreId));
        candidateCounts.push_back(static_cast<double>(candidateCount));
    }

    if (offreIds.isEmpty() || candidateCounts.isEmpty()) {
        return;
    }

    ui->customPlot->clearGraphs();
    ui->customPlot->clearPlottables();
    ui->customPlot->clearItems();

    QCPBars *bars = new QCPBars(ui->customPlot->xAxis, ui->customPlot->yAxis);
    bars->setData(offreIds, candidateCounts);

    bars->setPen(QPen(Qt::black)); // Border color
    bars->setBrush(QColor(100, 150, 200)); // Fill color
    bars->setWidth(0.6); // Bar width

    for (int i = 0; i < offreIds.size(); ++i) {
        QCPItemText *itemText = new QCPItemText(ui->customPlot);
        itemText->setPositionAlignment(Qt::AlignCenter);
        itemText->setText(QString::number(candidateCounts[i]));
        itemText->position->setType(QCPItemPosition::ptPlotCoords);
        itemText->position->setCoords(offreIds[i], candidateCounts[i]);

        itemText->position->setCoords(offreIds[i], candidateCounts[i] + 1);

        itemText->setFont(QFont("Arial", 8));
        itemText->setColor(Qt::black);
        itemText->setPadding(QMargins(5, 5, 5, 5));
    }

    ui->customPlot->xAxis->setLabel("Offre ID");
    ui->customPlot->yAxis->setLabel("Nombre des candidats");
    ui->customPlot->xAxis->setLabelFont(QFont("Arial", 10, QFont::Bold));
    ui->customPlot->yAxis->setLabelFont(QFont("Arial", 10, QFont::Bold));
    ui->customPlot->xAxis->setTickLabelFont(QFont("Arial", 8));
    ui->customPlot->yAxis->setTickLabelFont(QFont("Arial", 8));

    if (!offreIds.isEmpty()) {
        ui->customPlot->xAxis->setRange(offreIds.first() - 0.5, offreIds.last() + 0.5);
    }

    ui->customPlot->yAxis->setRange(0, *std::max_element(candidateCounts.constBegin(), candidateCounts.constEnd()) + 1);

    ui->customPlot->xAxis->grid()->setVisible(true);
    ui->customPlot->yAxis->grid()->setVisible(true);
    ui->customPlot->xAxis->grid()->setPen(QPen(QColor(200, 200, 200), 1, Qt::DashLine));
    ui->customPlot->yAxis->grid()->setPen(QPen(QColor(200, 200, 200), 1, Qt::DashLine));

    ui->customPlot->plotLayout()->insertRow(0);
    QCPTextElement *title = new QCPTextElement(ui->customPlot, "Nombre de candidats / offre", QFont("Arial", 12, QFont::Bold));
    ui->customPlot->plotLayout()->addElement(0, 0, title);

    ui->customPlot->replot();
}

/*
─────▄───▄
─▄█▄─█▀█▀█─▄█▄
▀▀████▄█▄████▀▀
─────▀█▀█▀

  */

void  MainWindow::browse()
{
    files.clear();

    QFileDialog dialog(this);
    dialog.setDirectory(QDir::homePath());
    dialog.setFileMode(QFileDialog::ExistingFiles);

    if (dialog.exec())
        files = dialog.selectedFiles();

    QString fileListString;
    foreach(QString file, files)
        fileListString.append( "\"" + QFileInfo(file).fileName() + "\" " );

    ui->file->setText( fileListString );

}
void   MainWindow::sendMail()
{
    Smtp* smtp = new Smtp("yassine.shimi02@gmail.com",ui->mail_pass->text(), "smtp.gmail.com");
    connect(smtp, SIGNAL(status(QString)), this, SLOT(mailSent(QString)));

    if( !files.isEmpty() )
        smtp->sendMail("yassine.shimi02@gmail.com", ui->rcpt->text() , ui->subject->text(),ui->msg->toPlainText(), files );
    else
        smtp->sendMail("yassine.shimi02@gmail.com", ui->rcpt->text() , ui->subject->text(),ui->msg->toPlainText());
}
//https://myaccount.google.com/apppasswords
//tggi lvwo kiho fzxb
void   MainWindow::mailSent(QString status)
{
    if(status == "Message sent")
        QMessageBox::warning( nullptr, tr( "Qt Simple SMTP client" ), tr( "Message sent!\n\n" ) );
    ui->rcpt->clear();
    ui->subject->clear();
    ui->file->clear();
    ui->msg->clear();
    ui->mail_pass->clear();
}
