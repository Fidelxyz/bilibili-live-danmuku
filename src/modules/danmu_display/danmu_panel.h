#ifndef DANMUKU_MODULES_DANMU_DISPLAY_DANMU_PANEL_H_
#define DANMUKU_MODULES_DANMU_DISPLAY_DANMU_PANEL_H_

#include <QWidget>

#include "danmu_config.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class DanmuPanel;
}
QT_END_NAMESPACE
class DanmuPanel final : public QWidget {
    Q_OBJECT

   public:
    explicit DanmuPanel(DanmuConfig *config, QWidget *parent = nullptr);
    ~DanmuPanel() override;

   private:
    void emitTestDanmu();
    void emitTestGift();
    void loadConfig();
    void apply() const;
    void setToDefault();
    void setFont();
    void setFontSize(const int &fontSize);
    void setMainColor();
    void setUsernameColor();
    void setContentColor();
    void setBackgroundColor();
    void toggleShowGift();
    void setShowGift(const bool &on) const;

   signals:
    void testDanmu(int uid, const QString &username, const QString &text,
                   bool isAdmin, bool isVIP, int userGuardLevel);
    void testGift(int uid, const QString &username, const QString &giftName,
                  int giftCount);

   private:
    Ui::DanmuPanel *ui;

    DanmuConfig *config;

    QFont font;
    QColor mainColor;
    QColor usernameColor;
    QColor contentColor;
    QColor backgroundColor;

    bool showGift = DEFAULT_CONFIG::showGift;
};
#endif
