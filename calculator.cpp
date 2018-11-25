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
    sumInMemory = 0.0;
    sumSoFar    = 0.0;
    factorSoFar = 0.0;
    waitingForOperand = true;
    isHexMode = false;
    currentView = Standard;

    // Create the entryfield

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

    readSettings();

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

    Button *pointButton      = createButton( tr("."),           ".",      ".",  SLOT( pointClicked() ));
    Button *changeSignButton = createButton( tr("\261"),        "+-",     SLOT( changeSignClicked() ));

    Button *backspaceButton  = createButton( trUtf8("← Back"),"BACK",   QKeySequence( Qt::Key_Backspace ), SLOT( backspaceClicked() ));
    Button *clearButton      = createButton( tr("CE"),          "CE",     QKeySequence::DeleteStartOfWord,   SLOT( clear() ));
    Button *clearAllButton   = createButton( tr("AC"),          "AC",     SLOT( clearAll() ));

    Button *clearMemoryButton = createButton( tr("MC"),         "MC",     SLOT( clearMemory() ));
    Button *readMemoryButton  = createButton( tr("MR"),         "MR",     SLOT( readMemory() ));
    Button *setMemoryButton   = createButton( tr("MS"),         "MS",     SLOT( setMemory() ));
    Button *addToMemoryButton = createButton( tr("M+"),         "M+",     SLOT( addToMemory() ));
