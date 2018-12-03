AT Desktop Calculator
=====================

This is a simple, orthodox desktop calculator written in C++ and Qt.

I developed it because I wanted a useful calculator applet for OS/2, and
all of the available options struck me as lacking in some way or another.

The basis of this program is the Calculator sample program included with
the Qt4 toolkit.  I have revised and extended it considerably.


Functionality
-------------

_AT Desktop Calculator_ supports a selection of standard, scientific, and
programming (bitwise logic) functions.  The scientific and/or programming
controls may be shown or hidden.

Both decimal and hexadecimal input modes are available.  However, real
(floating point) numbers are not supported in hexadecimal mode.

Mathematical order of operations is observed.  For example, entering
`5 [+] 2 [×] 3 [xⁿ] 3` will be evaluated as `5 + (2 × 3³)`, yielding a
result of `59`.  (Bitwise shifts are treated as multiplication operations
for this purpose.)

The calculator fonts are customizable.  Note that if you change the font
used for buttons, you should choose a Unicode-capable font which includes
the necessary symbols.


### Keyboard shortcuts

    0-9, A-F        (Digits)
    Enter (keypad)  Equals
    .               Decimal
    Shift -         Change sign
    /               Divide
    *               Multiply
    -               Subtract
    +               Add
    %               Modulo
    \               Integer conversion
    Shift R         Reciprocal
    Shift S         Square
    Shift X         Exponent (n)
    Shift Q         Square-root
    Shift T         Root (n)
    Shift P         Pi
    Shift E         e
    Shift <         Left bitwise shift
    Shift >         Right bitwise shift
    Shift &         Bitwise AND
    Shift |         Bitwise OR
    Shift ^         Bitwise XOR
    Shift ~         Bitwise NOT
    Backspace       Delete last digit
    Ctrl Backspace  Clear entry
    Ctrl Del        All clear


Notices
-------

    Copyright (C) 2018 Alexander Taylor

    This file is based in part on example code from the Qt4 Toolkit.
    Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).

    All rights reserved.

    You may use this file under the terms of the BSD license as follows:

    "Redistribution and use in source and binary forms, with or without
    modification, are permitted provided that the following conditions are
    met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in
      the documentation and/or other materials provided with the
      distribution.
    * Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
      the names of its contributors may be used to endorse or promote
      products derived from this software without specific prior written
      permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
    A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
    OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
    SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
    LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
    DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
    THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
    (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
    OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."

More information may be found at: https://github.com/altsan/at-calculator/wiki
