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
**
****************************************************************************/

#include <QtGui>
#include <sstream>

#include <math.h>
#include <limits.h>

#if 0   //ifdef __OS2__
#define WS_TOPMOST  0x00200000L
#define INCL_WINWINDOWMGR
#define INCL_WINERRORS
#include <os2.h>
#endif

#ifdef __OS2__
#include "os2native.h"
#endif


#include "version.h"
#include "button.h"
#include "calculator.h"

#define SETTINGS_VENDOR "ATutils"
#define SETTINGS_APP    "Calculator"


// ---------------------------------------------------------------------------
// CONSTRUCTOR
//

Calculator::Calculator( QWidget *parent )
    : QDialog( parent, Qt::WindowMinimizeButtonHint | Qt::WindowCloseButtonHint )
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
    clearAllButton   = createButton( tr("AC"),           "AC",   "Ctrl+Del",       SLOT( clearAll() ));
    clearButton      = createButton( tr("CE"),           "CE",   "Ctrl+Backspace", SLOT( clear() ));
    backspaceButton  = createButton( trUtf8("← Back"), "BACK", QKeySequence( Qt::Key_Backspace ), SLOT( backspaceClicked() ));

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

    pointButton      = createButton( tr("."),           ".",      ".",       SLOT( pointClicked() ));
    changeSignButton = createButton( trUtf8("±"),      "+-",     "Shift+-", SLOT( changeSignClicked() ));

    clearMemoryButton = createButton( tr("MC"),         "MC",     SLOT( clearMemory() ));
    readMemoryButton  = createButton( tr("MR"),         "MR",     SLOT( readMemory() ));
    setMemoryButton   = createButton( tr("MS"),         "MS",     SLOT( setMemory() ));
    addToMemoryButton = createButton( tr("M+"),         "M+",     SLOT( addToMemory() ));

    divisionButton   = createButton( trUtf8("÷"),      "/",      "/", SLOT(multiplicativeOperatorClicked() ));
    timesButton      = createButton( trUtf8("×"),      "*",      "*", SLOT(multiplicativeOperatorClicked() ));
    minusButton      = createButton( trUtf8("−"),     "-",      "-", SLOT(additiveOperatorClicked() ));
    plusButton       = createButton( tr("+"),           "+",      "+", SLOT(additiveOperatorClicked() ));

    moduloButton     = createButton( tr("Mod"),         "MOD",    "%", SLOT( multiplicativeOperatorClicked() ));

    equalButton      = createButton( tr("="),           "=",      QKeySequence( Qt::Key_Enter ), SLOT( equalClicked() ));

    reciprocalButton = createButton( tr("1/x"),         "RECIPR", "Shift+R", SLOT( unaryOperatorClicked() ));
    squareButton     = createButton( trUtf8("x²"),     "SQUARE", "Shift+S", SLOT( unaryOperatorClicked() ));
    expButton        = createButton( trUtf8("xⁿ"),    "NEXP",   "Shift+X", SLOT( exponentialOperatorClicked() ));
    squareRootButton = createButton( trUtf8("√x"),    "SQRT",   "Shift+Q", SLOT( unaryOperatorClicked() ));
    nRootButton      = createButton( trUtf8("ⁿ√x"), "NROOT",  "Shift+T", SLOT( exponentialOperatorClicked() ));
    piButton         = createButton( trUtf8("π"),      "PI",     "Shift+P", SLOT( unaryOperatorClicked() ));
    sinButton        = createButton( tr("sin"),         "SIN",    "Ctrl+Alt+S", SLOT( unaryOperatorClicked() ));
    cosButton        = createButton( tr("cos"),         "COS",    "Ctrl+Alt+C", SLOT( unaryOperatorClicked() ));
    tanButton        = createButton( tr("tan"),         "TAN",    "Ctrl+Alt+T", SLOT( unaryOperatorClicked() ));
    logButton        = createButton( tr("log"),         "LOG",    "Shift+L", SLOT( unaryOperatorClicked() ));
    lnButton         = createButton( tr("ln"),          "LN",     "Shift+N", SLOT( unaryOperatorClicked() ));
    eButton          = createButton( tr("e"),           "E",      "Shift+E", SLOT( unaryOperatorClicked() ));

    bitLeftButton    = createButton( tr("<<"),          "<<",     "Shift+<", SLOT( multiplicativeOperatorClicked() ));
    bitRightButton   = createButton( tr(">>"),          ">>",     "Shift+>", SLOT( multiplicativeOperatorClicked() ));
    bitAndButton     = createButton( tr("&&"),          "&",      "Shift+&", SLOT( multiplicativeOperatorClicked() ));
    bitOrButton      = createButton( tr("|"),           "|",      "Shift+|", SLOT( multiplicativeOperatorClicked() ));
    bitXorButton     = createButton( tr("^"),           "^",      "Shift+^", SLOT( multiplicativeOperatorClicked() ));
    bitNotButton     = createButton( trUtf8("¬"),      "NOT",    "Shift+~", SLOT( unaryOperatorClicked() ));
    integerButton    = createButton( tr("Int"),         "INT",    "\\",      SLOT( unaryOperatorClicked() ));

    // Menu actions
    //
    menuBar = new QMenuBar();

    fileMenu = menuBar->addMenu( tr("&File"));
    editMenu = menuBar->addMenu( tr("&Edit"));
    viewMenu = menuBar->addMenu( tr("&View"));
    helpMenu = menuBar->addMenu( tr("&Help"));

    quitAction = new QAction( tr("E&xit"), this );
    connect( quitAction, SIGNAL( triggered() ), this, SLOT( close() ));
