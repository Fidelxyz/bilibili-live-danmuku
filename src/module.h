#ifndef MODULE_H
#define MODULE_H

#include <QObject>
#include <QWidget>

struct ModuleMetadata {
    QString name;
    const QList<QString> dependency;
};

class Module : public QObject {
    Q_OBJECT

   public:
    Module(QWidget *parent);
    ~Module();
    QWidget *getWidget() const;

   public:
    static const ModuleMetadata moduleMetadata;

   protected:
    QWidget *widget;
};

#define MODULE \
   public:     \
    static const ModuleMetadata moduleMetadata;

#endif