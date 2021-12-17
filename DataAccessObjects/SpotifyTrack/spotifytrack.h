#ifndef SPOTIFYTRACK_H
#define SPOTIFYTRACK_H
#include <QString>
#include "../abstractdaoobject.h"
class SpotifyTrack : public AbstractDAOObject
{
private:

    void setupSpotifyTrack(const QString & name, const QString & artists,const QString & id, const QString & url)
    {
        map_["name"] = name;
        map_["artists"] = artists;
        map_["id"] = id;
        map_["url"] = url;
    }
    virtual void fromJsonObject(const QJsonObject & jsonObject) override
    {
        QString name = jsonObject.value("name").toString();
        QString id = jsonObject.value("id").toString();
        QString preview_url = jsonObject.value("preview_url").toString();
        QString artists;

        const auto jsonArrayArtists = jsonObject.value("artists").toArray();

        foreach (const QJsonValue & value, jsonArrayArtists){
            const auto artist = value.toObject();
            artists += artist.value("name").toString() + " ";
        }
        setupSpotifyTrack(name,artists,id,preview_url);
    }

public:

    SpotifyTrack() : SpotifyTrack(QString(""),QString(""),QString(""),QString("")){

    };
    SpotifyTrack(const QString & name, const QString & artists,const QString & id, const QString & url)
    {
        setupSpotifyTrack(name,artists,id,url);
    };
    SpotifyTrack(QString && name,QString && artists,QString && id, QString && url)
    {
        setupSpotifyTrack(std::move(name),std::move(artists), std::move(id), std::move(url));

    };
    SpotifyTrack(const QJsonObject & jsonObject)
    {
        fromJsonObject(jsonObject);
    }


    inline QString  name() const {return map_["name"].toString();};
    inline QString url() const {return map_["url"].toString();};
    inline QString artists() const {return map_["artists"].toString();};
    inline QString  id() const {return map_["id"].toString();};

};

#endif // SPOTIFYTRACK_H
