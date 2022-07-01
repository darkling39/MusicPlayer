#ifndef PLAYER_H
#define PLAYER_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QFileDialog>
#include <string>

QT_BEGIN_NAMESPACE
namespace Ui { class Player; }
QT_END_NAMESPACE

class Player : public QMainWindow
{
    Q_OBJECT

public:
    Player(QWidget *parent = nullptr);
    ~Player();

private:
    Ui::Player *ui;
    QMediaPlayer *mPlayer;
    QMediaPlaylist *mPlayList;
    void play();
    void stop();
    void findSong();
    void trackChanged(QMediaContent song);
    void volume();
    void changePlayback();
    void changeTrack();
    void changeMaxDur();
    void traking();
    void drag();
    void shuffleList();
};
#endif // PLAYER_H
