#include "spotifyplaylistplayer.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SpotifyPlayListPlayer w;
    w.show();
    return a.exec();
}
