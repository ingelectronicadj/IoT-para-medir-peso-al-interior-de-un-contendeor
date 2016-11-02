# Adquisión de datos del sensor FSR

Se utiliza un sensor que permite detectar físicamente la presión y peso. Son simples de usar y de bajo costo. 

## Midiendo la fuerza o presión con una FSR
La resistencia de este sensor cambia a mayor presión es aplicada. Cuando no hay presión la resistencia tiene a valores muy grandes 
(circuito abierto ), cuando la presión aumenta la resistencia cae . La siguiente gráfica indica aproximadamente la resistencia del 
sensor en diferentes mediciones de fuerza.

![Alt text](forceflex.jpg)

Es importante darse cuenta que la gráfica de la resistencia no es lineal(Es una gráfica logarítmica), y especialmente en mediciones 
baja fuerza esta tiene a su resistencia de circuito abierto.

Debido a que el FSR es basicamente un resistor , no necesitan polarización.

La forma en que esto funciona es que a medida que la resistencia de la FSR disminuye, la resistencia total del FSR y la resistencia 
pulldown disminuye desde aproximadamente 100kohm a 10Kohm.

Tenga en cuenta que nuestro método tiene la resistividad algo lineal, pero no proporciona tensión lineal! Eso es porque el equasion tensión es:

Vo = Vcc ( R / (R + FSR) )

Es decir, el voltaje es proporcional a la inversa de la resistencia FSR.


## Metódo de lectura de voltaje analogo
El mejor metódo para medir un sensor de resistencia es polarizar con Vcc el sensor en un extremo y en el otro un resistor pull-down a tierra.

This wiki uses the [Markdown](http://daringfireball.net/projects/markdown/) syntax.


```
$ git clone https://ojardila@bitbucket.org/ojardila/treschepes-hardware.git/wiki
```


You can also highlight snippets of text (we use the excellent [Pygments][] library).

[Pygments]: http://pygments.org/

```
#!python

def wiki_rocks(text):
    formatter = lambda t: "funky"+t
    return formatter(text)
```


You can check out the source of this page to see how that's done, and make sure to bookmark [the vast library of Pygment lexers][lexers], we accept the 'short name' or the 'mimetype' of anything in there.
[lexers]: http://pygments.org/docs/lexers/

