/****************************************************************************
**
** Copyright (C) 2018 Alexander Taylor
**
** This file is based in part on example code from the Qt4 Toolkit.
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
**
** All rights reserved.
**
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
**     the names of its contributors may be used to endorse or promote
**     products derived from this software without specific prior written
**     permission.
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
** $QT_END_LICENSE$
**
****************************************************************************/

#include <QtGui>

#include <math.h>

#include "version.h"
#include "button.h"
#include "calculator.h"

#define SETTINGS_VENDOR "ATutils"
#define SETTINGS_APP    "Calculator"


// ---------------------------------------------------------------------------
// CONSTRUCTOR
//

Calculator::Calculator( QWidget *parent )
    : QDialog( parent, Qt::WindowMinimizeButtonHint )
{
    // Initialize calculation status
    sumInMemory = 0.0;
    sumSoFar    = 0.0;
    factorSoFar = 0.0;
    waitingForOperand = true;

    // Initialize option fields
    isHexMode = false;
    isGrey    = false;
    currentView = Standard;

    // Create the entryfield and alternate-representation labels
    //

    display = new QLineEdit("0");
    display->setReadOnly( true );
    display->setAlignment( Qt::AlignRight );
    display->setMaxLength( 16 );

    reprName  = new QLabel("    ");
    reprName->setAlignment( Qt::AlignLeft );
    reprName->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed );
    reprLabel = new QLabel("00000000");
    reprLabel->setAlignment( Qt::AlignRight );
    reprLabel->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed );

