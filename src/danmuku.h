#ifndef DANMUKU_DANMUKU_H_
#define DANMUKU_DANMUKU_H_

#include <QMainWindow>
#include <QVBoxLayout>
#include <unordered_map>

#include "module.h"

class Danmuku final : public QMainWindow {
    Q_OBJECT

   public:
    explicit Danmuku(QWidget *parent = nullptr);
    ~Danmuku() override;
    [[nodiscard]] Module *getModule(const QString &name) const;
    void loadModule(Module *module);
    void unloadModule(const Module *module);

   private:
    QWidget *centralWidget;
    QVBoxLayout *layout_modules;
    std::unordered_map<QString, Module *> modules;
};

#endif  // DANMUKU_DANMUKU_H_