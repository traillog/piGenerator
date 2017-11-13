
#ifndef JOB_H
#define JOB_H

#include <QWidget>

class Job
{
public:
    Job( quint32 jId, quint64 jIniTerm, quint64 jEndTerm );
    ~Job();

    quint32 getId();
    quint64 getIniTerm();
    quint64 getEndTerm();
    qreal getResult();
    quint32 getWorkerID();

    void setId( quint32 jId );
    void setIniTerm( quint64 jIniTerm );
    void setEndTerm( quint64 jEndTerm );
    void setResult( qreal jResult );
    void setWorkerID( quint32 jWorkerID );

private:
    quint32 id;
    quint64 iniTerm;
    quint64 endTerm;
    qreal result;
    quint32 workerID;
};

#endif // JOB_H