/*
    Button *modeButton = createButton( tr("Hexadecimal"), "Hexadecimal", SLOT( modeClicked() ));
    QFontMetrics fm( modeButton->font() );
    modeButton->setMaximumHeight( fm.lineSpacing() + 5 );
    modeButton->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Fixed );
*/

    viewSelector = new QComboBox( this );
    viewSelector->addItem( tr("Standard"),    Qt::DisplayRole );
    viewSelector->addItem( tr("Scientific"),  Qt::DisplayRole );
    viewSelector->addItem( tr("Programming"), Qt::DisplayRole );
    viewSelector->addItem( tr("All"),         Qt::DisplayRole );
    viewSelector->setFocusPolicy( Qt::TabFocus );
    connect( viewSelector, SIGNAL( activated( const QString & )), this, SLOT( viewChanged( const QString & )));

    modeSelector = new QComboBox( this );
    modeSelector->addItem( tr("Decimal"),     Qt::DisplayRole );
    modeSelector->addItem( tr("Hexadecimal"), Qt::DisplayRole );
    modeSelector->setFocusPolicy( Qt::TabFocus );
    connect( modeSelector, SIGNAL( activated( const QString & )), this, SLOT( modeChanged( const QString & )));

    // Create the action buttons
    //
    clearAllButton   = createButton( tr("AC"),          "AC",     SLOT( clearAll() ));
    clearButton      = createButton( tr("CE"),          "CE",     QKeySequence::DeleteStartOfWord,   SLOT( clear() ));
    backspaceButton  = createButton( trUtf8("← Back"),"BACK",   QKeySequence( Qt::Key_Backspace ), SLOT( backspaceClicked() ));

    for ( int i = 0; i < NumDigitButtons; ++i ) {
        digitButtons[i] = createButton( QString::number( i ),
                                        QString::number( i ),
                                        QKeySequence( Qt::Key_0 + i ),
                                        SLOT( digitClicked() ));
    }
    for ( int i = 0; i < NumHexButtons; i++ ) {
        hexButtons[i] = createButton( QString( QChar( 65 + i )),
                                      QString( QChar( 65 + i )),
                                      QKeySequence( Qt::Key_A + i ),
                                      SLOT( digitClicked() ));
    }

    pointButton      = createButton( tr("."),           ".",      ".",  SLOT( pointClicked() ));
    changeSignButton = createButton( tr("\261"),        "+-",     SLOT( changeSignClicked() ));

    clearMemoryButton = createButton( tr("MC"),         "MC",     SLOT( clearMemory() ));
    readMemoryButton  = createButton( tr("MR"),         "MR",     SLOT( readMemory() ));
    setMemoryButton   = createButton( tr("MS"),         "MS",     SLOT( setMemory() ));
    addToMemoryButton = createButton( tr("M+"),         "M+",     SLOT( addToMemory() ));

    divisionButton   = createButton( tr("\367"),        "/",      "/", SLOT(multiplicativeOperatorClicked() ));
    timesButton      = createButton( tr("\327"),        "*",      "*", SLOT(multiplicativeOperatorClicked() ));
    minusButton      = createButton( trUtf8("−"),     "-",      "-", SLOT(additiveOperatorClicked() ));
    plusButton       = createButton( tr("+"),           "+",      "+", SLOT(additiveOperatorClicked() ));

    moduloButton     = createButton( tr("Mod"),         "MOD",    SLOT( multiplicativeOperatorClicked() ));
    squareRootButton = createButton( trUtf8("√"),     "SQRT",   SLOT( unaryOperatorClicked() ));

    equalButton      = createButton( tr("="),           "=",      QKeySequence( Qt::Key_Enter ), SLOT( equalClicked() ));

    reciprocalButton = createButton( tr("1/x"),         "RECIPR", SLOT( unaryOperatorClicked() ));
    squareButton     = createButton( tr("x\262"),       "SQUARE", SLOT( unaryOperatorClicked() ));
    expButton        = createButton( trUtf8("xⁿ"),    "NEXP",   SLOT( multiplicativeOperatorClicked() ));
    nRootButton      = createButton( trUtf8("ⁿ√x"), "NROOT",  SLOT( multiplicativeOperatorClicked() ));
    piButton         = createButton( trUtf8("π"),      "PI",     SLOT( unaryOperatorClicked() ));
    sinButton        = createButton( tr("sin"),         "SIN",    SLOT( unaryOperatorClicked() ));
    cosButton        = createButton( tr("cos"),         "COS",    SLOT( unaryOperatorClicked() ));
    tanButton        = createButton( tr("tan"),         "TAN",    SLOT( unaryOperatorClicked() ));
    logButton        = createButton( tr("log"),         "LOG",    SLOT( unaryOperatorClicked() ));
    lnButton         = createButton( tr("ln"),          "LN",     SLOT( unaryOperatorClicked() ));

    bitLeftButton    = createButton( tr("<<"),          "<<",     SLOT( multiplicativeOperatorClicked() ));
    bitRightButton   = createButton( tr(">>"),          ">>",     SLOT( multiplicativeOperatorClicked() ));
    bitAndButton     = createButton( tr("&&"),          "&",      SLOT( multiplicativeOperatorClicked() ));
    bitOrButton      = createButton( tr("|"),           "|",      SLOT( multiplicativeOperatorClicked() ));
    bitXorButton     = createButton( tr("^"),           "^",      SLOT( multiplicativeOperatorClicked() ));

    // Menu actions
    //

    styleMenu = new QMenu( tr("Appearance"), this );

    displayFontAction = new QAction( tr("Display font..."), this );
    connect( displayFontAction, SIGNAL( triggered() ), this, SLOT( displayFontChanged() ));
    buttonFontAction  = new QAction( tr("Button font..."), this );
    connect( buttonFontAction, SIGNAL( triggered() ), this, SLOT( buttonFontChanged() ));
    aboutAction       = new QAction( tr("Product information"), this );
    connect( aboutAction, SIGNAL( triggered() ), this, SLOT( about() ));

    monochromeAction  = new QAction( tr("Grey buttons"), this );
    monochromeAction->setCheckable( true );
    connect( monochromeAction, SIGNAL( changed() ), this, SLOT( greyChanged() ));

    styleMenu->addAction( displayFontAction );
    styleMenu->addAction( buttonFontAction );
    styleMenu->addSeparator();
    styleMenu->addAction( monochromeAction );

    // Lay out the controls
    //
    // Main (always-visible) button area

    QGridLayout *mainLayout = new QGridLayout;
    mainLayout->setSpacing( 6 );

    for ( int i = 1; i < NumDigitButtons; ++i ) {
        int row = ((9 - i) / 3) + 4;
        int column = ((i - 1) % 3) + 1;
        mainLayout->addWidget( digitButtons[i], row, column );
    }
    for ( int i = 0; i < NumHexButtons; i++ ) {
        mainLayout->addWidget( hexButtons[ i ], 3, i );
        hexButtons[ i ]->setVisible( isHexMode );
    }

    mainLayout->addWidget( clearAllButton, 2, 0, 1, 2 );
    mainLayout->addWidget( clearButton, 2, 2, 1, 2 );
    mainLayout->addWidget( backspaceButton, 2, 4, 1, 2 );

    mainLayout->addWidget( clearMemoryButton, 4, 0 );
    mainLayout->addWidget( setMemoryButton, 5, 0 );
    mainLayout->addWidget( readMemoryButton, 6, 0 );
    mainLayout->addWidget( addToMemoryButton, 7, 0 );

    mainLayout->addWidget( digitButtons[0], 7, 1 );
    mainLayout->addWidget( pointButton, 7, 2 );
    mainLayout->addWidget( changeSignButton, 7, 3 );

    mainLayout->addWidget( divisionButton, 4, 4 );
    mainLayout->addWidget( timesButton, 5, 4 );
    mainLayout->addWidget( minusButton, 6, 4 );
    mainLayout->addWidget( plusButton, 7, 4 );
    mainLayout->addWidget( equalButton, 6, 5, 2, 1 );

    mainLayout->addWidget( moduloButton, 4, 5 );
    mainLayout->addWidget( squareRootButton, 5, 5 );

    // Scientific button area

    sciLayout = new QGridLayout;
    sciLayout->addWidget( sinButton, 0, 0 );
    sciLayout->addWidget( cosButton, 1, 0 );
    sciLayout->addWidget( tanButton, 2, 0 );
    sciLayout->addWidget( logButton, 3, 0 );
    sciLayout->addWidget( lnButton, 4, 0 );
    sciLayout->addWidget( reciprocalButton, 0, 1 );
    sciLayout->addWidget( squareButton, 1, 1 );
    sciLayout->addWidget( expButton, 2, 1 );
    sciLayout->addWidget( nRootButton, 3, 1 );
    sciLayout->addWidget( piButton, 4, 1 );
    sciLayout->setSpacing( 6 );

    // Programming button area

    proLayout = new QGridLayout;
    proLayout->addWidget( bitLeftButton,  0, 0 );
    proLayout->addWidget( bitRightButton, 1, 0 );
    proLayout->addWidget( bitAndButton,   2, 0 );
    proLayout->addWidget( bitOrButton,    3, 0 );
    proLayout->addWidget( bitXorButton,   4, 0 );
    proLayout->setSpacing( 6 );

    // Section layout holding all the above button areas

    QHBoxLayout *secLayout = new QHBoxLayout;
    secLayout->setSpacing( 16 );
    secLayout->addLayout( proLayout,  1 );
    secLayout->addLayout( sciLayout,  2 );
    secLayout->addLayout( mainLayout, 6 );

    // Now the root-level layout

    QGridLayout *rootLayout = new QGridLayout;
    rootLayout->addWidget( display, 0, 0, 1, 2 );
    rootLayout->addWidget( reprName, 1, 0 );
