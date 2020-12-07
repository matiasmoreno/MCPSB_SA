from readFiles import readInstance, readProductions, readCoordinates, readRoutes

import math
import sys
import numpy as np
from pylab import *
import matplotlib.pyplot as plt
import os.path

def plotRoute(instance):
    # Read Data Files
    farmProduction, farmByQuality, cost = readInstance(True, instance)
    productions = readProductions()
    coordOrigin, coordinates = readCoordinates()

    # Read Routes
    routes = readRoutes(instance, 1000)
        
    # Plot figure
    plt.figure(num=None, figsize=(50, 50), dpi=100, facecolor='white', edgecolor='black')
    plt.xlabel('x')
    plt.ylabel('y')

    # Nodo Origen
    scatter(coordOrigin[0], coordOrigin[1], s=100 ,marker='s', c='k')

    # Nodos granjas
    for farm in farmProduction.keys():
        fProd = farmProduction[farm][0]
        quality = farmProduction[farm][1]
        nodeCoord = productions[fProd]
        x = coordinates[nodeCoord][0]
        y = coordinates[nodeCoord][1]
        colorsQuality = ['b', 'g', 'y']
        if quality == 1:
            scatter(x, y, s=30 ,marker='o', c=colorsQuality[0], alpha=0.5)
        elif quality == 2:
            scatter(x, y, s=30 ,marker='o', c=colorsQuality[1], alpha=0.5)
        elif quality == 3:
            scatter(x, y, s=30 ,marker='o', c=colorsQuality[1], alpha=0.5)
        plt.text(x, y, farm, fontsize=8)
        plt.text(x, y-0.003, farmProduction[farm][0], fontsize=8, alpha=1)

    plt.legend(('Deposito', 'Quality 1', 'Quality 2', 'Quality 3'), prop={"size":20})
    
    r = 0
    colors = ['#ed1b0c', '#ed750c', '#8a1ceb', '#006300', '#032082', '#964B00', '#4a4a4a', '#07abab', '#cf55b2']
    for route in routes:
        r+=1
        # Lineas de ruta
        routex = []
        routey = []
        arrow = []
        for i in range(len(route)):
            if route[i] == 0:
                routex.append(coordOrigin[0])
                routey.append(coordOrigin[1])
                if i == 0:
                    arrow.append(coordOrigin[0])
                    arrow.append(coordOrigin[1])
            else:
                fProd = farmProduction[route[i]][0]
                nodeCoord = productions[fProd]
                x = coordinates[nodeCoord][0]
                y = coordinates[nodeCoord][1]
                routex.append(x)
                routey.append(y)
                if i == 1:
                    arrow.append(x)
                    arrow.append(y)
        
        # Costo de ruta
        routeQualityList = []
        for i in range(len(route)-1):
            # Primer nodo
            if i == 0:
                f = route[i+1]
                fProd = farmProduction[f][0]
                nodeCoord = productions[fProd]
                x = coordinates[nodeCoord][0]
                y = coordinates[nodeCoord][1]
                posX = (coordOrigin[0] + x)/2
                posY = (coordOrigin[1] + y)/2
                plt.text(posX, posY, cost[0][f], fontsize=12, alpha=0.7)
            
            # Ultimo nodo
            elif i == len(route)-2:
                f = route[i]
                fProd = farmProduction[f][0]
                routeQualityList.append(farmProduction[f][1])
                nodeCoord = productions[fProd]
                x = coordinates[nodeCoord][0]
                y = coordinates[nodeCoord][1]
                posX = (coordOrigin[0] + x)/2
                posY = (coordOrigin[1] + y)/2
                plt.text(posX, posY, cost[f][0], fontsize=12, alpha=0.7)
            
            # Nodo intermedios
            else:
                f1 = route[i]
                fProd = farmProduction[f1][0]
                routeQualityList.append(farmProduction[f][1])
                nodeCoord = productions[fProd]
                x1 = coordinates[nodeCoord][0]
                y1 = coordinates[nodeCoord][1]

                f2 = route[i+1]
                fProd = farmProduction[f2][0]
                nodeCoord = productions[fProd]
                x2 = coordinates[nodeCoord][0]
                y2 = coordinates[nodeCoord][1]

                posX = (x1 + x2)/2
                posY = (y1 + y2)/2
                plt.text(posX, posY, cost[f1][f2], fontsize=12, alpha=0.7)
        
        routeQuality = max(routeQualityList)
        routeQualityColor = colorsQuality[routeQuality-1]

        plt.arrow(arrow[0], arrow[1], (arrow[2]-arrow[0])*0.2, (arrow[3]-arrow[1])*0.2, width=0.001, edgecolor='black', facecolor=routeQualityColor)
        plt.plot(routex, routey, color=colors[r-1], linestyle='--', linewidth = 1.2)
    
    plt.savefig(os.path.dirname(__file__) + '/Instance{}/Routes{}.0.png'.format(instance, instance))
    print('/Instance{}/Routes{}.png'.format(instance, instance))

for i in range(1,6):
    plotRoute(i)