:userdoc.

.*****************************************************************************
:h1 x=left y=bottom width=100% height=100% res=001.Calculadora AT

êsta es una calculadora simple de escritorio dise§ada para ser f†cil de usar.
Admite una selecci¢n de funciones est†ndar, cient°ficas y de programaci¢n
(l¢gica binaria).

:p.La interfaz deber°a serle familiar a cualquiera que haya usado una
calculadora de bolsillo est†ndar. Consulte las siguientes secciones para m†s
informaci¢n sobre el uso de este programa&colon.

:ul compact.
:li.:link reftype=hd res=100.Operar con la calculadora:elink.
:li.:link reftype=hd res=200.Asignaciones de teclas:elink.
:eul.

:p.Tenga en cuenta que este programa no pretende servir como calculadora
cient°fica avanzada. Las funciones cient°ficas incluidas son b†sicas y operan
s¢lo en grados sexagesimales, no centesimales ni en radianes.

:p.Este programa se basa parcialmente en c¢digo del programa calculadora de
ejemplo incluido con el juego de herramientas de Qt4. Consulte la secci¢n
:link res=900 reftype=hd.Avisos:elink. para m†s detalles sobre la licencia de
uso y derechos de copia.

.*----------------------------------------------------------------------------
.* :h2 x=left y=bottom width=100% height=100% res=002.Command-line options
.*** None at present


.*****************************************************************************
:h1 x=left y=bottom width=100% height=100% res=100.Operar con la calculadora

La calculadora admite cuatro conjuntos de funciones&colon.

:dl compact tsize=20.
:dt.Est†ndar
:dd.Funciones de calculadora b†sicas
:dt.Cient°ficas
:dd.Est†ndar m†s varias funciones algebraicas y trigonomÇtricas
:dt.Programaci¢n
:dd.Est†ndar m†s funciones l¢gicas de bits
:dt.Todas
:dd.Todas las anteriores
:edl.

:p.En todos los modos se respeta el orden de precedencia de las operaciones
m†tem†ticas (potenciaci¢n/ra°ces &bxh.&rahead. multiplicaci¢n/divisi¢n
&bxh.&rahead. suma/resta). (En modo de programaci¢n, las operaciones de
desplazamiento de bits se tratan como multiplicaci¢n a este respecto.)

:p.:hp7.Funciones b†sicas:ehp7.
:p.Estos controles est†n disponibles en todos los modos.

:dl compact.
:dt.0-9
:dd.Cifras numÇricas de cero a nueve
:dt.A-F
:dd.Cifras numÇricas de diez a quince (visibles s¢lo en modo hexadecimal)
:dt.+
:dd.Operador de suma
:dt.&minus.
:dd.Operador de resta
:dt.û
:dd.Operador de multiplicaci¢n
:dt.&divide.
:dd.Operador de divisi¢n
:dt.=
:dd.Igual a &ndash. efect£a el c†lculo actual y muestra el resultado.
:dt.Mod
:dd.M¢dulo &ndash. realiza la divisi¢n entera de los operandos y devuelve el resto.
:dt.Int
:dd.Devuelve la parte entera del valor actual (es decir, devuelve el entero inmediatamente inferior)
:edl.

:p.Adem†s la fila superior contiene tres botones para descartar el (los)
valor(es) actual(es)&colon.

:dl compact.
:dt.AC
:dd.All clear (descartar todo) &ndash. descarta el c†lculo actual incuidos
operandos introducidos anteriormente.
:dt.CE
:dd.Clear Entry (descartar entrada) &ndash. descarta el valor mostrado
actualmente en pantalla
:dt.Retroceso :dd.Borra el £ltimo car†cter de la pantalla.
:edl.

:p.:hp7.Funciones cient°ficas:ehp7.

:p.Con estas funciones, :hp1.x:ehp1. como se muestra en el texto de los botones
representa el valor actual. :hp1.n:ehp1., si est† presente, representa el
operando siguiente, que debe introducirse tras pulsar el bot¢n.

