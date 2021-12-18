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
    LocalPlaylist(const QJsonObject & jsonObject)
    {
        fromJsonObject(jsonObject);
    }
    LocalPlaylist(QString & name)
    {
        setupLocalPlaylist(name,{});
    }
    LocalPlaylist(QString & name, const QList<SpotifyTrack> & trackList)
    {
        setupLocalPlaylist(name,trackList);
    }
    QString name() const
    {
        return map_["name"].toString();
    }
    QList<SpotifyTrack> tracks() const
    {
        QList<SpotifyTrack> tracks;
        const auto jsonTrackList = map_["tracks"].toJsonArray();
        foreach(const auto & jsonTrack,jsonTrackList)
        {            
            tracks.push_back(SpotifyTrack(jsonTrack.toObject()));
        }
        return tracks;
    }
    void append(SpotifyTrack & track)
    {
        QList<SpotifyTrack> tracks_copy;
        tracks_copy = tracks();
        tracks_copy.push_back(track);
        QString listName = name();
        setupLocalPlaylist(listName,tracks_copy);
    }
    bool contains( const SpotifyTrack & targetTrack) const
    {
        auto trackList = tracks();
        foreach(const auto track,trackList)
        {
            if(track == targetTrack)
                    return true;
        }
        return false;
    }
    operator QString() const { return map_["name"].toString() + " " + map_["tracks"].toString(); }

};

#endif // LOCALPLAYLIST_H
