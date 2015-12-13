#ifndef PLAYERWIDGET_H
#define PLAYERWIDGET_H

#include <QWidget>
#include "Attachments/AudioAttachment/section.h"

namespace Ui {
class PlayerWidget;
}

class Player;
class PlayerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlayerWidget(QWidget *parent = 0);
    ~PlayerWidget();

    void setPlayer(Player* player);

private slots:
    void setSection(Section section);
    void stop();
    void setPlayPause(bool play);
    void seek(double pos);
    void updatePitchTempo();
    void setPitchTempo();
    void pause();
    void start();
    void updateVolume();
    void setVolume();
    void updatePosition();

private:
    Ui::PlayerWidget *ui;
    Player* m_player;
};

#endif // PLAYERWIDGET_H