//    rootLayout->addWidget( modeButton, 1, 0 );
    rootLayout->addWidget( reprLabel, 1, 1 );
    rootLayout->addLayout( secLayout, 2, 0, 1, 2 );

    rootLayout->addItem( new QSpacerItem( 1, 16 ), 3, 0 );
    rootLayout->addWidget( viewSelector, 4, 0 );
    rootLayout->addWidget( modeSelector, 4, 1 );

    setLayout( rootLayout );

    // Load settings and set up the UI accordingly
    //
    readSettings();

    setColourScheme();

    // Combo-box selections
    modeSelector->setCurrentIndex( (int) isHexMode );
    viewSelector->setCurrentIndex( currentView );
    if ( currentView == Programming || currentView == Standard )
        showLayout( sciLayout, false );
    if ( currentView == Scientific || currentView == Standard )
        showLayout( proLayout, false );

    // Finish up by setting the focus, window title and icon.

#ifndef __OS2__
    // Qt4 on OS/2 doesn't render PNGs well, so just leave it with the native icon
    QIcon icon;
    icon.addFile(":/images/calc_16.png", QSize( 16, 16 ), QIcon::Normal, QIcon::On );
    icon.addFile(":/images/calc_32.png", QSize( 32, 32 ), QIcon::Normal, QIcon::On );
    icon.addFile(":/images/calc_64.png", QSize( 64, 64 ), QIcon::Normal, QIcon::On );
    setWindowIcon( icon );
#endif

    display->setFocus( Qt::ActiveWindowFocusReason );
    setWindowTitle( tr("Calculator"));
}


// ===========================================================================
// SLOTS
//

