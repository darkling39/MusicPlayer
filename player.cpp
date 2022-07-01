#include "player.h"
#include "ui_player.h"
#include<string>
int MODE = 1;

Player::Player(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Player)
{
    ui->setupUi(this);
    mPlayer = new QMediaPlayer();
    mPlayList = new QMediaPlaylist();
    mPlayer->setPlaylist(mPlayList);
    ui->le_skreen->setReadOnly(true);
    mPlayer->setVolume(ui->s_volume->value());
    mPlayList->setCurrentIndex(0);
    connect(ui->b_play, &QPushButton::clicked, this, &Player::play);
    connect(ui->b_find, &QPushButton::clicked, this, &Player::findSong);
    connect(ui->b_next, &QPushButton::clicked, mPlayList, &QMediaPlaylist::next);
    connect(ui->b_prev, &QPushButton::clicked, mPlayList, &QMediaPlaylist::previous);
    connect(mPlayer, &QMediaPlayer::currentMediaChanged, this, &Player::trackChanged);
    connect(ui->b_shuff, &QPushButton::clicked, mPlayList, &QMediaPlaylist::shuffle);
    connect(ui->s_volume, &QSlider::valueChanged, this, &Player::volume);
    connect(ui->b_playback, &QPushButton::clicked, this, &Player::changePlayback);
    connect(ui->lw_skreen, &QListWidget::itemDoubleClicked,this, &Player::changeTrack);
    connect(mPlayer, &QMediaPlayer::durationChanged, this, &Player::changeMaxDur);
    connect(mPlayer, &QMediaPlayer::positionChanged, this, &Player::traking);
    connect(ui->s_dur, &QSlider::sliderReleased, this, &Player::drag);
    connect(ui->b_shuff, &QPushButton::clicked, this, &Player::shuffleList);
}

Player::~Player()
{
    delete ui;
}

void Player::play()
{
    mPlayer->play();
    ui->b_play->setText("||");
    disconnect(ui->b_play, &QPushButton::clicked, this, &Player::play);
    connect(ui->b_play, &QPushButton::clicked, this, &Player::stop);

}

void Player::stop()
{
    mPlayer->pause();
    ui->b_play->setText("|>");
    disconnect(ui->b_play, &QPushButton::clicked, this, &Player::stop);
    connect(ui->b_play, &QPushButton::clicked, this, &Player::play);
}


void Player::findSong()
{
    QString song = QFileDialog::getOpenFileName(this, tr("Play Music"), QDir::homePath(),tr("music (*.mp3)"));
    if(song != "")
    {
        mPlayList->addMedia(QUrl::fromLocalFile(song));
        QString str = song;
        str = str.remove(0, str.lastIndexOf("/")+1);
        str.chop(4);
        mPlayList->setCurrentIndex(mPlayList->currentIndex()+1);

        str.prepend(". ").prepend(QString::number(mPlayList->currentIndex()+1));
        ui->lw_skreen->addItem(str);
    }
}

void Player::trackChanged(QMediaContent song)
{
    QString str = song.request().url().toString();
    str = str.remove(0, str.lastIndexOf("/")+1);
    str.chop(4);
    ui->le_skreen->setText(str);
}

void Player::volume()
{
    mPlayer->setVolume(ui->s_volume->value());
}

void Player::changePlayback()
{
    switch(MODE%5)
    {
    case 0:
        ui->b_playback->setText("CurOnce");
        break;
    case 1:
        ui->b_playback->setText("CurLoop");
        break;
    case 2:
        ui->b_playback->setText("Seq");
        break;
    case 3:
        ui->b_playback->setText("Loop");
        break;
    case 4:
        ui->b_playback->setText("Random");
        break;
    }
    mPlayList->setPlaybackMode(QMediaPlaylist::PlaybackMode(MODE%5));
    MODE++;
}

void Player::changeTrack()
{
    mPlayList->setCurrentIndex(ui->lw_skreen->currentRow());
    if(mPlayer->state() == QMediaPlayer::StoppedState || mPlayer->state() == QMediaPlayer::PausedState)
        Player::play();
}

void Player::changeMaxDur()
{
    int duration = mPlayer->duration()/1000;

    ui->s_dur->setMaximum(duration);
}

void Player::traking()
{
    int position = mPlayer->position()/1000;
    int h, m, s;

    s = position%60;
    m = position/60%60;
    h = position/3600;

    QString positionText = "";

    if (h > 0) {
        positionText.append(QString::number(h));
        positionText.append(":");

        if (m < 10) positionText.append("0");
    }

    positionText.append(QString::number(m)).append(":");

    if (s < 10) positionText.append("0");

    positionText.append(QString::number(s));

    ui->l_pos->setText(positionText);
    ui->s_dur->setValue(position);
}

void Player::drag()
{

    mPlayer->setPosition(ui->s_dur->value()*1000);
}

void Player::shuffleList()
{
    ui->lw_skreen->clear();
    mPlayList->setCurrentIndex(0);
    while(mPlayList->currentIndex() != -1)
    {
        QString str = mPlayList->media(mPlayList->currentIndex()).request().url().toString();
        str = str.remove(0, str.lastIndexOf("/")+1);
        str.chop(4);
        str.prepend(". ").prepend(QString::number(mPlayList->currentIndex()+1));
        ui->lw_skreen->addItem(str);
        mPlayList->setCurrentIndex(mPlayList->currentIndex()+1);
    }
    mPlayList->setCurrentIndex(0);
    Player::play();
}





