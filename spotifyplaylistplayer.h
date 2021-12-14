#ifndef SPOTIFYPLAYLISTPLAYER_H
#define SPOTIFYPLAYLISTPLAYER_H

#include <QWidget>
#include <memory>
#include "spotifywrapper.h"

QT_BEGIN_NAMESPACE
namespace Ui { class SpotifyPlayListPlayer; }
QT_END_NAMESPACE

class SpotifyPlayListPlayer : public QWidget
{
    Q_OBJECT

public:
    SpotifyPlayListPlayer(QWidget *parent = nullptr);
    ~SpotifyPlayListPlayer();

private slots:
    void on_pushButtonGrant_clicked();

    void on_lineEditTrack_returnPressed();

private:
    SpotifyWrapper spotifyWrapper;
    Ui::SpotifyPlayListPlayer *ui;
};
#endif // SPOTIFYPLAYLISTPLAYER_H