// ---------------------------------------------------------------------------
// modeClicked
//
void Calculator::modeClicked()
{
    Button *clickedButton = qobject_cast<Button *>(sender());
    QString mode = clickedButton->text();

    if ( mode == tr("Hexadecimal"))
        clickedButton->setText( tr("Decimal"));
    else
        clickedButton->setText( tr("Hexadecimal"));
    modeChanged( (const QString) mode );
}


// ---------------------------------------------------------------------------
// digitClicked
//
void Calculator::digitClicked()
{
    Button *clickedButton = qobject_cast<Button *>( sender() );
    int digitValue;
    if ( isHexMode ) {
        bool ok;
        digitValue = clickedButton->identity().toInt( &ok, 16 );
    }
    else
        digitValue = clickedButton->identity().toInt();

    if ( display->text() == "0" && digitValue == 0.0 )
        return;

    if ( waitingForOperand ) {
        display->clear();
        waitingForOperand = false;
    }
    if ( isHexMode )
        display->setText( display->text() + QString::number( digitValue, 16 ).toUpper() );
    else
        display->setText( display->text() + QString::number( digitValue ));
    updateAltRepr();
}


// ---------------------------------------------------------------------------
// unaryOperatorClicked
//
void Calculator::unaryOperatorClicked()
{
    Button *clickedButton = qobject_cast<Button *>( sender() );
    QString clickedOperator = clickedButton->identity();
    double operand = currentDisplayValue();
    double result = 0.0;

    if ( clickedOperator == "SQRT") {           // square root
        if ( operand < 0.0 ) {
            abortOperation();
            return;
        }
        result = sqrt( operand );
    } else if ( clickedOperator == "SQUARE") {  // square
        result = pow( operand, 2.0 );
    } else if ( clickedOperator == "RECIPR") {  // reciprocal
        if ( operand == 0.0 ) {
            abortOperation();
            return;
        }
        result = 1.0 / operand;
    }
    else if ( clickedOperator == "LOG") {       // log
        if ( operand > 0.0 )
            result = log10( operand );
        else {
            abortOperation();
            return;
        }
    }
    else if ( clickedOperator == "LN") {        // ln
        if ( operand > 0.0 )
            result = log( operand );
        else {
            abortOperation();
            return;
        }
    }
    else if ( clickedOperator == "PI") {        // pi
        result = M_PI;
    }
    else if ( clickedOperator == "SIN") {       // sine
        result = RAD_TO_DEG( sin( DEG_TO_RAD( operand )));
    }
    else if ( clickedOperator == "COS") {       // cosine
        result = RAD_TO_DEG( cos( DEG_TO_RAD( operand )));
    }
    else if ( clickedOperator == "TAN") {       // tangent
        result = RAD_TO_DEG( tan( DEG_TO_RAD( operand )));
    }

    setCurrentDisplayValue( result );
    waitingForOperand = true;
}


// ---------------------------------------------------------------------------
// additiveOperatorClicked
//
void Calculator::additiveOperatorClicked()
{
    Button *clickedButton = qobject_cast<Button *>(sender());
    QString clickedOperator = clickedButton->identity();
    double operand = currentDisplayValue();

    if ( !pendingMultiplicativeOperator.isEmpty() ) {
        if ( ! calculate( operand, pendingMultiplicativeOperator )) {
            abortOperation();
            return;
        }
        setCurrentDisplayValue( factorSoFar );
        operand = factorSoFar;
        factorSoFar = 0.0;
        pendingMultiplicativeOperator.clear();
    }

    if ( !pendingAdditiveOperator.isEmpty() ) {
        if ( ! calculate( operand, pendingAdditiveOperator )) {
            abortOperation();
            return;
        }
        setCurrentDisplayValue( sumSoFar );
    } else {
        sumSoFar = operand;
    }

    pendingAdditiveOperator = clickedOperator;
    waitingForOperand = true;
}


// ---------------------------------------------------------------------------
// multiplicativeOperatorClicked
//
void Calculator::multiplicativeOperatorClicked()
{
    Button *clickedButton = qobject_cast<Button *>( sender() );
    QString clickedOperator = clickedButton->identity();
    double operand = currentDisplayValue();

    if ( !pendingMultiplicativeOperator.isEmpty() ) {
        if ( ! calculate( operand, pendingMultiplicativeOperator )) {
            abortOperation();
            return;
        }
        setCurrentDisplayValue( factorSoFar );
    } else {
        factorSoFar = operand;
    }

    pendingMultiplicativeOperator = clickedOperator;
    waitingForOperand = true;
}


