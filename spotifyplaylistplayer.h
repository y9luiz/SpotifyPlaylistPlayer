#ifndef SPOTIFYPLAYLISTPLAYER_H
#define SPOTIFYPLAYLISTPLAYER_H

#include <QWidget>
#include <memory>
#include <QMediaPlayer>
#include <QMap>
#include <QListWidget>
#include <QListWidgetItem>
#include "spotifywrapper.h"

#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
#define IS_QT6 1
#else
#define IS_QT6 0
#endif



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

    void on_pushButtonNewPlaylist_clicked();

    void on_listWidgetPlaylists_itemClicked(QListWidgetItem *item);

    void fillListWidgetPlaylists(const QList<SpotifyTrack> & tracks);

private:
    SpotifyWrapper spotifyWrapper;
    Ui::SpotifyPlayListPlayer *ui;
    std::shared_ptr<QMediaPlayer> player_;
#if IS_QT6
    std::shared_ptr<QAudioOutput> audioOutput_;
#endif
    QMap<QString,LocalPlaylist> localPlaylists_;
};
#endif // SPOTIFYPLAYLISTPLAYER_H
