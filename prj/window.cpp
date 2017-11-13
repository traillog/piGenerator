
#include <QtWidgets>
#include <QDebug>

#include "window.h"

Window::Window()
{
    // Calculation initial state
    calcState = IDLE;
    workersCount = 1;
    curNotAssignedJob = 1;
    totalResult = 0.0;
    receivedResults = 0;

    // Timekeeping initial state
    elapsedMsLastStart = 0;
    elapsedMsLastStop = 0;
    mins = secs = cents = 0;
    setupTimer();

    // Set up workers
    workers = NULL;
    setupWorkers( workersCount );

    // Set up GUI
    setupControls();
    setTermsPerJob( 1 );
    setCurJobIdBrowser( 1 );

    setWindowTitle( tr( "Pi Generator" ) );
}

Window::~Window()
{
    // Destroy workers
    delete[] workers;

    // Destroy jobs
    destroyJobs();
}

void Window::setupControls()
{
    jobsBoard = new JobsBoard;

    jobConfigGroup = new QGroupBox( tr( "Job Config" ) );

    workersLabel = new QLabel( tr( "Workers:" ) );
    workersSpinBox = new QSpinBox;
    workersSpinBox->setRange( 1, 4 );
    workersSpinBox->setSingleStep( 1 );
    workersSpinBox->setValue( 1 );
    workersSpinBox->setMaximumWidth( 50 );

    connect( workersSpinBox, SIGNAL( valueChanged( int ) ),
             this, SLOT( workersCountChanged( int ) ) );

    totalTermsLabel = new QLabel( tr( "Total Terms:" ) );
    totalTermsValue = new QLabel( tr( "" ) );
    totalTermsValue->setAlignment( Qt::AlignRight | Qt::AlignCenter );

    termsPerJobLabel = new QLabel( tr( "Terms Per Job:" ) );
    termsPerJobValue = new QLabel( tr( "" ) );
    termsPerJobValue->setAlignment( Qt::AlignRight | Qt::AlignCenter );

    termsPerJobSlider = new QSlider( Qt::Horizontal );
    termsPerJobSlider->setFocusPolicy( Qt::StrongFocus );
    termsPerJobSlider->setTickPosition( QSlider::TicksBelow );
    termsPerJobSlider->setMinimum( 1 );
    termsPerJobSlider->setMaximum( 100 );
    termsPerJobSlider->setSingleStep( 1 );
    termsPerJobSlider->setTickInterval( 10 );
    termsPerJobSlider->setValue( 1 );
    termsPerJobSlider->setFocus();
    termsPerJobLabel->setBuddy( termsPerJobSlider );

    connect( termsPerJobSlider, SIGNAL( valueChanged( int ) ),
             this, SLOT( setTermsPerJob( int ) ) );

    QGridLayout *configGridLayout = new QGridLayout;
    configGridLayout->addWidget( workersLabel, 0, 0 );
    configGridLayout->addWidget( workersSpinBox, 0, 1, Qt::AlignRight );
    configGridLayout->addWidget( totalTermsLabel, 1, 0 );
    configGridLayout->addWidget( totalTermsValue, 1, 1 );
    configGridLayout->addWidget( termsPerJobLabel, 2, 0 );
    configGridLayout->addWidget( termsPerJobValue, 2, 1 );
    configGridLayout->addWidget( termsPerJobSlider, 3, 1 );
    configGridLayout->setRowMinimumHeight( 1, 20 );
    configGridLayout->setRowMinimumHeight( 2, 20 );
    configGridLayout->setRowMinimumHeight( 3, 30 );

    QVBoxLayout *configVGroup = new QVBoxLayout;
    configVGroup->addLayout( configGridLayout );
    configVGroup->addStretch();

    jobConfigGroup->setLayout( configVGroup );


    jobBrowserGroup = new QGroupBox( tr( "Job Browser" ) );

    jobIdLabel = new QLabel( tr( "Job ID:" ) );
    jobIdValue = new QLabel( tr( "" ) );
    jobIdValue->setAlignment( Qt::AlignRight | Qt::AlignCenter );

    jobIniTermLabel = new QLabel( tr( "Initial Term:" ) );
    jobIniTermValue = new QLabel( tr( "" ) );
    jobIniTermValue->setAlignment( Qt::AlignRight | Qt::AlignCenter );

    jobEndTermLabel = new QLabel( tr( "Final Term:" ) );
    jobEndTermValue = new QLabel( tr( "" ) );
    jobEndTermValue->setAlignment( Qt::AlignRight | Qt::AlignCenter );

    jobResultLabel = new QLabel( tr( "Result:" ) );
    jobResultValue = new QLabel( tr( "" ) );
    jobResultValue->setAlignment( Qt::AlignRight | Qt::AlignCenter );

    jobWorkerLabel = new QLabel( tr( "Worker ID:" ) );
    jobWorkerValue = new QLabel( tr( "" ) );
    jobWorkerValue->setAlignment( Qt::AlignRight | Qt::AlignCenter );

    curJobLabel = new QLabel( tr( "Current Job:" ) );

    curJobSlider = new QSlider( Qt::Horizontal );
    curJobSlider->setFocusPolicy( Qt::StrongFocus );
    curJobSlider->setTickPosition( QSlider::TicksBelow );
    curJobSlider->setMinimum( 1 );
    curJobSlider->setMaximum( 100 );
    curJobSlider->setSingleStep( 1 );
    curJobSlider->setTickInterval( 10 );
    curJobSlider->setValue( 1 );
    curJobLabel->setBuddy( curJobSlider );

    connect( curJobSlider, SIGNAL( valueChanged( int ) ),
             this, SLOT( setCurJobIdBrowser( int ) ) );

    QGridLayout *browserGridLayout = new QGridLayout;
    browserGridLayout->addWidget( jobIdLabel, 0, 0 );
    browserGridLayout->addWidget( jobIdValue, 0, 1 );
    browserGridLayout->addWidget( jobIniTermLabel, 1, 0 );
    browserGridLayout->addWidget( jobIniTermValue, 1, 1 );
    browserGridLayout->addWidget( jobEndTermLabel, 2, 0 );
    browserGridLayout->addWidget( jobEndTermValue, 2, 1 );
    browserGridLayout->addWidget( jobResultLabel, 3, 0 );
    browserGridLayout->addWidget( jobResultValue, 3, 1 );
    browserGridLayout->addWidget( jobWorkerLabel, 4, 0 );
    browserGridLayout->addWidget( jobWorkerValue, 4, 1 );
    browserGridLayout->addWidget( curJobLabel, 5, 0 );
    browserGridLayout->addWidget( curJobSlider, 5, 1 );
    browserGridLayout->setRowMinimumHeight( 0, 27 );
    browserGridLayout->setRowMinimumHeight( 1, 18 );
    browserGridLayout->setRowMinimumHeight( 2, 22 );
    browserGridLayout->setRowMinimumHeight( 3, 22 );
    browserGridLayout->setRowMinimumHeight( 4, 22 );
    browserGridLayout->setRowMinimumHeight( 5, 22 );

    jobBrowserGroup->setLayout( browserGridLayout );


    QHBoxLayout *controlsLayout = new QHBoxLayout;
    controlsLayout->addWidget( jobConfigGroup );
    controlsLayout->addWidget( jobBrowserGroup );

    startStopButton = new QPushButton( tr( "&Start" ) );
    resetButton = new QPushButton( tr( "&Reset" ) );

    connect( startStopButton, SIGNAL( clicked() ),
             this, SLOT( startStopPressed() ) );

    connect( resetButton, SIGNAL( clicked() ),
             this, SLOT( resetPressed() ) );

    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addStretch();
    buttonLayout->addWidget( startStopButton );
    buttonLayout->addWidget( resetButton );
    buttonLayout->addStretch();

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->addWidget( jobsBoard );
    mainLayout->addLayout( controlsLayout );
    mainLayout->addSpacing( 6 );
    mainLayout->addLayout( buttonLayout );
    mainLayout->setStretch( 0, 2 );

    setLayout( mainLayout );
}