// ---------------------------------------------------------------------------
// equalClicked
//
void Calculator::equalClicked()
{
    double operand = currentDisplayValue();

    if ( !pendingMultiplicativeOperator.isEmpty() ) {
        if ( ! calculate( operand, pendingMultiplicativeOperator )) {
            abortOperation();
            return;
        }
        operand = factorSoFar;
        factorSoFar = 0.0;
        pendingMultiplicativeOperator.clear();
    }
    if (!pendingAdditiveOperator.isEmpty()) {
        if (!calculate(operand, pendingAdditiveOperator)) {
            abortOperation();
            return;
        }
        pendingAdditiveOperator.clear();
    } else {
        sumSoFar = operand;
    }

    setCurrentDisplayValue( sumSoFar );
    sumSoFar = 0.0;
    waitingForOperand = true;
}


// ---------------------------------------------------------------------------
// pointClicked
//
void Calculator::pointClicked()
{
    if ( isHexMode ) return;
    if ( waitingForOperand )
        display->setText("0");
    if ( !display->text().contains("."))
        display->setText( display->text() + tr("."));
    updateAltRepr();
    waitingForOperand = false;
}


// ---------------------------------------------------------------------------
// changeSignClicked
//
void Calculator::changeSignClicked()
{
    QString text = display->text();

    waitingForOperand = true;
    if ( isHexMode ) {
        bool ok;
        long long llValue = text.toULongLong( &ok, 16 );
        if ( !ok )  {
            abortOperation();
            // TODO show error: number out of range
            return;
        }
        if ( llValue > 0.0 )
            llValue -= (2 * llValue);
        else
            llValue = llabs( llValue );
        setCurrentDisplayValue( llValue );
    }
    else {
        double value = currentDisplayValue();
        if ( value > 0.0 ) {
            text.prepend( tr("-"));
        } else if ( value < 0.0 ) {
            text.remove( 0, 1 );
        }
        display->setText( text );
        updateAltRepr();
    }
}


// ---------------------------------------------------------------------------
// backspaceClicked
//
void Calculator::backspaceClicked()
{
    if (waitingForOperand)
        return;

    QString text = display->text();
    text.chop( 1 );
    if ( text.isEmpty() ) {
        text = "0";
        waitingForOperand = true;
    }
    if ( isHexMode )
        setCurrentDisplayValue( text.toLongLong( NULL, 16 ));
    else {
        display->setText( text );
        updateAltRepr();
    }
}


// ---------------------------------------------------------------------------
// clear
//
void Calculator::clear()
{
    display->setText("0");
    updateAltRepr();
    waitingForOperand = true;
}


// ---------------------------------------------------------------------------
// clearAll
//
void Calculator::clearAll()
{
    sumSoFar = 0.0;
    factorSoFar = 0.0;
    pendingAdditiveOperator.clear();
    pendingMultiplicativeOperator.clear();
    display->setText("0");
    updateAltRepr();
    waitingForOperand = true;
}


// ---------------------------------------------------------------------------
// clearMemory
//
void Calculator::clearMemory()
{
    sumInMemory = 0.0;
}


// ---------------------------------------------------------------------------
// readMemory
//
void Calculator::readMemory()
{
    setCurrentDisplayValue( sumInMemory );
    waitingForOperand = true;
}


// ---------------------------------------------------------------------------
// setMemory
//
void Calculator::setMemory()
{
    equalClicked();
    sumInMemory = currentDisplayValue();
}


// ---------------------------------------------------------------------------
// addToMemory
//
void Calculator::addToMemory()
{
    equalClicked();
    sumInMemory += currentDisplayValue();
}


// ---------------------------------------------------------------------------
// subFromMemory
//
void Calculator::subFromMemory()
{
    equalClicked();
    sumInMemory -= currentDisplayValue();
}


// ---------------------------------------------------------------------------
// modeChanged
//
void Calculator::modeChanged( const QString &text )
{
    double value = currentDisplayValue();
    if ( text == tr("Hexadecimal")) {
        isHexMode = true;
    }
    else {
        isHexMode = false;
    }
    for ( int i = 0; i < NumHexButtons; i++ )
        hexButtons[ i ]->setVisible( isHexMode );
    update();
    reprName->setText( isHexMode? "DEC:": "HEX:");
    setCurrentDisplayValue( value );
}


