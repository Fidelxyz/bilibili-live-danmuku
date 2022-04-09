#ifndef MODULE_H
#define MODULE_H

#include <QObject>
#include <QPointer>
#include <QWidget>

class Danmuku;

struct ModuleMetadata {
    QString name;
    QList<QString> dependencies;
};

class Module : public QObject {
    Q_OBJECT

   public:
    Module(const QString &name, const QList<QString> &dependencies,
           Danmuku *parent);
    ~Module();
    QWidget *getWidget() const;

   protected:
    Module *getModule(const QString &name);

   public:
    ModuleMetadata moduleMetadata;

   protected:
    QWidget *widget;
};

#define MODULE

#endif