void Window::setWorkersCount( const qint32 &wC )
{
    // Update workersCount in main window
    workersCount = wC;

    // Destroy current workers, create new ones
    setupWorkers( workersCount );

    // Update workersCount in jobs board
    jobsBoard->setWorkersCount( workersCount );

    // Reset results
    resetResults();

    // Reset elapsed time
    elapsedMsLastStart = 0;
    elapsedMsLastStop = 0;
    mins = secs = cents = 0;

    // Reset time display
    updateTimeDisp();
}

void Window::resetJobsCountWrk()
{
    // Setup counters of jobs done per worker
    jobsCountWrk.clear();

    for ( quint32 i = 0; i < workersCount; i++ )
    {
        jobsCountWrk.append( 0 );

        // Update jobs done per worker counter at jobs board
        jobsBoard->setJobsCountWrk( i + 1, tr( "0 %" ) );
    }
}

void Window::workersCountChanged( int value )
{
    // Update workersCount in main window
    setWorkersCount( value );
}

void Window::setTermsPerJob( int value )
{
    termsPerJob = value * 1000000;
    totalTerms = termsPerJob * 100;

    termsPerJobValue->setText( tr( "%L1" ).arg( termsPerJob ) );
    totalTermsValue->setText( tr( "%L1" ).arg( totalTerms ) );

    destroyJobs();

    createJobs();

    showJob( curJobBrowser );
}