:dl compact.
:dt.:artwork runin name='reciprocal.bmp'.
:dd.Inverso &ndash. divide uno por el valor actual.
:dt.:artwork runin name='square.bmp'.
:dd.Cuadrado &ndash. eleva el valor actual a su segunda potencia.
:dt.:artwork runin name='exponent.bmp'.
:dd.Potencia &ndash. eleva el valor actual a la :hp1.n:ehp1.-Çsima potencia.
:dt.:artwork runin name='sqroot.bmp'.
:dd.Ra°z &ndash. calcula la ra°z cuadrada del valor actual.
:dt.:artwork runin name='nroot.bmp'.
:dd.Ra°z-n &ndash. calcula la ra°z :hp1.n:ehp1.-Çsima del valor actual.
:dt.:artwork runin name='pi.bmp'.
:dd.Introduce el valor de la constante matem†tica :hp1.pi:ehp1..
:dt.sen
:dd.Funci¢n seno &ndash. calcula el seno del valor actual tom†ndolo en grados.
:dt.cos
:dd.Funci¢n coseno &ndash. calcula el coseno del valor actual tom†ndolo en grados.
:dt.tan
:dd.Funci¢n tangente &ndash. calcula la tangente del valor actual tom†ndolo en grados.
:dt.log
:dd.Logaritmo &ndash. calcula el logaritmo decimal o en base 10 del valor actual.
:dt.ln
:dd.Logaritmo neperiano &ndash. calcula el logaritmo natural o en base :hp1.e:ehp1. del valor actual.
:dt.e
:dd.Introduce el valor de la constante matem†tica :hp1.e:ehp1..
:edl.

:p.:hp7.Funciones de programaci¢n:ehp7.

:p.Todas las funciones siguiente excepto ÆNOØ necesitan un segundo operando que
se introduce tras pulsar el bot¢n.

:dl compact.
:dt.&lt.&lt.
:dd.Desplazar bits a la izquierda. El valor se desplaza a la izquierda tantos
bits como exprese el siguiente operando.
:dt.&gt.&gt.
:dd.Desplazar bits a la derecha. El valor se desplaza a la derecha tantos bits
como exprese el siguiente operando.
:dt.&amp.
:dd.Y &ndash. realiza una operaci¢n l¢gica Y entre el valor actual y el
siguiente operando.
:dt.|
:dd.O &ndash. realiza una operaci¢n l¢gica O entre el valor actual y el
siguiente operando.
:dt.^
:dd.XOR &ndash. realiza una operaci¢n l¢gica XOR (O exclusivo) entre el valor
actual y el siguiente operando.
:dt.&lnot.
:dd.NO &ndash. calcula el complemento a unos del valor actual.
:edl.


:p.:hp7.Modo de entrada:ehp7.

:p.Se puede seleccionar introducir valores decimales o hexadecimales. Sea cual
sea el modo activo, se mostrar†n los valores en la otra notaci¢n bajo el campo
de entrada.

:p.Tenga en cuenta que la entrada y representaci¢n hexadecimales tienen ciertas
limitaciones&colon.

:ul.
:li.No se pueden representar n£meros reales en hexadecimal. En su lugar, el
n£mero se truncar† en el separador decimal y s¢lo se mostrar† la parte entera.

:li.Tampoco se pueden representar valores absolutos que superen m†s- o menos-
0x7FFF FFFF FFFF FFFF (9.223.372.036.854.775.807). Introducir u obtener un
valor semejante en modo hexadecimal (o cambiar a modo hexadecimal mientras se
muestra un valor de estas caracter°sticas) tendr† un resultado impredecible.
:eul.

:p.Los n£meros negativos en modo hexadecimal se representan en notaci¢n de
complemento a dos (hasta una longitud de 64 bits).


.*****************************************************************************
:h1 x=left y=bottom width=100% height=100% res=200.Asignaciones de teclas

Se admite operar mediante las siguientes teclas y combinaciones.

:dl compact tsize=25 break=fit.
:dt.0-9, A-F
:dd.(Cifras)

:dt.Intro (teclado numÇrico)
:dd.(Cifras)

:dt.&per.
:dd.Decimal

:dt.May£s -
:dd.Cambio de signo

:dt./
:dd.Dividir

:dt.&asterisk.
:dd.Multiplicar

:dt.-
:dd.Restar

:dt.+
:dd.Sumar

:dt.%
:dd.M¢dulo

:dt.\
:dd.Convertir a entero

:dt.May£s R
:dd.Inverso

:dt.May£s S
:dd.Cuadrado

:dt.May£s X
:dd.Potencia (n)

:dt.May£s Q
:dd.Ra°z cuadrada

