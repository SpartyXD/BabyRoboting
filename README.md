# Mini RobotING
Recreacion de la mascota del capitulo de robotica UC (RobotING) version miniatura. 
Sirve de mascota de escritorio o de decoracion :D

![MiniRoboting](https://github.com/user-attachments/assets/c2cc37b5-c42c-429f-b218-fafaed97d9d3) 

![Render](https://github.com/user-attachments/assets/941c0e71-4e7e-445d-9095-c08d5b2c7d5c)

## Piezas utilizadas
* Filamento 3D (Esun PLA+)
* Arduino Nano
* Pantalla OLED SH1106 128x64 de 1.3"
* Buzzer Pasivo
* 2 leds de 5mm (rojo y azul)
* 2 resistencias de 220 ohm
* Cables dupont hembra-hembra pequeños

## Ensamblaje
Se debe imprmir 1 de cada pieza contenida en la carpeta **Piezas 3D**, luego de eso se deben conectar de la siguiente forma los componentes al arduino nano:
| Componente    | Pin           |
| ------------- |:-------------:| 
| LED roja      | resistencia - D6 | 
| LED azul     | resistencia - D5  | 
| Buzzer     | D9  | 
| Pantalla (SDA)     | A4 | 
| Pantalla (SCL)     | A5 | 
| Pantalla (VCC)     | 5V | 
| Todas las tierras (GND)     | GND | 

Luego, se debe pegar con silicona al chasis de manera que sobresalga el buzzer por la parte superior:
![Circuito](https://github.com/user-attachments/assets/4c4e9ba5-fce9-4523-ae25-493b676aa0b3)

Luego de eso hay que juntar y pegar ambas mitades y por ultimo pegar el buzzer al gorrito :)

## Codigo
El codigo viene listo en formato de proyecto de platformIO en la carpeta **Proyecto PlatformIO**, en cuanto a cosas configurables
estan los **DELAYS** de los comportamientos random de los leds y las caras. 
Tiene tambien agregada una libreria propia para manejar de mejor manera componentes externos (_objects.h_)

### Disclaimer
El proyecto original era usando una ESP32 TTGO PLUS, sin embargo la unica placa de esas que tenia se rompio y tenia que tener listo este proyecto para la feria de capitulos como en 2 dias 💀.
Asi que improvise con un arduino nano que tenia y el proceso de pegado quizas no es el mas optimizado, por lo que tratare de trabajar en una version mas amigable en cuanto a espacio se refiere y que sea
mas simple de ensamblar (_este ha sido por lejos de los ensambles mas caoticos que he tenido jskdjs_)
