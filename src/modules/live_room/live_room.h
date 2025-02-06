#ifndef DANMUKU_MODULES_LIVE_ROOM_LIVE_ROOM_H_
#define DANMUKU_MODULES_LIVE_ROOM_LIVE_ROOM_H_

#include <QHBoxLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QThread>

#include "module.h"
#include "protocol.h"

class LiveRoom final : public Module {
    Q_OBJECT
    DANMUKU_MODULE

   public:
    explicit LiveRoom(Danmuku* parent);
    ~LiveRoom() override;

   private:
    [[nodiscard]] QJsonObject requestDanmuInfo() const;
    [[nodiscard]] int requestUid() const;
    void stop();

   signals:
    void started();
    void stopped();
    void followersCountUpdated(const int& followersCount);

   public slots:
    void start();
    [[nodiscard]] QObject* getProtocol() const;
    void updateFollowersCount();

   private:
    Protocol* protocol;
    QLineEdit* input_roomID;
    QPushButton* btn_start;

    int roomID = -1;
    int uid = -1;
    QThread protocolThread;
};

#endif  // DANMUKU_MODULES_LIVE_ROOM_LIVE_ROOM_H_