void Window::setCurJobIdBrowser( int value )
{
    curJobBrowser = value;

    showJob( curJobBrowser );
}

void Window::createJobs()
{
    Job* tmpJob = 0;

    for ( int i = 0; i < 100; i++ )
    {
        tmpJob = new Job( i + 1, 0, 0 );

        tmpJob->setIniTerm( i * termsPerJob + 1 );
        tmpJob->setEndTerm( i * termsPerJob + termsPerJob );

        jobs.enqueue( tmpJob );
    }
}

void Window::destroyJobs()
{
    while ( !jobs.isEmpty() )
        delete jobs.dequeue();
}

void Window::showJob( int jobNum )
{
    if ( !jobs.empty() &&
         ( jobNum >= 1 ) && ( jobNum <= jobs.size() ) )
    {
        int jID = jobNum - 1;

        jobIdValue->setText(
            tr( "%L1" ).arg( jobs.at( jID )->getId() ) );

        jobIniTermValue->setText(
            tr( "%L1" ).arg( jobs.at( jID )->getIniTerm() ) );

        jobEndTermValue->setText(
            tr( "%L1" ).arg( jobs.at( jID )->getEndTerm() ) );

        jobResultValue->setText(
            tr( "%L1" ).arg( jobs.at( jID )->getResult(), 15, 'f', 15, '0' ) );

        jobWorkerValue->setText(
            tr( "%L1" ).arg( jobs.at( jID )->getWorkerID() ) );
    }
}

void Window::startStopPressed()
{
    switch ( calcState )
    {
    case IDLE:
        //=========================================
        // Start new calculation
        //=========================================
        // Reset accumulated results
        resetResults();

        // Reset elapsed time
        elapsedMsLastStart = 0;
        elapsedMsLastStop = 0;
        mins = secs = cents = 0;

        // Reset time display
        updateTimeDisp();

        calcState = RUNNING;
        resettingResults = false;
        startStopButton->setText( tr( "&Stop" ) );
        termsPerJobSlider->setEnabled( false );
        workersSpinBox->setEnabled( false );

        // Start timing
        startTime.start();
        timer->start( 10 );

        // Start initial jobs
        for ( quint32 i = 0; i < workersCount; i++ )
        {
            workers[ i ].startCalc(
                jobs.at( i )->getId(),
                jobs.at( i )->getIniTerm(),
                jobs.at( i )->getEndTerm() );

            ++curNotAssignedJob;
        }

        break;

    case RUNNING:
        // Stop running jobs
        for ( quint32 i = 0; i < workersCount; i++ )
        {
            workers[ i ].resetCalc();
        }

        calcState = STOPPED;
        resettingResults = true;
        startStopButton->setEnabled( false );
        startStopButton->setText( tr( "&Start" ) );
        termsPerJobSlider->setEnabled( false );
        workersSpinBox->setEnabled( false );

        // Stop timing
        timer->stop();
        updateTimeDiff();
        elapsedMsLastStop += elapsedMsLastStart;
        break;

    case STOPPED:
        break;
    }
}

void Window::resetPressed()
{
    //=========================================
    // Reset calculation
    //=========================================
    switch ( calcState )
    {
    case RUNNING:
        // Stop running jobs
        for ( quint32 i = 0; i < workersCount; i++ )
        {
            workers[ i ].resetCalc();
        }

        // Reset accumulated results
        resetResults();

        calcState = IDLE;
        resettingResults = true;
        startStopButton->setText( tr( "&Start" ) );
        termsPerJobSlider->setEnabled( true );
        workersSpinBox->setEnabled( true );

        // Reset timing
        timer->stop();

        // Reset elapsed time
        elapsedMsLastStart = 0;
        elapsedMsLastStop = 0;
        mins = secs = cents = 0;

        // Reset time display
        updateTimeDisp();
        break;

    case IDLE:
        // Reset accumulated results
        resetResults();

        // Reset elapsed time
        elapsedMsLastStart = 0;
        elapsedMsLastStop = 0;
        mins = secs = cents = 0;

        // Reset time display
        updateTimeDisp();
        break;

    case STOPPED:
        // Reset accumulated results
        resetResults();

        calcState = IDLE;
        startStopButton->setText( tr( "&Start" ) );
        startStopButton->setEnabled( true );
        termsPerJobSlider->setEnabled( true );
        workersSpinBox->setEnabled( true );

        // Reset timing
        timer->stop();

        // Reset elapsed time
        elapsedMsLastStart = 0;
        elapsedMsLastStop = 0;
        mins = secs = cents = 0;

        // Reset time display
        updateTimeDisp();
        break;
    }
}

