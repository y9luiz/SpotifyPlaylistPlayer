#include "spotifyplaylistplayer.h"
#include "./ui_spotifyplaylistplayer.h"

SpotifyPlayListPlayer::SpotifyPlayListPlayer(QWidget *parent)
    : QWidget(parent), spotifyWrapper(this)
    , ui(new Ui::SpotifyPlayListPlayer)
{
    ui->setupUi(this);

}

SpotifyPlayListPlayer::~SpotifyPlayListPlayer()
{
    delete ui;
}


void SpotifyPlayListPlayer::on_pushButtonGrant_clicked()
{
    spotifyWrapper.grant();
}

