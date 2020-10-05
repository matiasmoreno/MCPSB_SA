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
| float **addP** | Probabilidad de seleccionar movimiento Añadir en cada iteración |
| float **swapP** | Probabilidad de seleccionar movimiento Swap en cada iteración |
| float **dropP** | Probabilidad de seleccionar movimiento Swap en cada iteración |
| float **kImprovement** | Fracción de **nIterations** para determinar cambio de fase intensificación/diversificación |
| string **path** | Ruta del archivo de instancia |

Ejemplo de linea de ejecución:
```sh
$ ./MCPSB_SA 1 10 500000 80 0.99999 0.33 0.33 0.33 0.01 MCPSB/Real_Instances/instancia1.mcsb
```
