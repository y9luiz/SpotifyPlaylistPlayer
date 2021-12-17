#include "spotifyplaylistplayer.h"
#include "./ui_spotifyplaylistplayer.h"
#include "DataAccessObjects/dataaccessobject.h"
#include <QAudioOutput>
#include <QInputDialog>
SpotifyPlayListPlayer::SpotifyPlayListPlayer(QWidget *parent)
    : QWidget(parent), spotifyWrapper(this)
    , ui(new Ui::SpotifyPlayListPlayer)
{
    player_ = std::make_shared<QMediaPlayer>(this);
    #if IS_QT6
        audioOutput_ = std::make_shared<QAudioOutput>();
    #endif
    ui->setupUi(this);
    ui->lineEditTrack->setEnabled(false);
    ui->tableWidgetTracks->setSelectionBehavior(QAbstractItemView::SelectRows);
}

SpotifyPlayListPlayer::~SpotifyPlayListPlayer()
{
    delete ui;
}

void SpotifyPlayListPlayer::on_pushButtonGrant_clicked()
{
    spotifyWrapper.grant();

    QObject::connect(&spotifyWrapper, &SpotifyWrapper::userReplied, [this](SpotifyUser user){
           ui->lineEditTrack->setEnabled(true);
           ui->labelUserId->setText("<b>User ID: </b>" + user.id() + "\n"
                                    + "<b>E-mail: </b>"+user.email() +"</b>" + "\n"
                                    + "<b>Name: </b>" + user.name());
    });

}
void SpotifyPlayListPlayer::on_lineEditTrack_returnPressed()
{
    QString phrase = ui->lineEditTrack->text();
    spotifyWrapper.requestSearchTrack(phrase,10);
    ui->tableWidgetTracks->setRowCount(10);
    #if IS_QT6
        player_->setAudioOutput(audioOutput_.get());
    #endif
    QObject::connect(&spotifyWrapper, &SpotifyWrapper::trackReplied,
                     [this](QList<SpotifyTrack> tracksList){
                        uint8_t idx = 0;
                        foreach(const auto & track,tracksList)
                        {
                            ui->tableWidgetTracks->setItem(idx,0,new QTableWidgetItem(track.url()));
                            ui->tableWidgetTracks->setItem(idx,1,new QTableWidgetItem(track.name()));
                            ui->tableWidgetTracks->setItem(idx,2,new QTableWidgetItem(track.artists()));
                            idx++;
                        }
                     }
            );
}


void SpotifyPlayListPlayer::on_pushButton_clicked()
{
    auto items = ui->tableWidgetTracks->selectedItems();

    if(!items.empty())
    {
        // get the first element of the ItemWidgetList
        // which correspond to the track preview url
        QString trackPreviewUrl = items.front()->text();
        qDebug() << "[SpotifyPlayListPlayer::on_pushButton_clicked()][INFO] starting playing music";
        player_->setSource(QUrl(trackPreviewUrl));
        #if IS_QT6
            audioOutput_->setVolume(50);
        #else
            player_->setVolume(50);
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
    localPlaylists_.push_back(LocalPlaylist(playListName));
    qDebug() << "[SpotifyPlayListPlayer::on_pushButtonNewPlaylist_clicked()][INFO] Empty playlist with name " << playListName << " created";

}

