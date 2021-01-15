from readFiles import readInstance, readProductions, readCoordinates, readRoutes

import math
import sys
import numpy as np
from pylab import *
import matplotlib.pyplot as plt
import os.path

def validateMaxCapacity(routes, productions):
    MAX = 30000
    for route in routes:
        load = 0
        for farm in route:
            if farm != 0:
                load += productions[farm][0]
        if load > MAX:
            print("Capacidad máxima excedida en farm", farm)
            return False

    #print("1: Restricción de capacidad satisfecha!")
    return True

def visitedUnique(routes):
    visited = []
    for route in routes:
        for farm in route:
            if farm != 0:
                if farm in visited:
                    print("Granja", farm, "visitada mas de 1 vez!")
                    return False
    #print("2: Las granjas son visitadas solo una vez!")
    return True

def startAndFinishAtPlant(routes):
    PLANT = 0
    for route in routes:
        for i in range(len(route)):
            if i == 0:
                if route[i] != PLANT:
                    "Camión no inicia su ruta en la planta!"
            if i == len(route)-1:
                if route[i] != PLANT:
                    "Camión no termina su ruta en la planta!"

    #print("3: Todos los camiones inician y finalizan su ruta en la planta!")
    return True

def trucksJump(routes):
    #print("4: No hay saltos en el recorrido de los camiones!")
    return True

def getTruckQuality(routes, farmProduction):
    truckQuality = list()
    for route in routes:
        minQ = 1
        for farm in route:
            if farm != 0:
                farmQ = farmProduction[farm][1]
                if farmQ > minQ:
                    minQ = farmQ
        truckQuality.append(minQ)
    #print("8: La leche entregada por un camión es considerada de una sola calidad!")
    return truckQuality

def collectedMilk(routes, farmProduction, truckQuality):
    collected = [0,0,0]

    for r in range(len(routes)):
        tQ = truckQuality[r]
        for farm in routes[r]:
            if farm != 0:
                collected[tQ-1] += farmProduction[farm][0]

    #print("6: Toda leche recolectada se entrega a la planta!")
    return collected

def mixedMilk(collected, demands):
    mixedInPlant = [0,0,0]

    # Quality 3
    mixedInPlant[2] += collected[2]
    if mixedInPlant[2] < demands[2]:
        # Use Q2 to fill Q3
        diff = demands[2] - mixedInPlant[2]

        # Q2 is enough to fill Q3
        if diff < collected[1]:
            mixedInPlant[2] += diff
            collected[1] -= diff
        else: # Q2 is not enough, Use all Q2
            mixedInPlant[2] += collected[1]
            collected[1] = 0

        if mixedInPlant[2] < collected[2]:
            # Use Q1 to fill Q3
            diff = demands[2] - mixedInPlant[2]
            if diff < collected[0]:
                mixedInPlant[2] += diff
                collected[0] -= diff
            else:
                print("No hay suficiente leche para cumplir las demandas de Q3")
    
    # Quality 2
    mixedInPlant[1] += collected[1]
    if mixedInPlant[1] < demands[1]:
        # Use Q1 to fill Q2
        diff = demands[1] - mixedInPlant[1]

        # Q1 is enough to fill Q2
        if diff < collected[0]:
            mixedInPlant[1] += diff
            collected[0] -= diff
        else: # Q2 is not enough, Error
            print("No hay suficiente leche para cumplir las demandas de Q2")

    # Quality 1
    mixedInPlant[0] += collected[0]

    #print("7: La leche que se entrega en la planta puede ser mezclada!")

    for i in range(len(demands)):
        if mixedInPlant[i] < demands[i]:
            print(mixedInPlant[i], demands[i])
            print("**ERROR** No se cumple la demanda para la calidad", i)

    #print("5: Se cumplen las demandas mínimas de leche de la planta!")
    return mixedInPlant

def getRouteCost(routes, cost):
    routeCost = 0
    
    for route in routes:
        for r in range(len(route)):
            if r == len(route)-1:
                break
            routeCost += cost[route[r]][route[r+1]]

    return routeCost

def getProfit(routeCost, mixed, values):
    profit = -routeCost
    for i in range(len(mixed)):
        profit += mixed[i] * values[i]
    return profit

def validateRoute(instance):
    values = [0.03, 0.021, 0.009]
    # Read Data Files
    farmProduction, _, cost, demands = readInstance(True, instance)

    # Read Routes
    routes, outputs = readRoutes(instance, 1000)

    # Validations
    validateMaxCapacity(routes, farmProduction)
    visitedUnique(routes)
    startAndFinishAtPlant(routes)
    trucksJump(routes)
    truckQuality = getTruckQuality(routes, farmProduction)
    collected = collectedMilk(routes, farmProduction, truckQuality)
    mixed = mixedMilk(collected, demands)

    # Profit
    routeCost = getRouteCost(routes, cost)
    profit = getProfit(routeCost, mixed, values)
    tol = 1
    if abs(profit - outputs[0] < tol):
        print("La calidad de la solución es correcta!", outputs[0])
        tol = 1
    else:
        print("La calidad de la solución difiere a la calculada!")

instances = ["1", "1.0", "2", "2.0", "3", "3.0", "4", "4.0", "5", "5.0", "500", "500.0"]
for i in instances:
    print("Instancia", i)
    validateRoute(i)