// ---------------------------------------------------------------------------
// viewChanged
//
void Calculator::viewChanged( const QString &text )
{
    bool  scientific  = false;
    bool  programming = false;
    int   widthAdjust = 0;
/*
    QSize windowSize = size();
    QSize minSize = minimumSize();
*/

    if ( text == tr("Scientific")) {
        scientific = true;
        currentView = Scientific;
    }
    else if ( text == tr("Programming")) {
        programming = true;
        currentView = Programming;
    }
    else if ( text == tr("All")) {
        scientific  = true;
        programming = true;
        currentView = All;
    }
    else currentView = Standard;

    widthAdjust += showLayout( sciLayout, scientific );
    widthAdjust += showLayout( proLayout, programming );
/*
    windowSize.rwidth() += widthAdjust;
    minSize.rwidth() += widthAdjust;
    setMinimumSize( minSize );
    resize( windowSize );
*/
}


// ---------------------------------------------------------------------------
// greyChanged
//
void Calculator::greyChanged()
{
    isGrey = monochromeAction->isChecked();
    setColourScheme();
}


// ---------------------------------------------------------------------------
// buttonFontChanged
//
void Calculator::buttonFontChanged()
{
    bool selected;
    QFont newFont = QFontDialog::getFont( &selected, clearButton->font(), this );
    if ( selected ) {
        setButtonFont( newFont );
    }
}


// ---------------------------------------------------------------------------
// displayFontChanged
//
void Calculator::displayFontChanged()
{
    bool selected;
    QFont newFont = QFontDialog::getFont( &selected, display->font(), this );
    if ( selected ) {
        display->setFont( newFont );
    }
}


// ---------------------------------------------------------------------------
// about
//
void Calculator::about()
{
    QMessageBox::about( this,
                        tr("Product Information"),
                        tr("<b>AT Desktop Calculator</b><br>Version %1<hr>"
                           "<p>Copyright &copy; 2018 Alexander Taylor.<br>"
                           "Copyright &copy; 2011 Nokia Corporation and/or its subsidiaries.<br>"
                           "All rights reserved."
                           "<br></p>").arg( VER_FILEVERSION_STR )
                      );
}


// ===========================================================================
// OVERRIDDEN EVENTS
//

void Calculator::closeEvent( QCloseEvent *event )
{
    writeSettings();
    event->accept();
}


void Calculator::contextMenuEvent( QContextMenuEvent *event )
{
    QMenu menu( this );
    menu.addMenu( styleMenu );
    menu.addSeparator();
    menu.addAction( aboutAction );
    menu.exec( event->globalPos() );
}


// ===========================================================================
// OTHER METHODS
//

// ---------------------------------------------------------------------------
// createButton
//
// Shortcut method (overloaded) to create a calculator button with the given
// text, ID tag, optional key-sequence, and slot method.
//
Button *Calculator::createButton( const QString &text,
                                  const QString &identity,
                                  const char *member )
{
    Button *button = new Button( text );
    button->setIdentity( identity );
    connect( button, SIGNAL(clicked()), this, member );
    return button;
}

Button *Calculator::createButton( const QString &text,
                                  const QString &identity,
                                  QKeySequence::StandardKey key,
                                  const char *member )
{
    Button *button = createButton( text, identity, member );
    button->setShortcut( key );
    return button;
}

Button *Calculator::createButton( const QString &text,
                                  const QString &identity,
                                  const QString &key,
                                  const char *member )
{
    Button *button = createButton( text, identity, member );
    button->setShortcut( key );
    return button;
}


// ---------------------------------------------------------------------------
// setButtonFont
//
// Set all calculator buttons to the given font.
//
void Calculator::setButtonFont( const QFont &font )
{
    QList<Button *>allButtons = findChildren<Button *>();
    foreach( Button *b, allButtons ) {
        b->setFont( font );
    }
}