void Window::logResult( quint32 jobId, qreal jobResult, quint32 workerID )
{
    if ( resettingResults )
        return;

    // Update jobs vector
    jobs.at( jobId - 1 )->setResult( jobResult );
    jobs.at( jobId - 1 )->setWorkerID( workerID );

    // Update accumulated result
    totalResult += jobResult;

    // Update received results counter
    ++receivedResults;

    // Update jobs done per worker counter
    ++jobsCountWrk[ workerID - 1 ];

    qreal workLoad = jobsCountWrk[ workerID - 1 ] / 100.0 * 100.0;
    jobsBoard->setJobsCountWrk( workerID,
        tr( "%1 %" ).arg( workLoad, 0, 'f', 1, QLatin1Char( ' ' ) ) );

    // Update job browser
    // if job is currently being shown
    if ( jobId == curJobBrowser )
        showJob( curJobBrowser );

    // Update/display data in jobsBoard
    jobsBoard->setJobStatus( jobId, workerID );
    jobsBoard->setResultTxt( tr( "%1" ).arg(
        totalResult, 15, 'f', 15, '0' ) );
    jobsBoard->update();

    // Assign next unassigned job, while there is any
    if ( curNotAssignedJob <= ( quint32 )jobs.size() )
    {
        workers[ workerID - 1 ].startCalc(
            jobs.at( curNotAssignedJob - 1 )->getId(),
            jobs.at( curNotAssignedJob - 1 )->getIniTerm(),
            jobs.at( curNotAssignedJob - 1 )->getEndTerm() );

        ++curNotAssignedJob;
    }

    if ( receivedResults == ( quint32 )jobs.size() )
    {
        // we are finished!
        calcState = IDLE;
        resettingResults = false;
        startStopButton->setText( tr( "&Start" ) );
        startStopButton->setEnabled( true );
        termsPerJobSlider->setEnabled( true );
        workersSpinBox->setEnabled( true );

        // Stop timing
        timer->stop();
        updateTimeDiff();
        elapsedMsLastStop += elapsedMsLastStart;
    }
}

void Window::resetResults()
{
    // Reset accumulated result
    totalResult = 0.0;

    // Reset last unassigned job counter
    curNotAssignedJob = 1;

    // Reset received results counter
    receivedResults = 0;

    // Reset counters of jobs done per worker
    resetJobsCountWrk();

    // Reset results in jobs vector and jobs board
    Job* tmpJob = 0;

    for ( int jobId = 1; jobId <= 100; jobId++ )
    {
        tmpJob = jobs.at( jobId - 1 );

        tmpJob->setResult( 0.0 );
        tmpJob->setWorkerID( 0 );

        jobsBoard->setJobStatus( jobId, 0.0 );
    }

    // Update job browser
    showJob( curJobBrowser );

    // Reset displayed result in jobs board
    jobsBoard->setResultTxt( tr( "%1" ).arg(
        0.0, 15, 'f', 15, '0' ) );

    // Repaint jobs board
    jobsBoard->update();
}

void Window::setupWorkers( const quint32 &wC )
{
    // Destroy current workers
    if ( workers )
        delete[] workers;

    // Create new workers
    workers = new CalcThread[ wC ];

    // Config new workers
    for ( quint32 i = 0; i < wC; i++ )
    {
        workers[ i ].setWorkerId( i + 1 );

        connect( &workers[ i ], SIGNAL( jobDone( quint32, qreal, quint32 ) ),
                 this, SLOT( logResult( quint32, qreal, quint32 ) ) );
    }
}

void Window::setupTimer()
{
    timer = new QTimer( this );

    connect( timer, SIGNAL( timeout() ),
             this, SLOT( updateTimeDiff() ) );
}

void Window::updateTimeDiff()
{
    qint64 msDiff;

    // Get elapsed time being in RUNNING
    elapsedMsLastStart = ( qint64 )startTime.elapsed();

    // Add accumulated time from past runs
    msDiff = elapsedMsLastStart + elapsedMsLastStop;

    // Convert ms --> mint : secs : cents
    mins = msDiff / 60000;
    secs = ( msDiff - ( mins * 60000 ) ) / 1000;
    cents = ( msDiff - ( mins * 60000 ) - ( secs * 1000 ) ) / 10;

    // Display total elapsed time
    updateTimeDisp();
}

void Window::updateTimeDisp()
{
    static QString minsTxt;
    static QString secsTxt;
    static QString centsTxt;
    static QString totalTxt;

    minsTxt = QString( "%1" ).arg(
        mins, 2, 10, QLatin1Char('0') );

    secsTxt = QString( "%1" ).arg(
        secs, 2, 10, QLatin1Char('0') );

    centsTxt = QString( "%1" ).arg(
        cents, 2, 10, QLatin1Char('0') );

    totalTxt = minsTxt + " : " + secsTxt + " : " + centsTxt;

    // Send total elapsed time text to render area
    jobsBoard->setTimeTxt( totalTxt );

    // Update display
    jobsBoard->update();
}
