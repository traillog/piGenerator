
#include "calcthread.h"

#include <QtWidgets>
#include <QDebug>

CalcThread::CalcThread( QObject *parent )
    : QThread( parent )
{
    jobId = 1;
    iniTerm = 1;
    endTerm = 1;
    jobResult = 0.0;
    workerID = 0;

    pause = false;
    restart = false;
    abort = false;
}

CalcThread::~CalcThread()
{
    mutex.lock();
    abort = true;
    condition.wakeOne();
    mutex.unlock();

    wait();
}

void CalcThread::setWorkerId( quint32 workerID )
{
    this->workerID = workerID;
}

void CalcThread::startCalc( quint32 jobId, quint64 iniTerm, quint64 endTerm )
{
    QMutexLocker locker( &mutex );

    this->jobId = jobId;
    this->iniTerm = iniTerm;
    this->endTerm = endTerm;
    this->jobResult = 0.0;

    abort = false;

    if ( !isRunning() )
    {
        start();
    }
    else
    {
        restart = true;
        condition.wakeOne();
    }
}

void CalcThread::run()
{
    forever
    {
        mutex.lock();
        quint32 jobId = this->jobId;
        quint64 iniTerm = this->iniTerm;
        quint64 endTerm = this->endTerm;
        qreal jobResult = 0.0;
        quint32 workerID = this->workerID;
        mutex.unlock();

        int s = 1;                              // Current term's sign

        // Calculate local result
        for ( quint64 n = iniTerm; n <= endTerm; n++ )
        {
            if ( abort )
                return;

            // Set up term's sign
            if ( n % 2 )
                s = 1;      // odd terms are positive
            else
                s = -1;     // even terms are negative

            // Update local result
            jobResult += s * 4 / ( qreal )( ( 2 * n ) - 1 );
        }

        // Report results
        if ( !restart )
            emit jobDone( jobId, jobResult, workerID );

        mutex.lock();
        if ( !restart )
            condition.wait( &mutex );
        restart = false;
        mutex.unlock();
    }
}

void CalcThread::pauseCalc()
{
    QMutexLocker locker( &mutex );

    pause = true;
}

void CalcThread::resumeCalc()
{
    QMutexLocker locker( &mutex );

    if ( pause )
        condition.wakeOne();

    pause = false;
}

void CalcThread::resetCalc()
{
    QMutexLocker locker( &mutex );

    abort = true;
}
