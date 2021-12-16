#ifndef SPOTIFYPLAYLISTPLAYER_H
#define SPOTIFYPLAYLISTPLAYER_H

#include <QWidget>
#include <memory>
#include <QMediaPlayer>
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

    void on_pushButton_clicked();

    void on_tableWidgetTracks_cellClicked(int row, int column);

private:
    SpotifyWrapper spotifyWrapper;
    Ui::SpotifyPlayListPlayer *ui;
    std::shared_ptr<QMediaPlayer> player_;
    std::shared_ptr<QAudioOutput> audioOutput_;
};
#endif // SPOTIFYPLAYLISTPLAYER_H