//  Button *subFromMemoryButton = createButton( tr("M-"),       "M-",     SLOT(subFromMemory() ));

    Button *divisionButton   = createButton( tr("\367"),        "/",      "/", SLOT(multiplicativeOperatorClicked() ));
    Button *timesButton      = createButton( tr("\327"),        "*",      "*", SLOT(multiplicativeOperatorClicked() ));
    Button *minusButton      = createButton( trUtf8("−"),     "-",      "-", SLOT(additiveOperatorClicked() ));
    Button *plusButton       = createButton( tr("+"),           "+",      "+", SLOT(additiveOperatorClicked() ));
    Button *equalButton      = createButton( tr("="),           "=",      QKeySequence( Qt::Key_Enter ), SLOT( equalClicked() ));

    Button *moduloButton     = createButton( tr("Mod"),         "MOD",    SLOT( multiplicativeOperatorClicked() ));
    Button *squareRootButton = createButton( trUtf8("√"),     "SQRT",   SLOT( unaryOperatorClicked() ));

    Button *reciprocalButton = createButton( tr("1/x"),         "RECIPR", SLOT( unaryOperatorClicked() ));
    Button *squareButton     = createButton( tr("x\262"),       "SQUARE", SLOT( unaryOperatorClicked() ));
    Button *expButton        = createButton( trUtf8("xⁿ"),    "NEXP",   SLOT( multiplicativeOperatorClicked() ));
    Button *nRootButton      = createButton( trUtf8("ⁿ√x"), "NROOT",  SLOT( multiplicativeOperatorClicked() ));
    Button *logButton        = createButton( tr("log"),         "LOG",    SLOT( unaryOperatorClicked() ));
    Button *lnButton         = createButton( tr("ln"),          "LN",     SLOT( unaryOperatorClicked() ));
    Button *piButton         = createButton( trUtf8("π"),      "PI",     SLOT( unaryOperatorClicked() ));
    Button *sinButton        = createButton( tr("sin"),         "SIN",    SLOT( unaryOperatorClicked() ));
    Button *cosButton        = createButton( tr("cos"),         "COS",    SLOT( unaryOperatorClicked() ));
    Button *tanButton        = createButton( tr("tan"),         "TAN",    SLOT( unaryOperatorClicked() ));

    Button *bitLeftButton    = createButton( tr("<<"),          "<<",     SLOT( multiplicativeOperatorClicked() ));
    Button *bitRightButton   = createButton( tr(">>"),          ">>",     SLOT( multiplicativeOperatorClicked() ));
    Button *bitAndButton     = createButton( tr("&&"),          "&",      SLOT( multiplicativeOperatorClicked() ));
    Button *bitOrButton      = createButton( tr("|"),           "|",      SLOT( multiplicativeOperatorClicked() ));
    Button *bitXorButton     = createButton( tr("^"),           "^",      SLOT( multiplicativeOperatorClicked() ));

    QPalette palClr = clearAllButton->palette();
    palClr.setColor( QPalette::Button, QColor("#A04040"));
    palClr.setColor( QPalette::ButtonText, QColor("#FFFFFF"));
    clearAllButton->setPalette( palClr );

    QPalette palMem = clearMemoryButton->palette();
    palMem.setColor( QPalette::Button, QColor("#A8C8A8"));
    clearMemoryButton->setPalette( palMem );
    readMemoryButton->setPalette( palMem );
    setMemoryButton->setPalette( palMem );
    addToMemoryButton->setPalette( palMem );

    QPalette palOps = equalButton->palette();
    palOps.setColor( QPalette::Button, QColor("#A8A8A8"));
    clearButton->setPalette( palOps );
    backspaceButton->setPalette( palOps );
    squareRootButton->setPalette( palOps );
    moduloButton->setPalette( palOps );
    divisionButton->setPalette( palOps );
    timesButton->setPalette( palOps );
    minusButton->setPalette( palOps );
    plusButton->setPalette( palOps );
    equalButton->setPalette( palOps );

    QPalette palSci = squareButton->palette();
    palSci.setColor( QPalette::Button, QColor("#A0B0C8"));
    squareButton->setPalette( palSci );
    reciprocalButton->setPalette( palSci );
    expButton->setPalette( palSci );
    nRootButton->setPalette( palSci );
    piButton->setPalette( palSci );
    sinButton->setPalette( palSci );
    cosButton->setPalette( palSci );
    tanButton->setPalette( palSci );
    logButton->setPalette( palSci );
    lnButton->setPalette( palSci );

    QPalette palPro = bitOrButton->palette();
    palPro.setColor( QPalette::Button, QColor("#C8C8A8"));
    bitLeftButton->setPalette( palPro );
    bitRightButton->setPalette( palPro );
    bitAndButton->setPalette( palPro );
    bitOrButton->setPalette( palPro );
    bitXorButton->setPalette( palPro );


    // Main (always-visible) button area
    //

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
    mainLayout->addWidget( readMemoryButton, 5, 0 );
    mainLayout->addWidget( setMemoryButton, 6, 0 );
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

    mainLayout->addItem( new QSpacerItem( 1, 16 ), 8, 0 );
    mainLayout->addWidget( viewSelector, 9, 0, 1, 3 );
    mainLayout->addWidget( modeSelector, 9, 3, 1, 3 );

    // Scientific button area
    //

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
    //

    proLayout = new QGridLayout;
    proLayout->addWidget( bitLeftButton,  0, 0 );
    proLayout->addWidget( bitRightButton, 1, 0 );
    proLayout->addWidget( bitAndButton,   2, 0 );
    proLayout->addWidget( bitOrButton,    3, 0 );
    proLayout->addWidget( bitXorButton,   4, 0 );
    proLayout->setSpacing( 6 );

    viewSelector->setCurrentIndex( currentView );
    if ( currentView == Programming || currentView == Standard )
        showLayout( sciLayout, false );
    if ( currentView == Scientific || currentView == Standard )
        showLayout( proLayout, false );

    modeSelector->setCurrentIndex( (int) isHexMode );

    // Section layout holding all the above button areas
    //

    QHBoxLayout *secLayout = new QHBoxLayout;
    secLayout->setSpacing( 16 );
    secLayout->addLayout( proLayout,  1 );
    secLayout->addLayout( sciLayout,  2 );
    secLayout->addLayout( mainLayout, 6 );

    // Now the root-level layout
    //
    QGridLayout *rootLayout = new QGridLayout;
    rootLayout->addWidget( display, 0, 0, 1, 2 );
    rootLayout->addWidget( reprName, 1, 0 );
//    rootLayout->addWidget( modeButton, 1, 0 );
    rootLayout->addWidget( reprLabel, 1, 1 );
    rootLayout->addLayout( secLayout, 2, 0, 1, 2 );
    setLayout( rootLayout );

    display->setFocus( Qt::ActiveWindowFocusReason );
    setWindowTitle( tr("Calculator"));
}