#ifdef __OS2__
    quitAction->setShortcut( tr("F3") );
#else
    quitAction->setShortcut( QKeySequence::Quit );
#endif
    fileMenu->addAction( quitAction );

    displayFontAction = new QAction( tr("&Display font..."), this );
    connect( displayFontAction, SIGNAL( triggered() ), this, SLOT( displayFontChanged() ));
    buttonFontAction  = new QAction( tr("&Button font..."), this );
    connect( buttonFontAction, SIGNAL( triggered() ), this, SLOT( buttonFontChanged() ));
    monochromeAction  = new QAction( tr("&Grey buttons"), this );
    monochromeAction->setCheckable( true );
    connect( monochromeAction, SIGNAL( changed() ), this, SLOT( greyChanged() ));

    styleMenu = viewMenu->addMenu( tr("&Appearance"));
    styleMenu->addAction( displayFontAction );
    styleMenu->addAction( buttonFontAction );
    styleMenu->addSeparator();
    styleMenu->addAction( monochromeAction );

    onTopAction = new QAction( tr("Stay on &top"), this );
    onTopAction->setCheckable( true );
    connect( onTopAction, SIGNAL( changed() ), this, SLOT( onTopChanged() ));
#ifndef __OS2__
    viewMenu->addSeparator();
    viewMenu->addAction( onTopAction );
#endif

    copyAction = new QAction( tr("&Copy"), this );
#ifdef __OS2__
    QList<QKeySequence> copyShortcuts;
    copyShortcuts << QKeySequence("Ctrl+Ins") << QKeySequence::Copy;
    copyAction->setShortcuts( copyShortcuts );
#else
    copyAction->setShortcut( QKeySequence::Copy );
#endif
    connect( copyAction, SIGNAL( triggered() ), this, SLOT( copy() ));
    pasteAction = new QAction( tr("&Paste"), this );
#ifdef __OS2__
    QList<QKeySequence> pasteShortcuts;
    pasteShortcuts << QKeySequence("Shift+Ins") << QKeySequence::Paste;
    pasteAction->setShortcuts( pasteShortcuts );
#else
    pasteAction->setShortcut( QKeySequence::Paste );
