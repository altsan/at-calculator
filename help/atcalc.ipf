:userdoc.

.*****************************************************************************
:h1 x=left y=bottom width=100% height=100% res=001.AT Calculator
:p.This is a simple desktop calculator which is designed to be easy to use. It
supports a selection of standard, scientific, and programming (binary logic)
functions.

:p.The interface should be familiar to anyone who has used a standard pocket
calculator. Refer to the following sections for information about using this
program&colon.
:ul compact.
:li.:link reftype=hd res=100.Calculator operation:elink.
:li.:link reftype=hd res=200.Keyboard commands:elink.
:eul.

:p.Note that this program is not intended to serve as an advanced scientific
calculator. The included scientific functions are basic, and operate in
degree units only.

:p.This program is based in part on code from the Calculator sample program
included with the Qt4 toolkit. Refer to the :link res=900 reftype=hd.Notices:elink.
section for copyright and license details.


.*----------------------------------------------------------------------------
.* :h2 x=left y=bottom width=100% height=100% res=002.Command-line options
.*** None at present


.*****************************************************************************
:h1 x=left y=bottom width=100% height=100% res=100.Calculator Operation
:p.The calculator supports four display modes&colon.
:dl compact tsize=20.
:dt.Standard
:dd.Basic calculator functions
:dt.Scientific
:dd.Standard plus various algebraic and trigonometric functions
:dt.Programming
:dd.Standard plus bitwise logic functions
:dt.All
:dd.All of the above
:edl.

:p.Mathematical order of operations (exponentiation/root &bxh.&rahead.
multiplication/division &bxh.&rahead. addition/subtraction) is observed in
all modes. (In programmer mode, bitwise shift operations are treated as
multiplication operations for this purpose.)


:p.:hp7.Basic functions:ehp7.
:p.These controls are available in all modes.

:dl compact.
:dt.0-9
:dd.Numeric digits zero through nine
:dt.A-F
:dd.(Only visible in hexadecimal mode) numeric digits ten through fifteen
:dt.+
:dd.Addition operator
:dt.&minus.
:dd.Subtraction operator
:dt.ž
:dd.Multiplication operator
:dt.&divide.
:dd.Division operator
:dt.=
:dd.Equals &ndash. completes the current calculation and displays the result.
:dt.Mod
:dd.Modulo &ndash. performs an integer division on the operands and returns the remainder.
:dt.Int
:dd.Returns the integer portion of the current value (i.e. rounds it down to a whole number).
:edl.

:p.In addition, the topmost row contains three buttons for clearing the current
value(s)&colon.
:dl compact.
:dt.AC
:dd.All clear &ndash. completely clears the current calculation, including previously-entered
operands.
:dt.CE
:dd.Clear Entry &ndash. clears the current value in the display field.
:dt.Back
:dd.Deletes the last digit in the display field.
:edl.


:p.:hp7.Scientific functions:ehp7.
:p.With these functions, :hp1.x:ehp1. as shown in the button text represents
the current value.  :hp1.n:ehp1., if present, represents a following operand
to be entered after selecting the button.
:dl compact.
:dt.:artwork runin name='reciprocal.bmp'.
:dd.Reciprocal &ndash. divides one by the current value.
:dt.:artwork runin name='square.bmp'.
:dd.Square &ndash. takes the current value to the power of 2.
:dt.:artwork runin name='exponent.bmp'.
:dd.Exponent &ndash. takes the current value to the power of :hp1.n:ehp1.
:dt.:artwork runin name='sqroot.bmp'.
:dd.Square root &ndash. calculates root 2 of the current value.
:dt.:artwork runin name='nroot.bmp'.
:dd.Root &ndash. calculates the :hp1.n:ehp1.th root of the current value.
:dt.:artwork runin name='pi.bmp'.
:dd.Inputs the value of the mathemetical constant :hp1.pi:ehp1.
:dt.sin
:dd.Sine function &ndash. calculates the sine (in degrees) of the current value.
:dt.cos
:dd.Cosine function &ndash. calculates the cosine (in degrees) of the current value.
:dt.tan
:dd.Tangent function &ndash. calculates the tangent (in degrees) of the current value.
:dt.log
:dd.Common logarithm &ndash. calculates the base 10 logarithm of the current value.
:dt.ln
:dd.Natural logarithm &ndash. calculates the base :hp1.e:ehp1. logarithm of the current value.
:dt.e
:dd.Inputs the value of the mathematical constant :hp1.e:ehp1.
:edl.

