/* REXX script
 * Convert IPF help source into single-section HTML files.
 */
SIGNAL ON NOVALUE
PARSE ARG target
IF target == '' THEN target = 'atcalc'

PARSE VAR target . '_'langcode
IF langcode <> '' THEN DO
    encodings = 'IBM850 IBM866 SHIFT_JIS'
    languages = 'es ru ja'
    index = WORDPOS( langcode, languages )
    enc = WORD( encodings, index )
END
ELSE enc = 'IBM850'

CALL RxFuncAdd 'SysLoadFuncs', 'REXXUTIL', 'SysLoadFuncs'
CALL SysLoadFuncs

/* Pass 1: convert all IPF files into HTML files.
 */
IF target == '' THEN
    CALL SysFileTree '*.ipf', 'ipfs.', 'FO'
ELSE DO
    ipfs.0 = 1
    ipfs.1 = target'.ipf'
END
h = 0
DO i = 1 TO ipfs.0
    lp = LASTPOS('.', ipfs.i )
    outfile = SUBSTR( ipfs.i, 1, lp ) || 'html'
    'sed -r -f ipfhtml.sed' ipfs.i '|iconv -f' enc '-t UTF-8 >' outfile
    IF STREAM( outfile, 'C', 'QUERY EXISTS') <> '' THEN DO
        h = h + 1
        htms.h = outfile
    END
END
htms.0 = h

/* Pass 2: Split each HTML file up into individual sections.
 */
h = 0
DO i = 1 TO htms.0
    CALL LINEIN htms.i, 1, 0
    secs = 1
    lp = LASTPOS('.', htms.i )
    outfile = SUBSTR( htms.i, 1, lp ) || secs
    DO WHILE LINES( htms.i )
        l = LINEIN( htms.i )
        IF TRANSLATE( STRIP( LEFT( l, 10 ))) == '<!-- BEGIN' THEN DO
            /* Found new section - close current file and start a new one */
            IF secs > 1 THEN DO
                h = h + 1
                hsecs.h = outfile
                CALL LINEOUT outfile, '</body></html>'
                CALL LINEOUT outfile
                outfile = SUBSTR( htms.i, 1, lp ) || secs
                CALL LINEOUT outfile, '<html>'
                CALL LINEOUT outfile, '<head><meta http-equiv="Content-Type" content="text/html; charset=UTF-8"><link rel="stylesheet" type="text/css" href="help.css"></head><body>'
            END
            secs = secs + 1
        END

        IF l == '</html>' THEN
            CALL LINEOUT outfile, '</body>'

        CALL LINEOUT outfile, l

        IF l == '<html>' THEN
            CALL LINEOUT outfile, '<head><meta http-equiv="Content-Type" content="text/html; charset=UTF-8>"<link rel="stylesheet" type="text/css" href="help.css"></head><body>'

        /* Save any target anchors found */
        IF POS('<a name=', l ) <> 0 THEN DO
            PARSE VAR l . '<a name="'a_name'"' .
            anchors.a_name = FILESPEC('NAME', outfile ) || '.html'
        END
    END
    CALL LINEOUT outfile
    h = h + 1
    hsecs.h = outfile
    CALL STREAM htms.i, 'C', 'CLOSE'
    CALL SysFileDelete htms.i
END
hsecs.0 = h

/* Pass 3: Look for links and correct them to point to the anchor in the
 * appropriate file, then write out the final HTML files.
 * NOTE: this only works on the assumption that no more than one link
 * ever appears on a line.
 */
DO i = 1 TO hsecs.0
    outfile = hsecs.i || '.html'
    IF STREAM( outfile, 'C', 'QUERY EXISTS') <> '' THEN CALL SysFileDelete outfile
    DO WHILE LINES( hsecs.i )
        l = LINEIN( hsecs.i )
        IF POS('<a href="#', l ) <> 0 THEN DO
            PARSE VAR l _before '<a href="#'target'"' _after
            IF SYMBOL('anchors.target') == 'VAR' THEN DO
                SAY '['outfile'] Updating link:' target '->' anchors.target
                l = _before || '<a href="'anchors.target'#'target'"' _after
            END
        END
        CALL LINEOUT outfile, l
    END
    CALL LINEOUT outfile
    CALL STREAM hsecs.i, 'C', 'CLOSE'
    CALL SysFileDelete hsecs.i
END

RETURN 0


NOVALUE:
    SAY LEFT( sigl, 8 ) '+++ Uninitialized variable!'
    SAY LEFT( sigl, 8 ) '+++'
EXIT sigl

