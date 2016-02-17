#ifndef SOUNDNOTIFIER_H
#define SOUNDNOTIFIER_H

#include <QtWidgets>
//#include <QtMultimedia>

class SoundNotifier : public QObject
{
    Q_OBJECT
public:
    explicit SoundNotifier(QObject *parent, QString shared_prefix, QString sound_set);
    ~SoundNotifier();
    
signals:
    
public slots:
    void changeSoundSet(const QString &sound_set);
    void incoming();
    void outgoing();
    //void finishedPlaying(QAudio::State state);

private:
    void unload();
    QString shared_prefix;
    QString path;

    QFile *incoming_audio_file;
    QFile *outgoing_audio_file;

    //QAudioOutput* audio;
    //QAudioFormat* format;
};

#endif // SOUNDNOTIFIER_H