// ---------------------------------------------------------------------------
// setColourScheme
//
// Set the calculator colour scheme. Normally the different categories of
// buttons (digit, operator, memory, scientific, programming) use different
// colours.  If the 'grey' option is enabled, a more monochromatic scheme will
// be used.
//
void Calculator::setColourScheme()
{
    QPalette pal = clearAllButton->palette();
    pal.setColor( QPalette::Button, QColor("#A0A0A0"));
    pal.setColor( QPalette::ButtonText, QColor("#800000"));
    clearAllButton->setPalette( pal );

    pal.setColor( QPalette::ButtonText, QColor("#000000"));
    clearButton->setPalette( pal );
    backspaceButton->setPalette( pal );

    if ( !isGrey ) pal.setColor( QPalette::Button, QColor("#A8C8A8"));
    clearMemoryButton->setPalette( pal );
    readMemoryButton->setPalette( pal );
    setMemoryButton->setPalette( pal );
    addToMemoryButton->setPalette( pal );

    pal.setColor( QPalette::Button, QColor("#A8A8A8"));
    squareRootButton->setPalette( pal );
    moduloButton->setPalette( pal );
    divisionButton->setPalette( pal );
    timesButton->setPalette( pal );
    minusButton->setPalette( pal );
    plusButton->setPalette( pal );
    equalButton->setPalette( pal );

    if ( !isGrey ) pal.setColor( QPalette::Button, QColor("#A0B0C8"));
    squareButton->setPalette( pal );
    reciprocalButton->setPalette( pal );
    expButton->setPalette( pal );
    nRootButton->setPalette( pal );
    piButton->setPalette( pal );
    sinButton->setPalette( pal );
    cosButton->setPalette( pal );
    tanButton->setPalette( pal );
    logButton->setPalette( pal );
    lnButton->setPalette( pal );

    if ( !isGrey ) pal.setColor( QPalette::Button, QColor("#C8C8A8"));
    bitLeftButton->setPalette( pal );
    bitRightButton->setPalette( pal );
    bitAndButton->setPalette( pal );
    bitOrButton->setPalette( pal );
    bitXorButton->setPalette( pal );
}


// ---------------------------------------------------------------------------
// currentDisplayValue
//
// Returns the current value shown in the display (as double).
//
double Calculator::currentDisplayValue()
{
    bool ok;
    double value = 0;

    if ( isHexMode )
        value = (double) display->text().toLongLong( &ok, 16 );
    else
        value = display->text().toDouble();

    return value;
}


// ---------------------------------------------------------------------------
// setCurrentDisplayValue
//
// (Overloaded) Updates the display with the current value (either double or
// long long), along with the alternate-representation field. The numbers will
// be formatted in decimal/hexadecimal as appropriate.
//
void Calculator::setCurrentDisplayValue( double value )
{
    if ( isHexMode )
        display->setText( QString::number( (qlonglong) value, 16 ).toUpper() );
    else
        display->setText( QString::number( value ));
    updateAltRepr();
}

void Calculator::setCurrentDisplayValue( long long value )
{
    if ( value > 0xFFFFFFFF )
        display->setText( QString::number( value, isHexMode? 16: 10 ).toUpper() );
    else
        display->setText( QString::number( (long)value, isHexMode? 16: 10 ).toUpper() );
    updateAltRepr();
}


// ---------------------------------------------------------------------------
// abortOperation
//
void Calculator::abortOperation()
{
    clearAll();
    display->setText( tr("####"));
    updateAltRepr();
}


// ---------------------------------------------------------------------------
// calculate
//
// This method performs all two-operand calculation operations.
//
bool Calculator::calculate( double rightOperand, const QString &pendingOperator )
{
    if ( pendingOperator == "+") {                          // add
        sumSoFar += rightOperand;
    } else if ( pendingOperator == "-") {                   // subtract
        sumSoFar -= rightOperand;
    } else if ( pendingOperator == "*") {                   // multiply
        factorSoFar *= rightOperand;
    } else if ( pendingOperator == "/") {                   // divide
        if ( rightOperand == 0.0 )
           return false;
        factorSoFar /= rightOperand;
    } else if ( pendingOperator == "MOD") {                 // modulo
        if ( rightOperand == 0.0 )
           return false;
        factorSoFar = (qlonglong)factorSoFar % (qlonglong)rightOperand;
    } else if ( pendingOperator == "NEXP") {                // n-exp
        factorSoFar = pow( factorSoFar, rightOperand );
    } else if ( pendingOperator == "NROOT") {               // n-root
        if ( factorSoFar > 0.0 )
            factorSoFar = exp( log( factorSoFar ) / rightOperand );
        else if ( factorSoFar == 0.0 )
            return true;
        else return false;
    } else if ( pendingOperator == "<<") {                  // shift left
        factorSoFar = (qulonglong) factorSoFar << (qulonglong) rightOperand;
    } else if ( pendingOperator == ">>") {                  // shift right
        factorSoFar = (qulonglong) factorSoFar >> (qulonglong) rightOperand;
    } else if ( pendingOperator == "&") {                   // bitwise AND
        factorSoFar = (qulonglong) factorSoFar & (qulonglong) rightOperand;
    } else if ( pendingOperator == "|") {                   // bitwise OR
        factorSoFar = (qulonglong) factorSoFar | (qulonglong) rightOperand;
    } else if ( pendingOperator == "^") {                   // bitwise XOR
        factorSoFar = (qulonglong) factorSoFar ^ (qulonglong) rightOperand;
    }

    return true;
}