:dt.May£s T
:dd.Ra°z (n)

:dt.May£s P
:dd.Pi

:dt.May£s E
:dd.e

:dt.May£s <
:dd.Desplazamiento l¢gico a la izquierda

:dt.May£s >
:dd.Desplazamiento l¢gico a la derecha

:dt.May£s &amp.
:dd.Y l¢gico

:dt.May£s |
:dd.O l¢gico

:dt.May£s ^
:dd.XOR (O exclusivo) l¢gico

:dt.May£s ~
:dd.NO l¢gico

:dt.Retroceso
:dd.Borrar la £ltima cifra

:dt.Ctrl Retroceso
:dd.Descartar el £ltimo operando

:dt.Ctrl Supr
:dd.Descartar todo

:edl.



.*****************************************************************************
.* :h1 x=left y=bottom width=100% height=100% res=800.Limitations
.* TODO write me


.*****************************************************************************
:h1 x=left y=bottom width=100% height=100% res=900.Informaci¢n

Este programa se ha escrito en C++ con el juego de herramientas Qt. Compilado
usando Qt4 y GNU Compiler Collection (GCC).

:p.P†gina inicial del proyecto&colon.
https&colon.//github.com/altsan/at-calculator/

:p.
:p.:hp2.Calculadora de escritorio AT:ehp2.
.br
Copyright ∏ 2018 Alexander Taylor

:p.Este programa se basa parcialmente en c¢digo de ejemplo del juego de
herramientas para Qt4. Copyright ∏ 2011 Nokia Corporation y/o sus
subsidiarias.

:p.Todos los derechos reservados.

:p.Traducci¢n al espa§ol de Alfredo Fern†ndez D°az.

:p.Se puede utilizar este programa bajo los tÇrminos de la licencia BSD como
siguen:

:lm margin=4.
:p.Se permiten el uso y redistribuci¢n en forma binaria o de c¢digo fuente, con
o sin modificaciones, siempre que se cumplan las siguientes condiciones:

:ul.
:li.Las redistribuciones de c¢digo fuente deben conservar el aviso anterior
sobre derechos de copia, esta lista de condiciones y el siguiente descargo.
:li.Las redistribuciones binarias deben reproducir el aviso anterior sobre
derechos de copia, esta lista de condiciones y el siguiente descargo en la
documentaci¢n y cualesquiera materiales que acompa§en la distribuci¢n.
:li.Ni el nombre de Nokia Corporation y/o sus subsidiarias ni los nombres de
sus contribuidores podr†n utilizarse para respaldar o promover productos
derivados de este programa sin su permiso espec°fico previo por escrito.
:eul.

:p.LOS DEPOSITARIOS DE LOS DERECHOS DE COPIA Y SUS CONTRIBUIDORES SUMINISTRAN
ESTE PROGRAMA ÆTAL CUALØ, RENUNCIANDO A CUALESQUIERA GARANT÷AS, INCLUYENDO,
PERO SIN LIMITARSE A, LAS IMPL÷CITAS DE COMERCIALIZABILIDAD O IDONEIDAD PARA UN
PROP‡SITO PARTICULAR. BAJO NINGUNA CIRCUNSTANCIA SE RESPONSABILIZARµ AL
DEPOSITARIO DE LOS DERECHOS DE COPIA O SUS CONTRIBUIDORES DE NINGÈN DA•O
DIRECTO, INDIRECTO, INCIDENTAL, ESPECIAL, ILUSTRATIVO O DERIVADO (INCLUYENDO,
PERO SIN LIMITARSE A, EL SUMINISTRO DE MERCANC÷AS O SERVICIOS DE SUSTITUCI‡N;
PêRDIDA DE USO, DATOS, O LUCRO CESANTE; O INTERRUPCI‡N DE ACTIVIDAD COMERCIAL)
DE CUALQUIER FORMA O DEFINICI‡N DE RESPONSABILIDAD, YA SEA CONTRACTUAL,
OBJETIVA O CIVIL (INCLUYENDO LA NEGLIGENCIA U OTRAS CAUSAS) QUE SE DERIVE DE
CUALQUIER FORMA DE LA UTILIZACI‡N DE ESTE PROGRAMA, INCLUSO BAJO ADVERTENCIA DE
LA POSIBILIDAD DE TAL DA•O.
:lm margin=1.
:euserdoc.
