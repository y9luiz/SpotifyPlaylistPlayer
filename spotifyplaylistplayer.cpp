#include "spotifyplaylistplayer.h"
#include "./ui_spotifyplaylistplayer.h"
#include "DataAccessObjects/dataaccessobject.h"
#include <QAudioOutput>
#include <QInputDialog>
#include "utils.h"
SpotifyPlayListPlayer::SpotifyPlayListPlayer(QWidget *parent)
    : QWidget(parent), spotifyWrapper_(this)
    , ui_(new Ui::SpotifyPlayListPlayer)
{
    player_ = std::make_unique<QMediaPlayer>(this);
    #if IS_QT6
        audioOutput_ = std::make_unique<QAudioOutput>();
    #endif
    ui_->setupUi(this);
    ui_->lineEditTrack->setEnabled(false);
    //
    ui_->tableWidgetTracks->setSelectionBehavior(QAbstractItemView::SelectRows);
    // put the list on horizontal
    ui_->listWidgetPlaylists->setFlow(QListView::LeftToRight);
}

SpotifyPlayListPlayer::~SpotifyPlayListPlayer()
{
    delete ui_;
}

void SpotifyPlayListPlayer::on_pushButtonGrant_clicked()
{
    spotifyWrapper_.grant();

    QObject::connect(&spotifyWrapper_, &SpotifyWrapper::userReplied, [this](SpotifyUser user){
           ui_->lineEditTrack->setEnabled(true);
           ui_->labelUserId->setText("<b>User ID: </b>" + user.id() + "\n"
                                    + "<b>E-mail: </b>"+user.email() +"</b>" + "\n"
                                    + "<b>Name: </b>" + user.name());
    });

}
void SpotifyPlayListPlayer::on_lineEditTrack_returnPressed()
{
    QString phrase = ui_->lineEditTrack->text();
    spotifyWrapper_.requestSearchTrack(phrase, Constants::SpotifyPlaylistPlayer::playlistSize );
    #if IS_QT6
        player_->setAudioOutput(audioOutput_.get());
    #endif
    QObject::connect(&spotifyWrapper_, &SpotifyWrapper::trackReplied,
                     [this](QList<SpotifyTrack> trackList){
                       fillListWidgetPlaylists(trackList);
                     }
            );
}

void SpotifyPlayListPlayer::on_pushButtonPlayMusic_clicked()
{
    auto items = ui_->tableWidgetTracks->selectedItems();

    if(!items.empty())
    {
        // get the first element of the ItemWidgetList
        // which correspond to the track preview url
        QString trackPreviewUrl = items.front()->text();
        qDebug() << "[SpotifyPlayListPlayer::on_pushButton_clicked()][INFO] starting playing music";
        player_->setSource(QUrl(trackPreviewUrl));
        #if IS_QT6
            audioOutput_->setVolume(Constants::SpotifyPlaylistPlayer::volume);
        #else
            player_->setVolume(Constants::SpotifyPlaylistPlayer::volume);
        #endif
        player_->play();
    }
}

void SpotifyPlayListPlayer::on_pushButtonNewPlaylist_clicked()
{
    // once clicked to create a new playlist, get the playlist name from user input
    bool ok;

    QString playListName = QInputDialog::getText(this, tr("New Play List"),
                                           tr("Insert the Playlist name"), QLineEdit::Normal,
                                            tr(""), &ok);
    auto end = localPlaylists_.constEnd();
    if(!localPlaylists_.contains(playListName))
    {
        localPlaylists_.insert(playListName,LocalPlaylist(playListName));
        ui_->listWidgetPlaylists->addItem(playListName);
        qDebug() << "[SpotifyPlayListPlayer::on_pushButtonNewPlaylist_clicked()][INFO] Created empty playlist with name " << playListName;
    }
    else{
        qDebug() << "[SpotifyPlayListPlayer::on_pushButtonNewPlaylist_clicked()][INFO] Playlist already exist " << playListName << " created";
    }
}



void SpotifyPlayListPlayer::on_listWidgetPlaylists_itemClicked(QListWidgetItem *item)
{

    QString playListName = item->text();
    if(localPlaylists_.contains(playListName))
    {
        auto it = localPlaylists_.find(playListName);
        currentLocalPlaylist_ = &(it.value());
        qDebug() <<"[SpotifyPlayListPlayer::on_listWidgetPlaylists_itemClicked()][INFO] Retriving Playlis with name " << it->name();
        auto trackList = it->tracks();
        fillListWidgetPlaylists(trackList);
    }
}
void SpotifyPlayListPlayer::fillListWidgetPlaylists(const QList<SpotifyTrack> & trackList)
{
    // before fill, clear the data
    ui_->tableWidgetTracks->setRowCount(0);
    ui_->tableWidgetTracks->setRowCount(trackList.size());

    int idx = 0;
    foreach(const auto & track,trackList)
    {
        ui_->tableWidgetTracks->setItem(idx,0,new QTableWidgetItem(track.id()));
        ui_->tableWidgetTracks->setItem(idx,1,new QTableWidgetItem(track.url()));
        ui_->tableWidgetTracks->setItem(idx,2,new QTableWidgetItem(track.name()));
        ui_->tableWidgetTracks->setItem(idx,3,new QTableWidgetItem(track.artists()));
        idx++;
    }
}


void SpotifyPlayListPlayer::on_pushButtonAddToPlaylist_clicked()
{
    if(currentLocalPlaylist_ != nullptr)
    {

    }
}


void SpotifyPlayListPlayer::on_tableWidgetTracks_itemClicked(QTableWidgetItem *item)
{

}


