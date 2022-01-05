#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QVBoxLayout>
#include <unordered_map>

#include "module.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

   public:
    MainWindow(QWidget* parent = nullptr);
    ~MainWindow();
    Module* getModule(const QString& name) const;
    void addModule(Module* module);

   private:
    Ui::MainWindow* ui;
    QVBoxLayout* layout_modules;
    std::unordered_map<QString, Module*> modules;
};

#endif