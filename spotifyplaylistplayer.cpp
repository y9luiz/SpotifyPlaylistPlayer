#include "spotifyplaylistplayer.h"
#include "./ui_spotifyplaylistplayer.h"
#include "DataAccessObjects/dataaccessobject.h"
#include <QAudioOutput>
#include <QInputDialog>
#include <memory>
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
    currentTrack_ = nullptr;
    currentTrackList_.clear();

    QString phrase = ui_->lineEditTrack->text();
    spotifyWrapper_.requestSearchTrack(phrase, Constants::SpotifyPlaylistPlayer::playlistSize );
    #if IS_QT6
        player_->setAudioOutput(audioOutput_.get());
    #endif
    QObject::connect(&spotifyWrapper_, &SpotifyWrapper::trackReplied,
                     [this](QList<SpotifyTrack> trackList){
                        currentTrackList_ = trackList;
                        fillListWidgetPlaylists(trackList);
                     }
            );
}

void SpotifyPlayListPlayer::on_pushButtonPlayMusic_clicked()
{
    auto items = ui_->tableWidgetTracks->selectedItems();

    if(!items.empty())
    {
        // get the second element of the ItemWidgetList
        // which correspond to the track preview url
        QString trackPreviewUrl = items.value(1)->text();
        qDebug() << "[SpotifyPlayListPlayer::on_pushButton_clicked()][INFO] starting playing music";
        #if IS_QT6
            qDebug() << "wish";
            player_->setSource(QUrl(trackPreviewUrl));
            audioOutput_->setVolume(Constants::SpotifyPlaylistPlayer::volume);
        #else
            qDebug() << trackPreviewUrl;
            player_->setMedia(QUrl(trackPreviewUrl));
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
        qDebug() <<"[SpotifyPlayListPlayer::on_listWidgetPlaylists_itemClicked()][INFO] Retriving Playlist with name " << it->name();
        auto trackList = it->tracks();
        fillListWidgetPlaylists(trackList);
    }
}
void SpotifyPlayListPlayer::fillListWidgetPlaylists(const QList<SpotifyTrack> & trackList)
{
    // before fill, clear the data
    ui_->tableWidgetTracks->setRowCount(0);
    ui_->tableWidgetTracks->setRowCount(trackList.size());

    int rowIdx = 0;
    foreach(const auto & track,trackList)
    {
        int colIdx = 0;
        qDebug() << "[SpotifyPlayListPlayer::fillListWidgetPlaylists][INFO] adding track "<< track << " to tableWidgetTracks";
        ui_->tableWidgetTracks->setItem(rowIdx,colIdx++,new QTableWidgetItem(track.id()));
        ui_->tableWidgetTracks->setItem(rowIdx,colIdx++,new QTableWidgetItem(track.url()));
        ui_->tableWidgetTracks->setItem(rowIdx,colIdx++,new QTableWidgetItem(track.name()));
        ui_->tableWidgetTracks->setItem(rowIdx,colIdx,new QTableWidgetItem(track.artists()));
        rowIdx++;
    }
}


void SpotifyPlayListPlayer::on_pushButtonAddToPlaylist_clicked()
{
    if(currentLocalPlaylist_ != nullptr && currentTrack_ != nullptr)
    {
        qDebug() << "[SpotifyPlayListPlayer::::on_pushButtonAddToPlaylist_clicked()][INFO] track added to playlist " << *currentTrack_;
        currentLocalPlaylist_->append(*currentTrack_);
    }
}


void SpotifyPlayListPlayer::on_tableWidgetTracks_itemClicked(QTableWidgetItem *item)
{
    auto items = ui_->tableWidgetTracks->selectedItems();
    QString id = items.front()->text();

    for(auto & track:currentTrackList_)
    {
        if(track.id() == id)
        {
            currentTrack_ = &track;
        }
    }
}
