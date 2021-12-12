#include "spotifyplaylistplayer.h"
#include "./ui_spotifyplaylistplayer.h"

SpotifyPlayListPlayer::SpotifyPlayListPlayer(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::SpotifyPlayListPlayer)
{
    ui->setupUi(this);
}

SpotifyPlayListPlayer::~SpotifyPlayListPlayer()
{
    delete ui;
}

