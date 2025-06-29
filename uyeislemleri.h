//ZEHRA KUTLUGÜN
//22100011002
#ifndef UYEISLEMLERI_H
#define UYEISLEMLERI_H

#include <QDialog>
#include <QtSql/QSqlDatabase>
#include<QtSql/QSql>
#include<QtSql/QSqlQueryModel>
#include<QMessageBox>
#include<QSqlQuery>
#include<QVariant>
#include <QSqlTableModel>
#include<QSqlError>

namespace Ui {
class Uyeislemleri;
}

class Uyeislemleri : public QDialog
{
    Q_OBJECT

public:

    explicit Uyeislemleri(QSqlDatabase, QWidget *parent = nullptr);

    ~Uyeislemleri();

    void listele();
    bool uyeNoHasUnreturnedBooks(int uye_no);
public slots:
    void uyeTiklandi(const QModelIndex &index);
private slots:
    void on_btn_yenikayit_clicked();

    void on_btn_guncelle_clicked();

    void on_btn_sil_clicked();

private:
    Ui::Uyeislemleri *ui;
    QSqlDatabase db=QSqlDatabase::addDatabase("SQLİTE");
    QSqlQuery *sorgu;
    QSqlQueryModel *model1,*model2;
};

#endif // UYEISLEMLERI_H
