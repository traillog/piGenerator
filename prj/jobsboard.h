
#ifndef JOBSBOARD_H
#define JOBSBOARD_H

#include <QWidget>

class JobsBoard : public QWidget
{
public:
    JobsBoard( QWidget *parent = 0 );

    QSize minimumSizeHint() const override;
    QSize sizeHint() const override;

    void setWorkersCount( const qint32& wC );
    void setResultTxt( const QString& rsltTxt );
    void setTimeTxt( const QString &valueTxt );
    void setJobStatus( qint32 jobId, qint32 workerId );
    void setJobsCountWrk( qint32 wkId, const QString& wkBalance );


protected:
    void paintEvent( QPaintEvent *event ) override;

private:
    qint32 workersCount;            // Workers count
    QString resultText;             // Accumulated result
    QString elapsedTimeTxt;         // Elapsed time
    QList< qint32 > jobsStatus;     // Which worker did which job
    QList< QString > jobsCountWrk;  // Work done per worker (work balance)
    QList< QColor > colorMap;       // Color table

    void initJobsStatus();
    void initJobsCountWrk();
    void initColorMap();
};

#endif // JOBSBOARD_H
