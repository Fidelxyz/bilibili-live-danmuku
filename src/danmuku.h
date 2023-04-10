#ifndef DANMUKU_DANMUKU_H_
#define DANMUKU_DANMUKU_H_

#include <QMainWindow>
#include <QVBoxLayout>
#include <unordered_map>

#include "module.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class Danmuku;
}
QT_END_NAMESPACE

class Danmuku : public QMainWindow {
    Q_OBJECT

   public:
    Danmuku(QWidget* parent = nullptr);
    ~Danmuku();
    Module* getModule(const QString& name) const;
    void loadModule(Module* module);
    void unloadModule(Module* module);

   private:
    Ui::Danmuku* ui;
    QVBoxLayout* layout_modules;
    std::unordered_map<QString, Module*> modules;
};

#endif  // DANMUKU_DANMUKU_H_