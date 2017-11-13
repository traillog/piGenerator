
#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QQueue>
#include <QTime>

#include "job.h"
#include "jobsboard.h"
#include "calcthread.h"

QT_BEGIN_NAMESPACE
class QGroupBox;
class QSpinBox;
class QLabel;
class QSlider;
class QPushButton;
QT_END_NAMESPACE

class Window : public QWidget
{
    Q_OBJECT

public:
    Window();
    ~Window();

    enum CalcState { IDLE, RUNNING, STOPPED };

private slots:
    void workersCountChanged( int value );
    void setTermsPerJob( int value );
    void setCurJobIdBrowser( int value );
    void startStopPressed();
    void resetPressed();
    void resetResults();
    void logResult( quint32 jobId, qreal jobResult, quint32 workerID );
    void updateTimeDiff();

private:
    //==============================
    // GUI
    //==============================
    // Main blocks
    JobsBoard *jobsBoard;
    QGroupBox *jobConfigGroup;
    QGroupBox *jobBrowserGroup;
    QPushButton *startStopButton;
    QPushButton *resetButton;

    // Config group
    QLabel *workersLabel;
    QSpinBox *workersSpinBox;
    QLabel *totalTermsLabel;
    QLabel *totalTermsValue;
    QLabel *termsPerJobLabel;
    QLabel *termsPerJobValue;
    QSlider *termsPerJobSlider;
    
    // Job browser group
    QLabel *jobIdLabel;
    QLabel *jobIdValue;
    QLabel *jobIniTermLabel;
    QLabel *jobIniTermValue;
    QLabel *jobEndTermLabel;
    QLabel *jobEndTermValue;
    QLabel *jobResultLabel;
    QLabel *jobResultValue;
    QLabel *jobWorkerLabel;
    QLabel *jobWorkerValue;
    QLabel *curJobLabel;
    QSlider *curJobSlider;

    //==============================
    // Internal vars
    //==============================
    QQueue< Job* > jobs;            // Jobs vector

    quint32 workersCount;           // Amount of workers
    CalcThread* workers;            // Array of workers

    CalcState calcState;            // Current calculation state
    quint32 curNotAssignedJob;      // Last unassigned job
    quint32 receivedResults;        // Received resutls
    quint32 curJobBrowser;          // Job displayed in browser

    quint64 totalTerms;             // Total terms
    quint64 termsPerJob;            // Terms per job
    qreal totalResult;              // Accumulated result
    QList< quint32 > jobsCountWrk;  // Jobs done per worker

    bool resettingResults;

    //==============================
    // Timekeeping vars
    //==============================
    QTimer *timer;

    QTime startTime;
    qint64 elapsedMsLastStart;
    qint64 elapsedMsLastStop;

    quint32 mins;       // minutes
    quint32 secs;       // seconds
    quint32 cents;      // centiseconds

    //==============================
    // Auxiliary functions
    //==============================
    void setWorkersCount( const qint32 &wC );
    void setupWorkers( const quint32 &wC );
    void setupControls();
    void setupTimer();
    void createJobs();
    void destroyJobs();
    void resetJobsCountWrk();
    void showJob( int jobID );
    void updateTimeDisp();
};

#endif
