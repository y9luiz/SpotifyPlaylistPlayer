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
    QObject::connect(&spotifyWrapper, &SpotifyWrapper::authenticated, [this](){
           qDebug() << "authenticated";
           QNetworkReply *reply = spotifyWrapper.me();
           QObject::connect(reply, &QNetworkReply::finished, [=]() {
                   reply->deleteLater();
                   if (reply->error() != QNetworkReply::NoError) {
                       qDebug()<<"temos um erro";
                       qDebug() << reply->errorString();
                       return;
                   }
                   QTimer::singleShot(1000, &QCoreApplication::quit);
           });
       });

}

