#ifndef LOCALPLAYLIST_H
#define LOCALPLAYLIST_H
#include "../abstractdaoobject.h"
#include "../SpotifyTrack/spotifytrack.h"
#include <QJsonArray>
#include <QList>
class LocalPlaylist : public AbstractDAOObject
{
private:
    virtual void fromJsonObject(const QJsonObject & jsonObject)
    {
        QString name = jsonObject.value("name").toString();

        const auto jsonArrayTracks = jsonObject.value("tracks").toArray();

        map_["tracks"] = jsonArrayTracks;
    }
    void setupLocalPlaylist(QString & name, const QList<SpotifyTrack> & trackList)
    {
        QJsonArray jsonArrayTracks;

        for(auto & track:trackList)
        {
            jsonArrayTracks.append(track.toJsonObject());
        }

        map_["name"] = name;
        map_["tracks"] = jsonArrayTracks;
    }
    void setupLocalPlaylist(QString & name, const QList<SpotifyTrack> && trackList)
    {
        setupLocalPlaylist(name,std::ref(trackList));
    }
public:

    LocalPlaylist(QString & name)
    {
        setupLocalPlaylist(name,{});
    }
    LocalPlaylist(QString & name, const QList<SpotifyTrack> & trackList)
    {
        setupLocalPlaylist(name,trackList);
    }
};

#endif // LOCALPLAYLIST_H
