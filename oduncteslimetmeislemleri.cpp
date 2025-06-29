//ZEHRA KUTLUGÜN
//22100011002
#include "oduncteslimetmeislemleri.h"
#include "ui_oduncteslimetmeislemleri.h"

Oduncteslimetmeislemleri::Oduncteslimetmeislemleri(QSqlDatabase db,QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Oduncteslimetmeislemleri)
{
    ui->setupUi(this);
    // Constructor içinde
    connect(ui->tableView_oduncalinankitaplar, &QTableView::clicked, this, &Oduncteslimetmeislemleri::oduncAlinanKitabaTiklandi);

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("kutuphane.db");


    sorgu=new QSqlQuery(db);
    listele();
}

Oduncteslimetmeislemleri::~Oduncteslimetmeislemleri()
{
    delete ui;
}
void Oduncteslimetmeislemleri::listele() {
    QSqlTableModel *model3 = new QSqlTableModel(this);
    model3->setTable("odunc");
    model3->select();


    ui->tableView_oduncalinankitaplar->setModel(model3);
    ui->tableView_oduncalinankitaplar->resizeColumnsToContents();

}

void Oduncteslimetmeislemleri::listele2() {
    QSqlTableModel *model4 = new QSqlTableModel(this);
    model4->setTable("deneme");
    model4->select();


    ui->tableView_teslimedilenkitaplar->setModel(model4);
    ui->tableView_teslimedilenkitaplar->resizeColumnsToContents();
}
void Oduncteslimetmeislemleri::oduncAlinanKitabaTiklandi(const QModelIndex &index) {
    if (index.isValid()) {
        QSqlTableModel *model = qobject_cast<QSqlTableModel *>(ui->tableView_oduncalinankitaplar->model());
        if (model) {

            QVariant kitapNo = model->index(index.row(), model->fieldIndex("kitap_no")).data();
            QVariant uyeNo = model->index(index.row(), model->fieldIndex("uye_no")).data();


            ui->lineEdit_kitapno->setText(kitapNo.toString());
            ui->lineEdit_uyeno->setText(uyeNo.toString());
        }
    }
}
void Oduncteslimetmeislemleri::on_btn_oduncuver_clicked()
{
    int uye_no = ui->lineEdit_uyeno->text().toInt();
    int kitap_no = ui->lineEdit_kitapno->text().toInt();
    QString alma_tarihi;

    // Ödünç alınan kitabın alma tarihini al
    QSqlQuery queryAlmaTarih;
    queryAlmaTarih.prepare("SELECT tarih FROM odunc WHERE uye_no = :uye_no AND kitap_no = :kitap_no");
    queryAlmaTarih.bindValue(":uye_no", uye_no);
    queryAlmaTarih.bindValue(":kitap_no", kitap_no);
    if (queryAlmaTarih.exec() && queryAlmaTarih.next()) {
        alma_tarihi = queryAlmaTarih.value(0).toString();
    } else {
        QMessageBox::critical(this, tr("Hata"), "Ödünç alınan kitabın alma tarihi bulunamadı.");
        return;
    }

    QString verme_tarihi = ui->dateEdit->date().toString("yyyy-MM-dd");

    if (uye_no == 0 || kitap_no == 0 || verme_tarihi.isEmpty()) {
        QMessageBox::warning(this, "Uyarı", "Lütfen geçerli üye ve kitap numaralarını ve ödünç verme tarihini giriniz.");
        return;
    }

    // Borcu hesapla
    QDate almaTarihi = QDate::fromString(alma_tarihi, "yyyy-MM-dd");
    QDate vermeTarihi = QDate::fromString(verme_tarihi, "yyyy-MM-dd");
    int daysOverdue = almaTarihi.daysTo(vermeTarihi) - 15;
    int borc = (daysOverdue > 0) ? daysOverdue * 2 : 0;

    QSqlQuery query;
    query.prepare("INSERT INTO deneme (uye_no, kitap_no, alma_tarihi, verme_tarihi, borc) VALUES (:uye_no, :kitap_no, :alma_tarihi, :verme_tarihi, :borc)");
    query.bindValue(":uye_no", uye_no);
    query.bindValue(":kitap_no", kitap_no);
    query.bindValue(":alma_tarihi", alma_tarihi);
    query.bindValue(":verme_tarihi", verme_tarihi);
    query.bindValue(":borc", borc);

    if (query.exec()) {
        // Kitap stoğunu artır
        QSqlQuery updateQuery;
        updateQuery.prepare("UPDATE kitap SET kitap_sayisi = kitap_sayisi + 1 WHERE kitap_no = :kitap_no");
        updateQuery.bindValue(":kitap_no", kitap_no);
        if (!updateQuery.exec()) {
            QMessageBox::critical(this, tr("Hata"), updateQuery.lastError().text());
            return;
        }


        QSqlQuery deleteQuery;
        deleteQuery.prepare("DELETE FROM odunc WHERE uye_no = :uye_no AND kitap_no = :kitap_no");
        deleteQuery.bindValue(":uye_no", uye_no);
        deleteQuery.bindValue(":kitap_no", kitap_no);
        if (!deleteQuery.exec()) {
            QMessageBox::critical(this, tr("Hata"), deleteQuery.lastError().text());
            return;
        }

        QMessageBox::information(this, "Başarılı", "Kitap teslim edildi.");
        listele();
        listele2();
        ui->tableView_oduncalinankitaplar->resizeColumnsToContents();
        ui->tableView_teslimedilenkitaplar->resizeColumnsToContents();

    } else {
        QMessageBox::critical(this, "Hata", query.lastError().text());
        qDebug() << "Kitap teslim edilirken hata oluştu: " << query.lastError().text();
    }


}

