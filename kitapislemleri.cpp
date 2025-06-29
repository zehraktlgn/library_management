//ZEHRA KUTLUGÜN
//22100011002
#include "kitapislemleri.h"
#include "ui_kitapislemleri.h"

Kitapislemleri::Kitapislemleri(QSqlDatabase db,QWidget *parent)

    : QDialog(parent)
    , ui(new Ui::Kitapislemleri)
{
    ui->setupUi(this);


    connect(ui->tableView_tumkitaplar, &QTableView::clicked, this, &Kitapislemleri::kitabaTiklandi);

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("kutuphane.db");
    sorgu=new QSqlQuery(db);

    listele();
}

Kitapislemleri::~Kitapislemleri()
{
    delete ui;
}
void Kitapislemleri::listele() {

    QSqlTableModel *model1 = new QSqlTableModel(this);
    model1->setTable("kitap");
    model1->select();


    ui->tableView_tumkitaplar->setModel(model1);
    ui->tableView_tumkitaplar->resizeColumnsToContents();




}

void Kitapislemleri::kitabaTiklandi(const QModelIndex &index)
{
    if (index.isValid()) {

        QString kitapNo = index.model()->data(index.model()->index(index.row(), 0)).toString();
        QString kitapAdi = index.model()->data(index.model()->index(index.row(), 1)).toString();
        QString stok = index.model()->data(index.model()->index(index.row(), 2)).toString();


        ui->lineEdit_kitapno->setText(kitapNo);
        ui->lineEdit_kitapad->setText(kitapAdi);
        ui->lineEdit_kitapstok->setText(stok);


        QSqlQuery oduncSorgu(db);
        oduncSorgu.prepare("SELECT * FROM odunc WHERE kitap_no = ?");
        oduncSorgu.addBindValue(kitapNo.toInt());
        if (oduncSorgu.exec()) {

            QSqlTableModel *oduncModel = new QSqlTableModel(this);
            oduncModel->setTable("odunc");
            oduncModel->setFilter(QString("kitap_no = %1").arg(kitapNo.toInt()));
            oduncModel->select();


            ui->tableView_oduncalmadurumu->setModel(oduncModel);
            ui->tableView_oduncalmadurumu->resizeColumnsToContents();
        } else {
            QMessageBox::critical(this, "Hata", "Ödünç alınma durumu kontrol edilemedi: " + oduncSorgu.lastError().text());
        }


        QSqlQuery teslimSorgu(db);
        teslimSorgu.prepare("SELECT * FROM deneme WHERE kitap_no = ?");
        teslimSorgu.addBindValue(kitapNo.toInt());
        if (teslimSorgu.exec()) {

            QSqlTableModel *teslimModel = new QSqlTableModel(this);
            teslimModel->setTable("deneme");
            teslimModel->setFilter(QString("kitap_no = %1").arg(kitapNo.toInt()));
            teslimModel->select();


            ui->tableView_dahaoncedenoduncalmadurumu->setModel(teslimModel);
            ui->tableView_dahaoncedenoduncalmadurumu->resizeColumnsToContents();
        } else {
            QMessageBox::critical(this, "Hata", "Teslim edilme durumu kontrol edilemedi: " + teslimSorgu.lastError().text());
        }
    }

}


void Kitapislemleri::on_btn_yenikayit_clicked()
{
    if( ui->lineEdit_kitapad->text()=="" || ui->lineEdit_kitapstok->text()==""){
        QMessageBox::critical(this, "HATA", "Eksik Bilgi Girdiniz");
    }
    else {
        sorgu->prepare("INSERT INTO kitap(kitap_ad,kitap_sayisi) VALUES(?, ?)");


        sorgu->addBindValue(ui->lineEdit_kitapad->text());
        sorgu->addBindValue(ui->lineEdit_kitapstok->text().toInt());


        if (!sorgu->exec()) {
            QMessageBox::critical(this, "Hata", "Yeni kitap eklenemedi: " + sorgu->lastError().text());
            return;
        }

        listele();
    }
}


void Kitapislemleri::on_btn_guncelle_clicked()
{

    sorgu->prepare("UPDATE kitap SET kitap_ad=?, kitap_sayisi=? WHERE kitap_no=?");


    sorgu->addBindValue(ui->lineEdit_kitapad->text());
    sorgu->addBindValue(ui->lineEdit_kitapstok->text().toInt());
    sorgu->addBindValue(ui->lineEdit_kitapno->text().toInt());


    if (!sorgu->exec()) {

        QMessageBox::critical(this, "Hata", "Güncellenemedi: " + sorgu->lastError().text());
        return;
    }


    listele();
}


void Kitapislemleri::on_btn_sil_clicked()
{
    int kitapNo = ui->lineEdit_kitapno->text().toInt();


    QSqlQuery oduncKontrolSorgu(db);
    oduncKontrolSorgu.prepare("SELECT COUNT(*) FROM odunc WHERE kitap_no = ?");
    oduncKontrolSorgu.addBindValue(kitapNo);
    if (oduncKontrolSorgu.exec() && oduncKontrolSorgu.next()) {
        int count = oduncKontrolSorgu.value(0).toInt();
        if (count > 0) {
            QMessageBox::warning(this, "Uyarı", "Bu kitap şu anda ödünç verilmiş durumda. Silinemez.");
            return;
        }
    } else {
        QMessageBox::critical(this, "Hata", "Ödünç kontrolü sırasında bir hata oluştu: " + oduncKontrolSorgu.lastError().text());
        return;
    }


    sorgu->prepare("DELETE FROM kitap WHERE kitap_no = ?");
    sorgu->addBindValue(kitapNo);

    if (!sorgu->exec()) {
        QMessageBox::critical(this, "Hata", "Silinemedi: " + sorgu->lastError().text());
        return;
    }
    listele();

}
