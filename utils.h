#ifndef UTILS_H
#define UTILS_H
#include <QString>
#include <QJsonDocument>
#include <QFile>
#include <QDir>
#if QT_VERSION >= QT_VERSION_CHECK(6,0,0)
    #define IS_QT6 1
#else
    #define IS_QT6 0
#endif


namespace Constants
{
    namespace SpotifyWrapper
    {
        const QString authorizationUrlString = "https://accounts.spotify.com/authorize";
        const QString acessTokenUrlString = "https://accounts.spotify.com/api/token";
        const QString responseType = "code";
        const QString clientId = "20087250e7fb4414a94189aea4f714a4";
        const QString clientSecret = "207ce7f1040b44d39ae7d45494a8655d";
        const QString scope = "user-top-read user-read-private user-read-email";
        constexpr uint16_t port = 8080;
        const QString redirectUri = "http://localhost:" + QString::number(port) + "/cb/";
        const QString grantType = "authorization_code";
    }
    namespace SpotifyPlaylistPlayer
    {
        const uint8_t playlistSize = 10;
        const uint8_t volume = 50;
    }
};


#endif // UTILS_H
