#ifndef SPOTIFYPLAYLISTPLAYER_H
#define SPOTIFYPLAYLISTPLAYER_H

#include <QWidget>
#include <memory>
#include <QMediaPlayer>
#include <QMap>
#include <QListWidget>
#include <QListWidgetItem>
#include <QTableWidgetItem>
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

    void on_pushButtonNewPlaylist_clicked();

    void on_listWidgetPlaylists_itemClicked(QListWidgetItem *item);

    void fillListWidgetPlaylists(const QList<SpotifyTrack> & tracks);

    void on_pushButtonAddToPlaylist_clicked();

    void on_tableWidgetTracks_itemClicked(QTableWidgetItem *item);

    void on_pushButtonPlayMusic_clicked();

private:
    SpotifyWrapper spotifyWrapper;
    Ui::SpotifyPlayListPlayer *ui;
    std::unique_ptr<QMediaPlayer> player_;
#if IS_QT6
    std::unique_ptr<QAudioOutput> audioOutput_;
#endif
    QMap<QString,LocalPlaylist> localPlaylists_;
    // store a reference to current selected playlist
    LocalPlaylist * currentLocalPlaylist = nullptr;
    // store a reference to current selected track
    SpotifyTrack * currentTrack = nullptr;
};
#endif // SPOTIFYPLAYLISTPLAYER_H
