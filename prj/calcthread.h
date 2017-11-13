
#ifndef CALCTHREAD_H
#define CALCTHREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>

class CalcThread : public QThread
{
    Q_OBJECT

public:
    CalcThread( QObject *parent = 0 );
    ~CalcThread();

    void startCalc( quint32 jobId, quint64 iniTerm, quint64 endTerm );
    void pauseCalc();
    void resumeCalc();
    void resetCalc();

    void setWorkerId( quint32 workerID );

signals:
    void jobDone( quint32 jobId, qreal jobResult, quint32 workerID );

protected:
    void run() override;

private:
    QMutex mutex;
    QWaitCondition condition;

    bool restart;
    bool pause;
    bool abort;

    quint32 jobId;
    quint64 iniTerm;
    quint64 endTerm;
    qreal jobResult;
    quint32 workerID;
};

#endif // CALCTHREAD_H
