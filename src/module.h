#ifndef MODULE_H
#define MODULE_H

#include <QObject>
#include <QWidget>

class MainWindow;

struct ModuleMetadata {
    QString name;
    QList<QString> dependencies;
};

class Module : public QObject {
    Q_OBJECT

   public:
    Module(const QString &name, const QList<QString> &dependencies = {});
    ~Module();
    QWidget *getWidget() const;

   public:
    ModuleMetadata moduleMetadata;
    static MainWindow *moduleManager;

   protected:
    QWidget *widget;
    static Module *getModule(const QString &name);
};

#define MODULE

#endif