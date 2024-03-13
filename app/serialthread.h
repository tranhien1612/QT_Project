
#ifndef SERIALTHREAD_H
#define SERIALTHREAD_H


#include <QThread.h>

class SerialThread : public QThread
{
    Q_OBJECT
public:
    explicit SerialThread(QObject* parent = 0);

protected:
    void startSerial();
};

#endif // SERIALTHREAD_H
