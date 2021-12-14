#ifndef UTILS_H
#define UTILS_H
#include <QString>

namespace Constants
{
    namespace SpotifyWrapper
    {
        const QString authorizationUrlString = "https://accounts.spotify.com/authorize";
        const QString acessTokenUrlString = "https://accounts.spotify.com/api/token";
        const QString responseType = "code";
        const QString clientId = "20087250e7fb4414a94189aea4f714a4";
        const QString clientSecret = "207ce7f1040b44d39ae7d45494a8655d";
        const QString scope = "user-top-read";
        constexpr uint16_t port = 8080;
        const QString redirectUri = "http://localhost:" + QString::number(port) + "/cb/";
        const QString grantType = "authorization_code";
    }
};

#endif // UTILS_H
