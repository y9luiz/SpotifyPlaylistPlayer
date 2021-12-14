#include "spotifyplaylistplayer.h"
#include "./ui_spotifyplaylistplayer.h"

SpotifyPlayListPlayer::SpotifyPlayListPlayer(QWidget *parent)
    : QWidget(parent), spotifyWrapper(this)
    , ui(new Ui::SpotifyPlayListPlayer)
{
    ui->setupUi(this);
    ui->lineEditTrack->setEnabled(false);
}

SpotifyPlayListPlayer::~SpotifyPlayListPlayer()
{
    delete ui;
}

void SpotifyPlayListPlayer::on_pushButtonGrant_clicked()
{
    spotifyWrapper.grant();

    QObject::connect(&spotifyWrapper, &SpotifyWrapper::userIdReplied, [this](){
           ui->lineEditTrack->setEnabled(true);
           ui->labelUserId->setText("<b>User ID:</b>" + spotifyWrapper.getUserId());
    });

}
void SpotifyPlayListPlayer::on_lineEditTrack_returnPressed()
{
    qDebug() <<"[SpotifyPlayListPlayer::on_lineEditTrack_returnPressed()][INFO] Searching for playlist";
    spotifyWrapper.searchTrack(ui->lineEditTrack->text(),10);
}