// ===========================================================================
// SLOTS
//

// ---------------------------------------------------------------------------
//
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
//
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
//
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
//
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
//
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
//
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
//
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
//
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
//
//
void Calculator::backspaceClicked()
{
    if (waitingForOperand)
        return;

    QString text = display->text();
    text.chop(1);
    if (text.isEmpty()) {
        text = "0";
        waitingForOperand = true;
    }
    if ( isHexMode )
        setCurrentDisplayValue( text.toLongLong( NULL, 16 ));
    else {
        display->setText(text);
        updateAltRepr();
    }
}


// ---------------------------------------------------------------------------
//
//
void Calculator::clear()
{
//    if ( waitingForOperand ) return;
    display->setText("0");
    updateAltRepr();
    waitingForOperand = true;
}


// ---------------------------------------------------------------------------
//
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
//
//
void Calculator::clearMemory()
{
    sumInMemory = 0.0;
}


// ---------------------------------------------------------------------------
//
//
void Calculator::readMemory()
{
    setCurrentDisplayValue( sumInMemory );
    waitingForOperand = true;
}


// ---------------------------------------------------------------------------
//
//
void Calculator::setMemory()
{
    equalClicked();
    sumInMemory = currentDisplayValue();
}


// ---------------------------------------------------------------------------
//
//
void Calculator::addToMemory()
{
    equalClicked();
    sumInMemory += currentDisplayValue();
}


// ---------------------------------------------------------------------------
//
//
void Calculator::subFromMemory()
{
    equalClicked();
    sumInMemory -= currentDisplayValue();
}


// ---------------------------------------------------------------------------
//
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
//
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



// ===========================================================================
// OVERRIDDEN EVENTS
//

void Calculator::closeEvent( QCloseEvent *event )
{
    writeSettings();
    event->accept();
}



// ===========================================================================
// OTHER METHODS
//

// ---------------------------------------------------------------------------
//
//
Button *Calculator::createButton( const QString &text,
                                  const QString &identity,
                                  const char *member )
{
    Button *button = new Button( text );
    button->setFont( btnFont );
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
//
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
//
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
//
//
void Calculator::abortOperation()
{
    clearAll();
    display->setText( tr("####"));
    updateAltRepr();
}


// ---------------------------------------------------------------------------
//
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
//
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
//
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
//
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
//
//
void Calculator::readSettings()
{
    QSettings settings( SETTINGS_VENDOR, SETTINGS_APP );

    reprName->setText( isHexMode? "DEC:": "HEX:");

    currentView = (short) settings.value("view").toInt();
    isHexMode = settings.value("mode").toBool();
    restoreGeometry( settings.value("geometry").toByteArray() );

    // Set up fonts
    editFont = display->font();

    QString foundFont = findFont("Inconsolata");
    if ( foundFont.isEmpty() )
        foundFont = findFont("Source Code Pro");
    if ( !foundFont.isEmpty() )
        editFont.setFamily( foundFont );

    btnFont = editFont;
    editFont.setPointSize( editFont.pointSize() + 8 );

    foundFont = findFont("Helvetica");
    if ( !foundFont.isEmpty() )
        foundFont = findFont("Source Sans Pro");
    if ( !foundFont.isEmpty() ) {
        btnFont.setPointSize( btnFont.pointSize() + 2 );
        btnFont.setFamily( foundFont );
    }
/*
    QFont font("");
    font.fromString( settings.value("editFont", editFont ).toString() );
*/

    display->setFont( editFont );
}


// ---------------------------------------------------------------------------
//
//
void Calculator::writeSettings()
{
    QSettings settings( SETTINGS_VENDOR, SETTINGS_APP );

    settings.setValue("geometry",   saveGeometry() );
    settings.setValue("editFont",   editFont.toString() );
    settings.setValue("buttonFont", btnFont.toString() );
    settings.setValue("view",       currentView );
    settings.setValue("mode",       isHexMode );
}


