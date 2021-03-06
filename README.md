# Implementación del sensor de movimiento de filamento de Prusa, el PAT9125, en Marlin 2.0.
Hace unos meses adquirí un modulo PAT9125 pensando en poder incorporar este novedoso sensor láser de la Prusa en mi P3Steel donde usaba Marlin 1.1.9.

Estuve revisando el código del firmware de prusa y no conozco tanto Marlin como para incorporar directamente el sensor usando las librerías de Prusa, más usando I2C como protocolo, que es el que usa este módulo (SDA/SCL).

Pero después descubrí que Marlin 2.0 (aunque en la release bug-fix) incorpora una nueva característica mediante la cual se puede controlar los atascos o falta de filamento en base al movimiento del mismo, mediante un encoder.

Se me ocurrio pues usar un modulo Attiny85 que hiciera de interfaz entre el sensor y mi electrónica RAMPS1.4+Mega2560 que corre Marlin 2.0
El Attiny85 recibe los datos del movimiento del sensor (coordenada y) mediante I2C y, siempre que ve cambios en dicho valor, envía pulsos (HIGH/LOW) hacia al pin de deteccion de filamento (normalmente el puerto D4). Además como el módulo Attiny tiene un led 'bultin' que se activa mediante la salida 1, tambén lo enciendo a la vez que el pulso de encoder, lo que sirve de monitor

Encontré en Thingiverse un soporte para el sensor de Prusa https://www.thingiverse.com/thing:3387643, bastante elegante y mínimo, por lo que opté por usarlo y dejar el módulo Attiny separado en otra cajita, que coloqué encima de la caja de la electrónica,  dejando un orificio para el led de actividad. 

Sin embargo el soporte tuve que modificarlo porque el filamento pasaba demasiado cerca del láser, prácticamente tocando, y no detectaba bien, y modifqué el soprote separando el orificio 2 milímetros del láser, con lo que detectaba perfectamente. He probado con filamento negro, blanco y rojo y funciona perfectamente. Me falta probar con transparentes o translúcidos, a ver que tal. Aunque siempre se puede desconectara desde el menú del lcd si algún filamento da problemas.

