#ifndef TEXTPREPROCESSOR_H
#define TEXTPREPROCESSOR_H

#include <QtCore>

class TextPreprocessor : public QObject
{
    Q_OBJECT
public:
    explicit TextPreprocessor(QObject *parent, QString _shared_prefix, QString emoticon_set);
    QString formatText(const QString &text);
    
signals:
    
public slots:
    void changeEmoticonSet(const QString &emoticon_set);

private:
    QString path;
    QString shared_prefix;
    QMap<QString, QString> replacements;
};

#endif // TEXTPREPROCESSOR_H
