#ifndef LOCALPLAYLISTFILEMANAGER_H
#define LOCALPLAYLISTFILEMANAGER_H

#include <QObject>
#include <QList>
#include <QString>
#include <QJsonDocument>
#include <QStringList>
#include <QDir>
#include "../DataAccessObjects/LocalPlaylist/localplaylist.h"
class LocalPlaylistFileManager : public QObject
{
    Q_OBJECT
public:
    explicit LocalPlaylistFileManager(QString directoryName,QObject *parent = nullptr);
    QList<LocalPlaylist> loadLocalPlaylistsFromDisk() const;
    void saveLocalPlaylistToDisk(const LocalPlaylist & localPlaylist) const;
private:
    void createDirectory() const;
    QJsonDocument loadJsonFile(QString fileName) const;
    void saveJsonFile(QJsonDocument document, QString fileName) const;
    QStringList listJsonFilesInDirectory() const;
    QString directoryName_;
    QDir dir_;
signals:

};

#endif // LOCALPLAYLISTFILEMANAGER_H
