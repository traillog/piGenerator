
#include "jobsboard.h"

#include <QPainter>

JobsBoard::JobsBoard( QWidget *parent )
    : QWidget( parent )
{
    setWorkersCount( 1 );

    initColorMap();

    initJobsStatus();

    setResultTxt( tr( "%1" ).arg( 0.0, 15, 'f', 15, '0' ) );

    setTimeTxt( tr( "00 : 00 : 00" ) );

    setBackgroundRole( QPalette::Base );
    setAutoFillBackground( true );

    update();
}

QSize JobsBoard::minimumSizeHint() const
{
    return QSize( 600, 370 );
}

QSize JobsBoard::sizeHint() const
{
    return QSize( 600, 370 );
}

void JobsBoard::paintEvent( QPaintEvent * /* event */)
{
    QPainter painter( this );
    QBrush jobColor;
    QPoint jobOrg;
    qint32 jobWorker;

    //=============================================
    // Draw jobs board
    painter.save();
    painter.translate( width() / 2 - 200,
                       height() / 2 - ( 20 * 5 ) );

    // Iterate over all jobs status
    for ( int i = 0; i < 100; i++ )
    {
        // Get job's worker
        // worker == 0 --> job is not done
        // worker != 0 --> job is done
        jobWorker = jobsStatus.at( i );

        // Square origin
        jobOrg.setX( ( i % 10 ) * 20 );
        jobOrg.setY( ( i / 10 ) * 20 );

        // Get job's color
        jobColor = colorMap.at( jobWorker );

        // Draw job's square (ID colored)
        painter.setBrush( jobColor );
        painter.drawRect( jobOrg.x(), jobOrg.y(),
                          20, 20 );
    }

    painter.restore();
    //=============================================

    //=============================================
    // Other outputs
    painter.save();
    painter.translate( width() / 2 + 50,
                       height() / 2 - 105 );

    // Output accumulated result
    painter.drawText( 0, 20, resultText );

    // Output elapsed time
    painter.drawText( 0, 48, elapsedTimeTxt );

    // Output work balance & legend
    for ( int i = 0; i < workersCount; i++ )
    {
        jobColor = colorMap.at( i + 1 );

        painter.setBrush( jobColor );
        painter.drawRect( 0, 65 + i * 25, 16, 16 );

        painter.drawText( 27, 79 + i * 25,
                          tr( "%1" ).arg( jobsCountWrk.at( i ) ) );
    }

    painter.restore();
    //=============================================

    // Frame around plot area
    painter.setRenderHint( QPainter::Antialiasing, false );
    painter.setPen( palette().dark().color() );
    painter.setBrush( Qt::NoBrush );
    painter.drawRect( QRect( 0, 0, width() - 1, height() - 1 ) );
}

void JobsBoard::initColorMap()
{
    colorMap.append( Qt::white );
    colorMap.append( QColor( 255,   0,   0, 127 ) );
    colorMap.append( QColor(   0, 255,   0, 127 ) );
    colorMap.append( QColor(   0,   0, 255, 127 ) );
    colorMap.append( QColor( 255, 255,   0, 127 ) );
}

void JobsBoard::setWorkersCount( const qint32& wC )
{
    workersCount = wC;

    initJobsCountWrk();
}

void JobsBoard::initJobsCountWrk()
{
    // Empty current list
    jobsCountWrk.clear();

    // Initialise list
    for ( int i = 0; i < workersCount; i++ )
    {
        jobsCountWrk.append( tr( "0 %" ) );
    }
}

void JobsBoard::initJobsStatus()
{
    // Empty current list
    jobsStatus.clear();

    // Initialise list
    for ( int i = 0; i < 100; i++ )
    {
        jobsStatus.append( 0 );
    }
}

void JobsBoard::setJobStatus( qint32 jobId, qint32 workerId )
{
    if ( ( jobId >= 1 && jobId <= 100 ) &&
         ( workerId >= 0 && workerId <= workersCount ) )
    {
        jobsStatus.replace( jobId - 1, workerId );
    }
}

void JobsBoard::setJobsCountWrk( qint32 wkId, const QString& wkBalance )
{
    if ( wkId >= 1 && wkId <= workersCount )
    {
        jobsCountWrk.replace( wkId - 1, wkBalance );
    }
}

void JobsBoard::setResultTxt( const QString& rsltTxt )
{
    resultText = rsltTxt;
}

void JobsBoard::setTimeTxt( const QString &valueTxt )
{
    elapsedTimeTxt = valueTxt;
}
