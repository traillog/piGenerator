
#include "job.h"

Job::Job( quint32 jId, quint64 jIniTerm, quint64 jEndTerm )
    : id( jId ), iniTerm( jIniTerm ), endTerm( jEndTerm )
{
    result = 0.0;
    workerID = 0;
}

Job::~Job()
{
}

quint32 Job::getId()
{
    return id;
}

quint64 Job::getIniTerm()
{
    return iniTerm;
}

quint64 Job::getEndTerm()
{
    return endTerm;
}

qreal Job::getResult()
{
    return result;
}

quint32 Job::getWorkerID()
{
    return workerID;
}

void Job::setId( quint32 jId )
{
    id = jId;
}

void Job::setIniTerm( quint64 jIniTerm )
{
    iniTerm = jIniTerm;
}

void Job::setEndTerm( quint64 jEndTerm )
{
    endTerm = jEndTerm;
}

void Job::setResult( qreal jResult )
{
    result = jResult;
}

void Job::setWorkerID( quint32 jWorkerID )
{
    workerID = jWorkerID;
}