:p.:hp7.Programming functions:ehp7.
:p.All of the following functions except for NOT take a following operand after
selecting the button.
:dl compact.
:dt.&lt.&lt.
:dd.Binary shift left. The value is shifted left by a number of bits equal to the following operand.
:dt.&gt.&gt.
:dd.Binary shift right. The value is shifted right by a number of bits equal to the following operand.
:dt.&amp.
:dd.AND &ndash. performs a bitwise AND between the current value and the following operand.
:dt.|
:dd.OR &ndash. performs a bitwise OR between the current value and the following operand.
:dt.^
:dd.XOR &ndash. performs a bitwise eXclusive-OR between the current value and the following operand.
:dt.&lnot.
:dd.NOT &ndash. applies ones-complement negation to the current value.
:edl.


:p.:hp7.Input mode:ehp7.

:p.You can select either decimal or hexadecimal input mode. Whichever mode is
active, the other notation will be displayed underneath the entry field.

:p.Note that hexadecimal representation and input have certain limitations&colon.
:ul.
:li.Real numbers cannot be represented in hexadecimal. Instead, the number will
be truncated at the decimal point, and only the whole-number (integer) portion
will be displayed.
:li.Absolute values exceeding plus or minus 0x7FFF FFFF FFFF FFFF
(9,223,372,036,854,775,807) cannot be represented either. Entering or deriving
such a number while in hexadecimal mode (or switching to hexadecimal mode while
such a number is entered) will have unpredictable results.
:eul.
:p.Negative numbers in hexadecimal mode are represented in twos-complement notation
(up to 64-bit width).


.*****************************************************************************
:h1 x=left y=bottom width=100% height=100% res=200.Keyboard Commands
:p.The following keyboard commands and shortcuts are supported.

:dl compact tsize=20 break=fit.
:dt.0-9, A-F
:dd.(Digits)

:dt.Enter (keypad)
:dd.(Digits)

:dt.&per.
:dd.Decimal

:dt.Shift -
:dd.Change sign

:dt./
:dd.Divide

:dt.&asterisk.
:dd.Multiply

:dt.-
:dd.Subtract

:dt.+
:dd.Add

:dt.%
:dd.Modulo

:dt.\
:dd.Integer conversion

:dt.Shift R
:dd.Reciprocal

:dt.Shift S
:dd.Square

:dt.Shift X
:dd.Exponent (n)

:dt.Shift Q
:dd.Square-root

:dt.Shift T
:dd.Root (n)

:dt.Shift P
:dd.Pi

:dt.Shift E
:dd.e

:dt.Shift <
:dd.Left bitwise shift

:dt.Shift >
:dd.Right bitwise shift

:dt.Shift &amp.
:dd.Bitwise AND

:dt.Shift |
:dd.Bitwise OR

:dt.Shift ^
:dd.Bitwise XOR

:dt.Shift ~
:dd.Bitwise NOT

:dt.Backspace
:dd.Delete last digit

:dt.Ctrl Backspace
:dd.Clear entry

:dt.Ctrl Del
:dd.All clear

:edl.



.*****************************************************************************
.* :h1 x=left y=bottom width=100% height=100% res=800.Limitations
.* TODO write me


.*****************************************************************************
:h1 x=left y=bottom width=100% height=100% res=900.Notices
:p.This program is written in C++ using the Qt toolkit. It has been
built using Qt4 and the GNU Compiler Collection (GCC).

:p.Project home page&colon. https&colon.//github.com/altsan/at-calculator/
:p.

:p.:hp2.AT Desktop Calculator:ehp2.
.br
Copyright (C) 2018 Alexander Taylor

:p.This file is based in part on example code from the Qt4 Toolkit.
Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).

:p.All rights reserved.

:p.You may use this file under the terms of the BSD license as follows:

:lm margin=4.
:p.Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are
met:
:ul.
:li.Redistributions of source code must retain the above copyright
notice, this list of conditions and the following disclaimer.
:li.Redistributions in binary form must reproduce the above copyright
notice, this list of conditions and the following disclaimer in
the documentation and/or other materials provided with the
distribution.
:li.Neither the name of Nokia Corporation and its Subsidiary(-ies) nor
the names of its contributors may be used to endorse or promote
products derived from this software without specific prior written
permission.
:eul.

:p.THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
"AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
:lm margin=1.
:euserdoc.
