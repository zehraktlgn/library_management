//ZEHRA KUTLUGÜN
//22100011002
#include "oduncalmaislemleri.h"
#include "ui_oduncalmaislemleri.h"

Oduncalmaislemleri::Oduncalmaislemleri(QSqlDatabase db,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Oduncalmaislemleri)
{
    ui->setupUi(this);

    connect(ui->tableView_tumuyeler, &QTableView::clicked, this, &Oduncalmaislemleri::uyeyeTiklandi);
    connect(ui->tableView_tumkitaplar, &QTableView::clicked, this, &Oduncalmaislemleri::kitabaTiklandi);

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("kutuphane.db");
    sorgu=new QSqlQuery(db);

    listele();
}

Oduncalmaislemleri::~Oduncalmaislemleri()
{
    delete ui;
}
void Oduncalmaislemleri::listele() {


    QSqlTableModel *model1 = new QSqlTableModel(this);
    model1->setTable("üye");
    model1->select();


    ui->tableView_tumuyeler->setModel(model1);
    ui->tableView_tumuyeler->resizeColumnsToContents();

    QSqlTableModel *model2 = new QSqlTableModel(this);
    model2->setTable("kitap");
    model2->select();


    ui->tableView_tumkitaplar->setModel(model2);
    ui->tableView_tumkitaplar->resizeColumnsToContents();



    QSqlTableModel *model3 = new QSqlTableModel(this);
    model3->setTable("odunc");
    model3->select();


    ui->tableView_oduncalkitaplistesi->setModel(model3);
    ui->tableView_oduncalkitaplistesi->resizeColumnsToContents();



}
void Oduncalmaislemleri::uyeyeTiklandi(const QModelIndex &index) {
    if (index.isValid()) {

        QVariant value = index.data();


        if (index.column() == 0) {
            ui->lineEdit_uyeno->setText(value.toString());
        }
    }
}

void Oduncalmaislemleri::kitabaTiklandi(const QModelIndex &index) {
    if (index.isValid()) {

        QVariant value = index.data();


        if (index.column() == 0) {

            ui->lineEdit_kitapno->setText(value.toString());
        }
    }
}

bool Oduncalmaislemleri::uyeKitapAlmisMi(int uye_no, int kitap_no) {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM odunc WHERE uye_no = :uye_no AND kitap_no = :kitap_no");
    query.bindValue(":uye_no", uye_no);
    query.bindValue(":kitap_no", kitap_no);
    if (query.exec() && query.next()) {
        int count = query.value(0).toInt();
        return count > 0;
    } else {
        qDebug() << "Üyenin ödünç aldığı kitaplar sorgulanırken bir hata oluştu: " << query.lastError().text();
        return false;
    }
}


bool Oduncalmaislemleri::kitapStoktaMi(int kitap_no) {
    QSqlQuery query;
    query.prepare("SELECT kitap_sayisi FROM kitap WHERE kitap_no = :kitap_no");
    query.bindValue(":kitap_no", kitap_no);

    if (query.exec() && query.next()) {
        int stok = query.value(0).toInt();
        return stok > 0;
    } else {
        qDebug() << "Kitabın stok durumu sorgulanırken bir hata oluştu: " << query.lastError().text();
        return false;
    }
}

void Oduncalmaislemleri::on_btn_oduncal_clicked()
{int uye_no = ui->lineEdit_uyeno->text().toInt();
    int kitap_no = ui->lineEdit_kitapno->text().toInt();
    QString odunc_tarihi = ui->dateEdit->date().toString("yyyy-MM-dd");

    if (uye_no == 0 || kitap_no == 0) {
        QMessageBox::warning(this, "Uyarı", "Lütfen geçerli üye ve kitap numaralarını giriniz.");
        return;
    }

    if (uyeKitapAlmisMi(uye_no, kitap_no)) {
        QMessageBox::warning(this, "Uyarı", "Bu üye zaten bu kitabı ödünç almış.");
        return;
    }

    if (!kitapStoktaMi(kitap_no)) {
        QMessageBox::warning(this, "Uyarı", "Bu kitap kütüphanede stokta değil.");
        return;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO odunc (uye_no, kitap_no, tarih) VALUES (:uye_no, :kitap_no, :odunc_tarihi)");
    query.bindValue(":uye_no", uye_no);
    query.bindValue(":kitap_no", kitap_no);
    query.bindValue(":odunc_tarihi", odunc_tarihi);

    if (query.exec()) {
        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE kitap SET kitap_sayisi = kitap_sayisi - 1 WHERE kitap_no = :kitap_no");
        updateQuery.bindValue(":kitap_no", kitap_no);

        if (updateQuery.exec()) {
            QMessageBox::information(this, "Başarılı", "Kitap ödünç alındı ve stok güncellendi.");
        } else {
            QMessageBox::warning(this, "Uyarı", "Kitap stok güncellenemedi: " + updateQuery.lastError().text());
            qDebug() << "Kitap stok güncellenirken hata oluştu: " << updateQuery.lastError().text();
        }

        listele();
        ui->tableView_oduncalkitaplistesi->resizeColumnsToContents();
    } else {
        QMessageBox::critical(this, "Hata", "Kitap ödünç alma işlemi sırasında bir hata oluştu: " + query.lastError().text());
        qDebug() << "Kitap ödünç alınırken hata oluştu: " << query.lastError().text();
    }

}

