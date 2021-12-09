#ifndef DANMU_H
#define DANMU_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class Danmu;
}
QT_END_NAMESPACE

class Danmu : public QWidget {
    Q_OBJECT

   public:
    Danmu(QWidget *parent = nullptr);
    ~Danmu();

   private:
    Ui::Danmu *ui;

   private slots:
};

#endif