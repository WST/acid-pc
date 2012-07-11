#include "textpreprocessor.h"

TextPreprocessor::TextPreprocessor(QObject *parent, QString _shared_prefix, QString emoticon_set): QObject(parent) {
    shared_prefix = _shared_prefix;
    changeEmoticonSet(emoticon_set);
}

void TextPreprocessor::changeEmoticonSet(const QString &emoticon_set) {
    path = shared_prefix + "/emoticons/" + emoticon_set;

    //
}

QString TextPreprocessor::formatText(const QString &text) {
    /// TODO
}
