# Basketball Bluetooth Score Board
Codigo fuente del firmware para BBSB

El codigo se divide en 3 firmwares para tres modulos:</br></br>
<pre><b>mcoms</b> :: Modulo de comunicaciones</pre></br>
<pre><b>mtime</b> :: Modulo de control de tiempo</pre></br>
<pre><b>mscore</b> :: Modulo de control de puntos</pre>

# Comandos
Los comandos que acepta deben de tener el formato <b>-[COMANDO]#</b>

La lista de comandos es la siguiente:</br></br>
<pre><b>Bn</b> :: Activa el buzzer en el modo <b>n</b>: 0 Toque corto, 1 Toque largo.</pre></br>
<pre><b>Fnn</b> :: Asigna faltas del jugador, <b>nn</b> es el numero de faltas a 2 digitos.</pre></br>
<pre><b>Jnn</b> :: Asigna numero del jugador, <b>nn</b> es el numero de jugador a 2 digitos.</pre></br>
<pre><b>L0</b> :: Asigna 0 al puntaje de local.</pre></br>
<pre><b>L1</b> :: Incrementa 1 al puntaje del local.</pre></br>
<pre><b>L2</b> :: Incrementa 2 al puntaje del local.</pre></br>
<pre><b>L3</b> :: Incrementa 3 al puntaje del local.</pre></br>
<pre><b>LD</b> :: Decrementa 1 al puntaje del local.</pre></br>
<pre><b>LFn</b> :: Asigna el valor <b>n</b> a las faltas de local.</pre></br>
<pre><b>LInnn</b> :: Inicializa el puntaje local, <b>nnn</b> es el numero a inicializar a 3 digitos.</pre></br>
<pre><b>LP</b> :: Asigna la posicion a local.</pre></br>
<pre><b>P</b> :: Pausa el contador de tiempo general.</pre></br>
<pre><b>Q</b> :: Continua con el contador de tiempo general.</pre></br>
<pre><b>Rn</b> :: Enciende el indicador del periodo <b>n</b>.</pre></br>
<pre><b>Snn</b> :: Asigna el contador de tiro a <b>nn</b> segundos.</pre></br>
<pre><b>Tmmss</b> :: Asigna el contador de tiempo a <b>mm</b> minutos y <b>ss</b> segundos.</pre></br>
<pre><b>V0</b> :: Asigna 0 al puntaje de visitante.</pre></br>
<pre><b>V1</b> :: Incrementa 1 al puntaje del visitante.</pre></br>
<pre><b>V2</b> :: Incrementa 2 al puntaje del visitante.</pre></br>
<pre><b>V3</b> :: Incrementa 3 al puntaje del visitante.</pre></br>
<pre><b>VD</b> :: Decrementa 1 al puntaje del visitante.</pre></br>
<pre><b>VFn</b> :: Asigna el valor <b>n</b> a las faltas de visitante.</pre></br>
<pre><b>VInnn</b> :: Inicializa el puntaje visitante, <b>nnn</b> es el numero a inicializar a 3 digitos.</pre></br>
<pre><b>VP</b> :: Asigna la posicion a visitante.</pre></br>
<pre><b>X</b> :: Activa la rutina de prueba.</pre></br>
<pre><b>Z</b> :: Reinicia el dispositivo (los 3 módulos).</pre></br>