// ---------------------------------------------------------------------------
// findFont
//
QString Calculator::findFont( const QString fontFamily )
{
    static QFontDatabase fontdb;
    QString matchingFont = "";
    QStringList matching = fontdb.families().filter( fontFamily );
    if ( !matching.isEmpty() )
        matchingFont = matching.at( 0 );
    return matchingFont;
}


// ---------------------------------------------------------------------------
// updateAltRepr
//
void Calculator::updateAltRepr()
{
    long long llValue;
    char achRepr[ 17 ] = {0};
    bool ok;

    if ( isHexMode ) {
        // need to use toULongLong to parse hex in case of signed overflow
        llValue = display->text().toULongLong( &ok, 16 );
        if ( ok ) {
            if ( llValue > 0xFFFFFFFF )
                sprintf( achRepr, "%lld", llValue );
            else
                sprintf( achRepr, "%ld", (long)llValue );
        }
    }
    else {
        llValue = display->text().toLongLong( &ok, 10 );
        if ( ok ) {
            if ( llValue > 0xFFFFFFFF )
                sprintf( achRepr, "%016llX", llValue );
            else
                sprintf( achRepr, "%08lX", (long)llValue );
        }
    }
    if ( ok )
        reprLabel->setText( QString( achRepr ));
    else
        reprLabel->setText("####");
}


// ---------------------------------------------------------------------------
// showLayout
//
int Calculator::showLayout( const QLayout *layout, bool visible )
{
    int widthAdjust = 0;
/*
    if ( !visible ) {
        // we will be hiding this layout; find how much width to subtract
        widthAdjust -= layout->contentsRect().width();
    }
*/
    for ( int i = 0; i < layout->count(); i++ ) {
        QWidget *control = layout->itemAt( i )->widget();
        if ( control )
            control->setVisible( visible );
    }
/*
    if ( visible ) {
        // we are showing this layout; find how much width to add
        widthAdjust = layout->minimumSize().width();
    }
*/
    return widthAdjust;
}


// ---------------------------------------------------------------------------
// readSettings
//
void Calculator::readSettings()
{
    QSettings settings( SETTINGS_VENDOR, SETTINGS_APP );

    currentView = (short) settings.value("view").toInt();
    isHexMode   = settings.value("mode").toBool();
    isGrey      = settings.value("monochrome").toBool();
    monochromeAction->setChecked( isGrey );

    restoreGeometry( settings.value("geometry").toByteArray() );

    reprName->setText( isHexMode? "DEC:": "HEX:");

    // Set up fonts
    QFont font = display->font();
    QString foundFont = findFont("Consolas");
    if ( foundFont.isEmpty() )
        QString foundFont = findFont("Inconsolata");
    if ( foundFont.isEmpty() )
        foundFont = findFont("Source Code Pro");
    if ( foundFont.isEmpty() )
        foundFont = "Workplace Sans";
    font.fromString( settings.value("editFont", foundFont + ",18").toString() );
    display->setFont( font );

    foundFont = findFont("Helvetica");
    if ( foundFont.isEmpty() )
        foundFont = findFont("Source Sans Pro");
    if ( foundFont.isEmpty() )
        foundFont = "Workplace Sans";
    font.fromString( settings.value("buttonFont", foundFont + ",12").toString() );
    setButtonFont( font );
}


// ---------------------------------------------------------------------------
// writeSettings
//
void Calculator::writeSettings()
{
    QSettings settings( SETTINGS_VENDOR, SETTINGS_APP );

    settings.setValue("geometry",   saveGeometry() );
    settings.setValue("editFont",   display->font().toString() );
    settings.setValue("buttonFont", clearButton->font().toString() );
    settings.setValue("view",       currentView );
    settings.setValue("mode",       isHexMode );
    settings.setValue("monochrome", isGrey );
}


