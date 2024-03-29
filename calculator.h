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

#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QDialog>
#include <QProcess>

// Constants for QtAssistant-based cross-platform help
#define HELP_HTML_ROOT      "qthelp://altsan.atcalc/help/"
#define HELP_HTML_GENERAL   "atcalc.1.html"
#define HELP_HTML_KEYS      "atcalc.3.html"

// Constants for OS/2 native help
#define HELP_PANEL_GENERAL        1
#define HELP_PANEL_KEYS         200


#define DEG_TO_RAD( x )     (x * ( M_PI / 180 ))
#define RAD_TO_DEG( x )     (x * ( 180 / M_PI ))

QT_BEGIN_NAMESPACE
class QMenuBar;
class QLineEdit;
class QLabel;
class QComboBox;
class QGridLayout;
class QMenu;
QT_END_NAMESPACE
class Button;

class Calculator : public QDialog
{
    Q_OBJECT

public:
    Calculator(QWidget *parent = 0);
    ~Calculator();

protected:
    void closeEvent( QCloseEvent *event );
//    void contextMenuEvent( QContextMenuEvent *event );
    void keyPressEvent( QKeyEvent *event );

private slots:
    void modeClicked();
    void digitClicked();
    void unaryOperatorClicked();
    void additiveOperatorClicked();
    void multiplicativeOperatorClicked();
    void exponentialOperatorClicked();
    void equalClicked();
    void pointClicked();
    void changeSignClicked();
    void backspaceClicked();
    void clear();
    void clearAll();
    void clearMemory();
    void readMemory();
    void setMemory();
    void addToMemory();
    void subFromMemory();
    void copy();
    void paste();
    void modeChanged( const QString &text );
    void viewChanged( const QString &text );
    void onTopChanged();
    void greyChanged();
    void buttonFontChanged();
    void displayFontChanged();
    void showGeneralHelp();
    void showKeysHelp();
    void about();

private:
    Button *createButton( const QString &text, const QString &identity, const char *member );
    Button *createButton( const QString &text, const QString &identity, QKeySequence::StandardKey key, const char *member );
    Button *createButton( const QString &text, const QString &identity, const QString &key, const char *member );
    void    setButtonFont( const QFont &font );
    void    setColourScheme();
    void    abortOperation();
    long double  currentDisplayValue();
    void    setCurrentDisplayValue( long double value );
    void    setCurrentDisplayValue( long long value );
    bool    calculate( long double rightOperand, const QString &pendingOperator );
    void    updateAltRepr();
    int     showLayout( const QLayout *layout, bool visible );
    QString findFont( const QString fontFamily );
    void    readSettings();
    void    writeSettings();
    void    createHelp();
    void    launchAssistant( const QString &panel );

    long double sumInMemory;
    long double sumSoFar;
    long double factorSoFar;
    long double expSoFar;
    QString pendingAdditiveOperator;
    QString pendingMultiplicativeOperator;
    QString pendingExponentialOperator;
    bool waitingForOperand;
    bool isOnTop;
    bool isHexMode;
    bool isGrey;

    enum { Standard = 0, Scientific = 1, Programming = 2, All = 3 };
    short currentView;

    QLineEdit *display;
    QLabel *reprName;
    QLabel *reprLabel;
    QComboBox *modeSelector;
    QComboBox *viewSelector;
    QGridLayout *sciLayout;
    QGridLayout *proLayout;

    QMenuBar *menuBar;

    QMenu   *fileMenu;
    QMenu   *editMenu;
    QMenu   *viewMenu;
    QMenu   *styleMenu;
    QMenu   *helpMenu;
    QAction *quitAction;
    QAction *monochromeAction;
    QAction *displayFontAction;
    QAction *buttonFontAction;
    QAction *onTopAction;
    QAction *aboutAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *helpGeneralAction;
    QAction *helpKeysAction;

    enum { NumDigitButtons = 10 };
    enum { NumHexButtons = 6 };
    Button *digitButtons[ NumDigitButtons ];
    Button *hexButtons[ NumHexButtons ];
    Button *pointButton;
    Button *changeSignButton;
    Button *backspaceButton;
    Button *clearButton;
    Button *clearAllButton;
    Button *clearMemoryButton;
    Button *readMemoryButton;
    Button *setMemoryButton;
    Button *addToMemoryButton;
    Button *divisionButton;
    Button *timesButton;
    Button *minusButton;
    Button *plusButton;
    Button *equalButton;
    Button *moduloButton;
    Button *squareRootButton;
    Button *reciprocalButton;
    Button *squareButton;
    Button *expButton;
    Button *nRootButton;
    Button *logButton;
    Button *lnButton;
    Button *eButton;
    Button *piButton;
    Button *sinButton;
    Button *cosButton;
    Button *tanButton;
    Button *bitLeftButton;
    Button *bitRightButton;
    Button *bitAndButton;
    Button *bitOrButton;
    Button *bitXorButton;
    Button *bitNotButton;
    Button *bitLroButton;
    Button *bitRroButton;
    Button *integerButton;

    // Program help (platform specific implementation)
    void *helpInstance;

    // QtAssistant process
    QProcess *helpProcess;

};

#endif
