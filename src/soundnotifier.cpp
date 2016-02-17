#include "soundnotifier.h"

SoundNotifier::SoundNotifier(QObject *parent, QString shared_prefix, QString sound_set): QObject(parent) {
/*
    this->shared_prefix = shared_prefix;
    incoming_audio_file = outgoing_audio_file = 0;
    changeSoundSet(sound_set);

    QAudioFormat format;
    format.setSampleRate(22050);
    format.setChannelCount(1);
    format.setSampleSize(32);
    format.setCodec("audio/pcm");
    format.setByteOrder(QAudioFormat::LittleEndian);
    format.setSampleType(QAudioFormat::Float);

    audio = new QAudioOutput(format, this);
    connect(audio,SIGNAL(stateChanged(QAudio::State)),SLOT(finishedPlaying(QAudio::State)));
*/
}

SoundNotifier::~SoundNotifier() {
/*
    unload();
    delete audio;
*/
}

void SoundNotifier::unload() {
/*
    if(incoming_audio_file) incoming_audio_file->close();
    if(outgoing_audio_file) outgoing_audio_file->close();

    delete incoming_audio_file;
    delete outgoing_audio_file;
*/
}

void SoundNotifier::changeSoundSet(const QString &sound_set) {
/*
    path = shared_prefix + "/sounds/" + sound_set;

    unload();

    incoming_audio_file = new QFile(path + "/incoming.wav");
    outgoing_audio_file = new QFile(path + "/outgoing.wav");
    incoming_audio_file->open(QIODevice::ReadOnly);
    outgoing_audio_file->open(QIODevice::ReadOnly);
*/
}

void SoundNotifier::incoming() {
/*
    audio->start(incoming_audio_file);
    incoming_audio_file->seek(0);
*/
}

void SoundNotifier::outgoing() {
/*
    audio->start(outgoing_audio_file);
    outgoing_audio_file->seek(0);
*/
}

//void SoundNotifier::finishedPlaying(QAudio::State state) {
    //if(state == QAudio::IdleState) audio->stop();
//}
