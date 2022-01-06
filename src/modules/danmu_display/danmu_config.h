#ifndef DANMU_CONFIG_H
#define DANMU_CONFIG_H

#include <QColor>
#include <QSettings>

class DanmuConfig : public QObject {
    Q_OBJECT

   public:
    DanmuConfig(QObject *parent = nullptr);
    ~DanmuConfig();
    void save();
    void apply();

   signals:
    void changed();

   public:
    int windowWidth;
    int windowHeight;
    int opacity;
    bool onTop;
    int fontSize;
    QColor mainColor;
    QColor usernameColor;
    QColor contentColor;
    QColor backgroundColor;

   private:
    QSettings settings;
};

#endif