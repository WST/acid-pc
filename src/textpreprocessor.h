#ifndef TEXTPREPROCESSOR_H
#define TEXTPREPROCESSOR_H

#include <QObject>

class TextPreprocessor : public QObject
{
    Q_OBJECT
public:
    explicit TextPreprocessor(QObject *parent, QString shared_prefix, QString emoticon_set);
    
signals:
    
public slots:
    void changeEmoticonSet(const QString &emoticon_pack);
};

#endif // TEXTPREPROCESSOR_H
