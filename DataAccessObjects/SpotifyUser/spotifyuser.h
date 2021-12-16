#ifndef SPOTIFYUSER_H
#define SPOTIFYUSER_H

#include <QString>
struct SpotifyUser
{
    QString name_;
    QString email_;
    QString id_;
    SpotifyUser(QString && name, QString && email, QString && id):
        name_(name), email_(email), id_(id)
    {
    }
};

#endif // SPOTIFYUSER_H
