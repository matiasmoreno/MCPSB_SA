# Código de Memoria para título de Ingeniero civil Informático
Departamento de informática - UTFSM
Matías Alexander Moreno Pando
201673508-9

## Para compilar y ejecutar el programa
Abrir consola en el mismo directorio del archivo **MCPSB_SA.cpp**. Luego usar comando **make** para compilar el programa.
Para ejecutar el programa escribir en la misma linea de comandos **./MCPSB_SA  + parámetros**
Se pueden manipular las semillas, las instancias y diferentes parametros para la ejecucion del programa en la linea de comando. Los parámetros posibles en orden son:

| Parámetro | Efecto |
| ------ | ------ |
| int **Seed** | Semilla para números aleatorios |
| int **nResets** | Cantidad de reinicios de temperaruta |
| int **nIterations** | Cantidad de iteraciones por cada reinicio |
| float **T0** | Temperatura máxima del sistema |
| float **alpha** | Tasa de enfriamiento |
| float **addP** | Probabilidad de seleccionar movimiento Añadir en diversificación 0-1 |
| float **swapI** | Probabilidad de seleccionar movimiento Swap Externo en intensificación 0-1|
| float **kImprovement** | Fracción de **nIterations** para determinar cambio de fase intensificación/diversificación |
| float **intensificationRatio** | Razón entre total de iteraciones de fases intensificación / diversificación |
| int **sheduling** | Tipo de scheduling para temperatura: 0 - Exponencial, 1 - Lineal, 2 - Sinusoidal, 3 - Doble Sinusoidal, 4 - Triple Sinusoidal |
| string **path** | Ruta del archivo de instancia |

Ejemplo de linea de ejecución:
```sh
$ ./1 200 500000 35 0.99999 0.5 0.5 0.01 0.5 1 MCPSB/Real_Instances/instancia1.0.mcsb
```
