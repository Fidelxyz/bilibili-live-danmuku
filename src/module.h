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
    Module(const QString &name, const QList<QString> &dependencies = {});
    ~Module();
    QWidget *getWidget() const;

   public:
    ModuleMetadata moduleMetadata;
    static Danmuku *danmuku;

   protected:
    QWidget *widget;
    static Module *getModule(const QString &name);
};

#define MODULE

#endif