#endif
    connect( pasteAction, SIGNAL( triggered() ), this, SLOT( paste() ));

    editMenu->addAction( copyAction );
    editMenu->addAction( pasteAction );

    helpGeneralAction = new QAction( tr("General &help"), this );
    helpGeneralAction->setStatusTip( tr("General program help") );
    helpGeneralAction->setShortcut( tr("F1") );
    connect( helpGeneralAction, SIGNAL( triggered() ), this, SLOT( showGeneralHelp() ));

    helpKeysAction = new QAction( tr("&Keys help"), this );
    helpKeysAction->setStatusTip( tr("Help on keyboard commands") );
    connect( helpKeysAction, SIGNAL( triggered() ), this, SLOT( showKeysHelp() ));

    aboutAction = new QAction( tr("Product &information"), this );
    connect( aboutAction, SIGNAL( triggered() ), this, SLOT( about() ));

    helpMenu->addAction( helpGeneralAction );
    helpMenu->addAction( helpKeysAction );
    helpMenu->addSeparator();
    helpMenu->addAction( aboutAction );

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
    mainLayout->addWidget( integerButton, 5, 5 );

    // Scientific button area

    sciLayout = new QGridLayout;
    sciLayout->addWidget( sinButton,        0, 0 );
    sciLayout->addWidget( cosButton,        1, 0 );
    sciLayout->addWidget( tanButton,        2, 0 );
    sciLayout->addWidget( logButton,        3, 0 );
    sciLayout->addWidget( lnButton,         4, 0 );
    sciLayout->addWidget( eButton,         5, 0 );
    sciLayout->addWidget( reciprocalButton, 0, 1 );
    sciLayout->addWidget( squareButton,     1, 1 );
    sciLayout->addWidget( expButton,        2, 1 );
    sciLayout->addWidget( squareRootButton, 3, 1 );
    sciLayout->addWidget( nRootButton,      4, 1 );
    sciLayout->addWidget( piButton,         5, 1 );
    sciLayout->setSpacing( 6 );

    // Programming (bit operation) button area

    proLayout = new QGridLayout;
    proLayout->addWidget( bitLeftButton,  0, 0 );
    proLayout->addWidget( bitRightButton, 1, 0 );
    proLayout->addWidget( bitAndButton,   2, 0 );
    proLayout->addWidget( bitOrButton,    3, 0 );
    proLayout->addWidget( bitXorButton,   4, 0 );
    proLayout->addWidget( bitNotButton,   5, 0 );
    proLayout->setSpacing( 6 );

    // Section layout holding all the above button areas

    QHBoxLayout *secLayout = new QHBoxLayout;
    secLayout->setSpacing( 16 );
    secLayout->addLayout( proLayout,  1 );
    secLayout->addLayout( sciLayout,  2 );
    secLayout->addLayout( mainLayout, 6 );

    // Now the root-level layout

    QGridLayout *rootLayout = new QGridLayout;
    rootLayout->setMenuBar( menuBar );

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

    helpInstance = NULL;
    createHelp();

    // Combo-box selections
    modeSelector->setCurrentIndex( (int) isHexMode );
    for ( int i = 0; i < NumHexButtons; i++ )
        hexButtons[ i ]->setVisible( isHexMode );

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


