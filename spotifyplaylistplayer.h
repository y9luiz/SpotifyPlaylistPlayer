#ifndef SPOTIFYPLAYLISTPLAYER_H
#define SPOTIFYPLAYLISTPLAYER_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class SpotifyPlayListPlayer; }
QT_END_NAMESPACE

class SpotifyPlayListPlayer : public QWidget
{
    Q_OBJECT

public:
    SpotifyPlayListPlayer(QWidget *parent = nullptr);
    ~SpotifyPlayListPlayer();

private:
    Ui::SpotifyPlayListPlayer *ui;
};
#endif // SPOTIFYPLAYLISTPLAYER_H
