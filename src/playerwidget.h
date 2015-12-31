#ifndef PLAYERWIDGET_H
#define PLAYERWIDGET_H

#include <QWidget>
#include "Attachments/AudioAttachment/section.h"

namespace Ui {
class PlayerWidget;
}

class Player;
class AudioSlider;
class PlayerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PlayerWidget(QWidget *parent = 0);
    ~PlayerWidget();

    void setPlayer(Player* player);
    AudioSlider* slider() const;

public slots:
    void stop();
private slots:
    void setPlayPause(bool play);
    void seek(double pos);
    void updatePitchTempo();
    void setPitchTempo();
    void pause();
    void start();
    void updateVolume();
    void setVolume();
    void updatePosition();
    void setCurrentSection();
    void updateCurrentSection();

    void on_pushButtonReset_clicked();

protected:
    void showEvent(QShowEvent *e);
    void hideEvent(QHideEvent *e);

private:
    Ui::PlayerWidget *ui;
    Player* m_player;
};

#endif // PLAYERWIDGET_H
