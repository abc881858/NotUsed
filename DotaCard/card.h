#ifndef CARD_H
#define CARD_H

#include <QObject>

class Card : public QObject {
    Q_OBJECT
    Q_ENUMS(Primary)

public:
    explicit Card(QObject* parent = 0, int value = -1);
    QString description;

    int getISDN() const;
    void setISDN(int value);

private:
    int ISDN;

signals:

public slots:
};

#endif // CARD_H
