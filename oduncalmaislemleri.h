//ZEHRA KUTLUGÜN
//22100011002
#ifndef ODUNCALMAISLEMLERI_H
#define ODUNCALMAISLEMLERI_H

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
class Oduncalmaislemleri;
}

class Oduncalmaislemleri : public QDialog
{
    Q_OBJECT

public:
    explicit Oduncalmaislemleri(QSqlDatabase,QWidget *parent = nullptr);
    ~Oduncalmaislemleri();


    bool uyeKitapAlmisMi(int uye_no, int kitap_no);
    bool kitapStoktaMi(int kitap_no);
public slots:
    void uyeyeTiklandi(const QModelIndex &index);
    void kitabaTiklandi(const QModelIndex &index);
private slots:
    void on_btn_oduncal_clicked();

private:
    Ui::Oduncalmaislemleri *ui;
    QSqlDatabase db=QSqlDatabase::addDatabase("SQLİTE");
    QSqlQuery *sorgu;
    QSqlQueryModel *model1,*model2,*model3;
    void listele();
};

#endif // ODUNCALMAISLEMLERI_H