// ---------------------------------------------------------------------------
// DESTRUCTOR
//
Calculator::~Calculator()
{
#ifdef __OS2__
    if ( helpInstance ) OS2Native::destroyNativeHelp( helpInstance );
#endif
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
    long double operand = currentDisplayValue();
    long double result = 0.0;

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
    else if ( clickedOperator == "E") {         // e
        result = M_E;
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
    else if ( clickedOperator == "NOT") {       // 1's complement
        result = ~((qlonglong) operand);
    }
    else if ( clickedOperator == "INT") {       // integer
        //QVariant var( operand );
        setCurrentDisplayValue( (qlonglong) operand );     // var.toLongLong()
        waitingForOperand = true;
        return;
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
    long double operand = currentDisplayValue();

    if ( !pendingExponentialOperator.isEmpty() ) {
        if ( ! calculate( operand, pendingExponentialOperator )) {
            abortOperation();
            return;
        }
        setCurrentDisplayValue( expSoFar );
        operand = expSoFar;
        expSoFar = 0.0;
        pendingExponentialOperator.clear();
    }

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
    long double operand = currentDisplayValue();

    if ( !pendingExponentialOperator.isEmpty() ) {
        if ( ! calculate( operand, pendingExponentialOperator )) {
            abortOperation();
            return;
        }
        setCurrentDisplayValue( expSoFar );
        operand = expSoFar;
        expSoFar = 0.0;
        pendingExponentialOperator.clear();
    }

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
// exponentialOperatorClicked
//
void Calculator::exponentialOperatorClicked()
{
    Button *clickedButton = qobject_cast<Button *>( sender() );
    QString clickedOperator = clickedButton->identity();
    long double operand = currentDisplayValue();

    if ( !pendingExponentialOperator.isEmpty() ) {
        if ( ! calculate( operand, pendingExponentialOperator )) {
            abortOperation();
            return;
        }
        setCurrentDisplayValue( expSoFar );
    } else {
        expSoFar = operand;
    }

    pendingExponentialOperator = clickedOperator;
    waitingForOperand = true;
}


// ---------------------------------------------------------------------------
// equalClicked
//
void Calculator::equalClicked()
{
    long double operand = currentDisplayValue();

    if ( !pendingExponentialOperator.isEmpty() ) {
        if ( ! calculate( operand, pendingExponentialOperator )) {
            abortOperation();
            return;
        }
        operand = expSoFar;
        expSoFar = 0.0;
        pendingExponentialOperator.clear();
    }
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
        qlonglong llValue = text.toULongLong( &ok, 16 );
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
        long double value = currentDisplayValue();
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
//    equalClicked();
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
    long double value = currentDisplayValue();
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
// onTopChanged
//
void Calculator::onTopChanged()
{
    isOnTop = onTopAction->isChecked();
#if 0   //ifdef __OS2__
    HWND hwnd = WinQueryWindow( winId(), QW_OWNER );
    if ( hwnd )
        WinSetWindowBits( hwnd, QWL_STYLE, isOnTop? WS_TOPMOST: 0L, WS_TOPMOST );
#else
    Qt::WindowFlags flags = windowFlags();
    if ( isOnTop )
        flags |= Qt::WindowStaysOnTopHint;
    else
        flags &= ~Qt::WindowStaysOnTopHint;
    setWindowFlags( flags );
    show();
#endif

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

void Calculator::showGeneralHelp()
{
#ifdef __OS2__
    OS2Native::showHelpPanel( helpInstance, HELP_PANEL_GENERAL );
#else
    launchAssistant( HELP_HTML_GENERAL );
#endif
}


void Calculator::showKeysHelp()
{
#ifdef __OS2__
    OS2Native::showHelpPanel( helpInstance, HELP_PANEL_KEYS );
#else
    launchAssistant( HELP_HTML_KEYS );
#endif
}

// ---------------------------------------------------------------------------
// about
//
void Calculator::about()
{
    QMessageBox::about( this,
                        tr("Product Information"),
                        tr("<b>AT Desktop Calculator</b><br>Version %1<hr>"
                           "<p>Copyright &copy; 2018,2022 Alexander Taylor.<br>"
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


/*
void Calculator::contextMenuEvent( QContextMenuEvent *event )
{
    QMenu menu( this );
    menu.addMenu( editMenu );
    menu.addMenu( styleMenu );
#ifndef __OS2__
    menu.addAction( onTopAction );
#endif
    menu.addSeparator();
    menu.addAction( aboutAction );
    menu.exec( event->globalPos() );
#if 0   // ifdef __OS2__
    if ( isOnTop )
        WinSetWindowBits( menu.winId(), QWL_STYLE, WS_TOPMOST, WS_TOPMOST );
#endif
}
*/


void Calculator::keyPressEvent( QKeyEvent *event )
{
    switch ( event->key() ) {
        case Qt::Key_Escape:
            // Prevent Esc from closing window
            event->accept();
            break;
        default:
            break;
    }
}


// ===========================================================================
// OTHER METHODS
//

void Calculator::createHelp()
{
#ifdef __OS2__
    helpInstance = OS2Native::setNativeHelp( this, QString("atcalc"), tr("Calculator Help") );
#else
    helpProcess = new QProcess( this );
#endif
}

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
    button->setShortcut( QKeySequence( key ));
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
    for ( int i = 0; i < allButtons.size(); i++ ) {
        Button *b = allButtons.at( i );
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

    pal.setColor( QPalette::Button, QColor("#A8A8A8"));
    integerButton->setPalette( pal );
    moduloButton->setPalette( pal );
    divisionButton->setPalette( pal );
    timesButton->setPalette( pal );
    minusButton->setPalette( pal );
    plusButton->setPalette( pal );
    equalButton->setPalette( pal );

    if ( !isGrey ) pal.setColor( QPalette::Button, QColor("#A8C8A8"));
    clearMemoryButton->setPalette( pal );
    readMemoryButton->setPalette( pal );
    setMemoryButton->setPalette( pal );
    addToMemoryButton->setPalette( pal );

    if ( !isGrey ) pal.setColor( QPalette::Button, QColor("#A0B0C8"));
    squareButton->setPalette( pal );
    reciprocalButton->setPalette( pal );
    expButton->setPalette( pal );
    squareRootButton->setPalette( pal );
    nRootButton->setPalette( pal );
    piButton->setPalette( pal );
    sinButton->setPalette( pal );
    cosButton->setPalette( pal );
    tanButton->setPalette( pal );
    logButton->setPalette( pal );
    lnButton->setPalette( pal );
    eButton->setPalette( pal );

    if ( !isGrey ) pal.setColor( QPalette::Button, QColor("#C8C8A8"));
    bitLeftButton->setPalette( pal );
    bitRightButton->setPalette( pal );
    bitAndButton->setPalette( pal );
    bitOrButton->setPalette( pal );
    bitXorButton->setPalette( pal );
    bitNotButton->setPalette( pal );
}


// ---------------------------------------------------------------------------
// currentDisplayValue
//
// Returns the current value shown in the display (as long double).
//
long double Calculator::currentDisplayValue()
{
    long double value = 0;
    qlonglong llValue = 0;

    if ( isHexMode ) {
        // TODO try and detect overflow and indicate an unrepresentable value
        // (as it is, any value greater than 2^63 will be returned as modulo)
        llValue = (qlonglong) display->text().toULongLong( 0, 16 );
        value = (long double) llValue;
    }
    else {
        // Need to use stringstream because there's no Qt toLongDouble() method
        std::stringstream ss( display->text().toStdString() );
        ss >> value;
    }
    return value;
}


// ---------------------------------------------------------------------------
// setCurrentDisplayValue
//
// (Overloaded) Updates the display with the current value (either long double or
// qlonglong), along with the alternate-representation field. The numbers will
// be formatted in decimal/hexadecimal as appropriate.
//
void Calculator::setCurrentDisplayValue( long double value )
{
    if ( isHexMode )
        display->setText( QString::number( (qlonglong) value, 16 ).toUpper() );
    else {
        // QString::number doesn't support long double, so use stringstream
        std::stringstream ss;
        ss << value;
        display->setText( QString::fromStdString( ss.str() ));
    }
    updateAltRepr();
}

void Calculator::setCurrentDisplayValue( qlonglong value )
{
    if ( llabs( value ) > 0xFFFFFFFF )
        display->setText( QString::number( value, isHexMode? 16: 10 ).toUpper() );
    else
        display->setText( QString::number( (qint32)value, isHexMode? 16: 10 ).toUpper() );
    updateAltRepr();
}


// ---------------------------------------------------------------------------
// copy
//
void Calculator::copy()
{
    QApplication::clipboard()->setText( display->text() );
}


// ---------------------------------------------------------------------------
// paste
//
void Calculator::paste()
{
    long double value;
    std::stringstream ss( QApplication::clipboard()->text().toStdString() );
    ss >> value;
    if ( waitingForOperand ) {
        display->clear();
        waitingForOperand = false;
    }
    setCurrentDisplayValue( value );
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
bool Calculator::calculate( long double rightOperand, const QString &pendingOperator )
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
        factorSoFar = (qlonglong)factorSoFar % (qlonglong) rightOperand;
    } else if ( pendingOperator == "NEXP") {                // n-exp
        expSoFar = pow( expSoFar, rightOperand );
    } else if ( pendingOperator == "NROOT") {               // n-root
        if ( expSoFar > 0.0 )
            expSoFar = exp( log( expSoFar ) / rightOperand );
        else if ( expSoFar == 0.0 )
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
    qulonglong llValue;
    char achRepr[ 17 ] = {0};
    bool ok;

    if ( isHexMode ) {
        // need to use toULongLong to parse hex in case of signed overflow
        llValue = display->text().toULongLong( &ok, 16 );
        if ( ok ) {
            if ( llValue > 0xFFFFFFFF )
                sprintf( achRepr, "%lld", llValue );
            else
                sprintf( achRepr, "%d", (qint32)llValue );
        }
    }
    else {
        llValue = display->text().toLongLong( &ok, 10 );
        if ( !ok &&
            ( sscanf( display->text().toLocal8Bit().data(), "%lld", &llValue ) == 1 ))
            ok = true;
        if ( ok ) {
            if ( llValue > 0xFFFFFFFF )
                sprintf( achRepr, "%016llX", llValue );
            else
                sprintf( achRepr, "%08X", (qint32)llValue );
        }
        else {
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
    font.fromString( settings.value("editFont", foundFont + ",16").toString() );
    display->setFont( font );

    foundFont = findFont("Helvetica");
    if ( foundFont.isEmpty() )
        foundFont = findFont("Source Sans Pro");
    if ( foundFont.isEmpty() )
        foundFont = "Workplace Sans";
    font.fromString( settings.value("buttonFont", foundFont + ",10").toString() );
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


// ---------------------------------------------------------------------------
// launchAssistant
//
void Calculator::launchAssistant( const QString &panel )
{
    QString assistant = QLibraryInfo::location( QLibraryInfo::BinariesPath )
                        + QLatin1String("/assistant");
    QStringList args;
    args << QLatin1String("-collectionFile")
         << QLatin1String("atcalc.qhc")
         << QLatin1String("-enableRemoteControl");
    helpProcess->start( assistant, args );
    if ( !helpProcess->waitForStarted() ) {
        helpProcess->start( QLatin1String("assistant"), args );
        if ( !helpProcess->waitForStarted() ) {
            QMessageBox::warning( this,
								  tr("Help viewer not available."),
								  tr("QtAssistant could not be opened. To view program help, ensure that "
									 "QtAssistant is installed and available in the program directory or "
									 "in your system path."),
								  QMessageBox::Ok
								);
            return;
        }
    }
    QByteArray assistantInput;
    assistantInput.append("setSource " + QString( HELP_HTML_ROOT ) + panel + QString("\n"));
    helpProcess->write( assistantInput );
}

