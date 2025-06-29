//ZEHRA KUTLUGÜN
//22100011002
#include "uyeislemleri.h"
#include "ui_uyeislemleri.h"



Uyeislemleri::Uyeislemleri(QSqlDatabase db,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Uyeislemleri)
{
    ui->setupUi(this);
    connect(ui->tableView_tumuyeler, &QTableView::clicked, this, &Uyeislemleri::uyeTiklandi);

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("kutuphane.db");


    sorgu=new QSqlQuery(db);
    listele();
}

Uyeislemleri::~Uyeislemleri()
{
    delete ui;
}
void Uyeislemleri::listele() {

    // QSqlTableModel oluşturup tabloyu ayarlıyoruz
    QSqlTableModel *model1 = new QSqlTableModel(this);
    model1->setTable("üye");
    model1->select();


    ui->tableView_tumuyeler->setModel(model1);
    ui->tableView_tumuyeler->resizeColumnsToContents();



}

void Uyeislemleri::on_btn_yenikayit_clicked()
{
    if(ui->lineEdit_uyead->text()==""||ui->lineEdit_uyesoyad->text()==""){
        QMessageBox::critical(this,"HATA","Eksik Bilgi Girdiniz");
    }
    else{
        sorgu->prepare("INSERT INTO üye(uye_ad, uye_soyad) VALUES(?, ?)");

        sorgu->addBindValue(ui->lineEdit_uyead->text());
        sorgu->addBindValue(ui->lineEdit_uyesoyad->text());

        if (!sorgu->exec()) {

            QMessageBox::critical(this, "Hata", "Yeni üye eklenemedi: " + sorgu->lastError().text());
            return;
        }
    }
    listele();

}


void Uyeislemleri::on_btn_guncelle_clicked()
{


    sorgu->prepare("UPDATE üye SET uye_ad=?, uye_soyad=? WHERE uye_no=?");


    sorgu->addBindValue(ui->lineEdit_uyead->text());
    sorgu->addBindValue(ui->lineEdit_uyesoyad->text());
    sorgu->addBindValue(ui->lineEdit_uyeno->text().toInt());


    if (!sorgu->exec()) {
        QMessageBox::critical(this, "Hata", "Güncellenemedi: " + sorgu->lastError().text());
        return;
    }

    listele();
}


void Uyeislemleri::on_btn_sil_clicked()
{int uye_no = ui->lineEdit_uyeno->text().toInt();

    if (uyeNoHasUnreturnedBooks(uye_no)) {
        QMessageBox::critical(this, "Hata", "Üyenin ödünç aldığı kitaplar mevcut. Üye silinemedi.");
        return;
    }

    sorgu->prepare("DELETE FROM üye WHERE uye_no = ?");
    sorgu->addBindValue(uye_no);

    if (!sorgu->exec()) {
        QMessageBox::critical(this, "Hata", "Silinemedi: " + sorgu->lastError().text());
        return;
    }

    listele();

}
bool Uyeislemleri::uyeNoHasUnreturnedBooks(int uye_no)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM odunc WHERE uye_no = ?");
    query.addBindValue(uye_no);

    if (!query.exec() || !query.next()) {
        qDebug() << "Error checking unreturned books: " << query.lastError().text();
        return true;
    }

    int count = query.value(0).toInt();
    return count > 0;
}
void Uyeislemleri::uyeTiklandi(const QModelIndex &index)
{
    if (index.isValid()) {

        QString uyeNo = index.model()->data(index.model()->index(index.row(), 0)).toString();
        QString uyeAdi = index.model()->data(index.model()->index(index.row(), 1)).toString();
        QString uyeSoyadi = index.model()->data(index.model()->index(index.row(), 2)).toString();


        ui->lineEdit_uyeno->setText(uyeNo);
        ui->lineEdit_uyead->setText(uyeAdi);
        ui->lineEdit_uyesoyad->setText(uyeSoyadi);
    }
}
