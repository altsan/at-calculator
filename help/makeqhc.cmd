/* REXX script
 * Generate the QtAssistant collection and compressed help files.
 */
uxroot = VALUE('UNIXROOT',,'OS2ENVIRONMENT')
uxroot'\usr\lib\qt4\bin\qcollectiongenerator atcalc.qhcp -o atcalc.qhc'

