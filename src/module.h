#ifndef DANMUKU_MODULE_H_
#define DANMUKU_MODULE_H_

#include <QObject>
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
    ~Module() override;
    [[nodiscard]] QWidget *getWidget() const;

   protected:
    [[nodiscard]] Module *getModule(const QString &name) const;

   public:
    ModuleMetadata moduleMetadata;

   protected:
    QWidget *widget;
};

#define DANMUKU_MODULE

#endif  // DANMUKU_MODULE_H_