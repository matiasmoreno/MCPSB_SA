// Entrega 3 IAA - Matias Moreno - 201673508-9

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <bits/stdc++.h>
#include <chrono> 
#include <math.h>    
#include <vector>
#include <algorithm>
using namespace std;

// Random integers generator

#define srand48(x) srand((int)(x))
#define drand48() ((double)rand()/RAND_MAX)

int realAll = 0;
int realInstance = 1;
int firstInstance = 1, lastInstance = 6;
int firstSeed = 0, lastSeed = 5;
int randLength = 5;
int nResets = 5000, nIterations = 10000;
//int w = 50;
float T0 = 10000;
float alpha = 1;
float addP = 0.20;
float swapP = 0.60;
float alphaCtrl = 1;
int Seed;

void Capture_Params(int argc, char **argv){
    realInstance = atoi(argv[1]);
    firstInstance = atoi(argv[2]);
    lastInstance = atoi(argv[3]);
    firstSeed = atoi(argv[4]);
    lastSeed = atoi(argv[5]);
    randLength = atoi(argv[6]);
    nResets = atoi(argv[7]);
    nIterations = atoi(argv[8]);
    T0 = atof(argv[9]);
    alpha = atof(argv[10]);
    addP = atof(argv[11]);
    swapP = atof(argv[12]);
    alphaCtrl = atof(argv[13]);
    //w = atoi(argv[14]);
}

float float_rand(float a, float b) {
    float retorno = 0;

    if (a < b) {
        retorno = (float) ((b - a) * drand48());
        retorno = retorno + a;
    } else {
        retorno = (float) ((a - b) * drand48());
        retorno = retorno + b;
    }

    return retorno;
}

int int_rand(int a, int b){
    int retorno = 0;

    if (a < b){
        retorno = (int) ((b - a) * drand48());
        retorno = retorno + a;
    }

    else{
        retorno = (int) ((a - b) * drand48());
        retorno = retorno + b;
    }
    if (retorno == b)
    {
      retorno -= 1;
    }
    return retorno;
}

void getRealPrize(int realPrize[], vector <int> routes[], int nTrucks, int nQualities, int oProd[], int farmQuality[])
{
  int i, j, load, minQ;
  for (i = 0; i < nQualities; i++)
  {
    realPrize[i] = 0;
  }
  
  for (i = 1; i < nTrucks; i++)
  {
    load = 0;
    minQ = 1;
    if (int(routes[i].size() > 2))
    {
      for (j = 1; j < int(routes[i].size() - 1); j++)
      {
        load += oProd[routes[i][j]];
        if (farmQuality[ routes[i][j] ] > minQ)
        {
          minQ = farmQuality[ routes[i][j] ];
        }
      }
      realPrize[minQ] += load;
    }
  }
}

void bestBlend(int finalPrize[], int realPrize[], int minPrize[])
{
  int diff;
  finalPrize[3] = realPrize[3];
  finalPrize[2] = realPrize[2];
  finalPrize[1] = realPrize[1];
  if (finalPrize[3] < minPrize[3])
  {
    // Completar la diferencia con leche de calidad 2 y 1 si es necesario
    diff = minPrize[3] - finalPrize[3];
    if (finalPrize[2] >= diff)
    {
      finalPrize[2] -= diff;
    }
    else
    {
      finalPrize[1] -= diff - finalPrize[2];
      finalPrize[2] -= 0;
    }
    finalPrize[3] += diff;
  }

  // Verificar si se cumple la restriccion para la leche 2
  if (finalPrize[2] < minPrize[2])
  {
    // Completar la diferencia con leche de calidad 1
    diff = minPrize[2] - finalPrize[2];
    finalPrize[2] += diff;
    finalPrize[1] -= diff;
  }
}

void getCapacity(int capacity[], int oCap[], int oProd[], vector <int> routes[], int nTrucks){
  int i, j;
  for (i = 1; i < nTrucks; i++)
  {
    capacity[i] = oCap[i];
    for (j = 1; j < int(routes[i].size() - 1); j++)
    {
      capacity[i] -= oProd[routes[i][j]];
    }
  }
}

void getProduction(int production[], int oProd[], vector <int> routes[], int nTrucks, int nFarms){
  int i, j;
  for (i = 0; i < nFarms; i++)
  {
    production[i] = oProd[i];
  }
  
  for (i = 1; i < nTrucks; i++)
  {
    for (j = 1; j < int(routes[i].size() - 1); j++)
    {
      production[routes[i][j]] = 0;
    }
  }
}

void getLoadQuality(int loadQuality[], int farmQuality[], vector <int> routes[], int nTrucks){
  int i, j, minQ;
  for (i = 1; i < nTrucks; i++)
  {
    minQ = 1;
    for (j = 1; j < int(routes[i].size() - 1); j++)
    {
      if (farmQuality[routes[i][j]] > minQ)
      {
        minQ = farmQuality[routes[i][j]];
      }
    }
    loadQuality[i] = minQ;
  }
}

void measureDist(int distance[], int nTrucks, vector <int> routes[], int **cost)
{
  int dist, i , j;
  for (i = 1; i < nTrucks; i++)
  {
    dist = 0;
    if (routes[i].size() > 2)
    {
      // origen - primera
      dist += cost[ 0 ][ routes[i][1] ];
      for (j = 1; j < int(routes[i].size() - 1); j++)
      {
        // ultima - origen
        if (j == int(routes[i].size() - 2))
        {
          dist += cost[ routes[i][j] ][ 0 ];
        }
        else
        {
          // actual - siguiente
          dist += cost[ routes[i][j] ][ routes[i][j + 1] ];
        }
      }
    }
    distance[i] = dist;
  }
}

// Funcion de factibilidad
bool feasible(int realPrize[], int minPrize[], vector <int> routes[])
{
  int av2, av3;

  // Factibilidad en recolección mínima

  // Checkear factibilidad en calidad 1
  if (realPrize[1] < minPrize[1])
  {
    return false;
  }
  else
  {
    // Factibilidad en calidad 2
    av2 = realPrize[2] + realPrize[1] - minPrize[1];
    if (av2 < minPrize[2])
    {
      return false;
    }
    else
    {
      // Factibilidad en calidad 3
      av3 = realPrize[3] + av2 - minPrize[2];
      if (av3 < minPrize[3])
      {
        return false;
      }
    }
  }
 
  return true;
}

// Función de evaluación
float eval(int realPrize[], int minPrize[], float profit[], int ** cost, int nTrucks, vector <int> routes[], float& actualIncome, float& actualCost)
{
  float quality = 0;
  int finalPrize[4], i;
  bestBlend(finalPrize, realPrize, minPrize);
  
  for (i = 1; i < 4; i++)
  {
    quality += finalPrize[i] * profit[i];
  }
  actualIncome = quality;
  // Costo de rutas
  for (i = 1; i < nTrucks; i++)
  {
    if (routes[i].size() > 2)
    {
      // origen - primera
      quality -= cost[ 0 ][ routes[i][1] ];
      for (int j = 1; j < int(routes[i].size() - 1); j++)
      {
        // ultima - origen
        if (j == int(routes[i].size() - 2))
        {
          quality -= cost[ routes[i][j] ][ 0 ];
        }
        else
        {
          // actual - siguiente
          quality -= cost[ routes[i][j] ] [routes[i][j + 1] ];
        }
      }
    }
  }
  actualCost = actualIncome - quality;
  return quality;
}

// Seleccionar mejor granja externa
int getTopRandomExternalFarm(vector <int> routes[], int **cost, int production[], int oProd [], int farmQuality[], int T, int rFarm, int nTrucks, int nFarms, int nQualities, int randLenght, float profit[], int capacity[], vector<int> iQualityFarms[], int origin){
  int j, k, loadQ = 1;
  // Obtener calidad de la mezcla en el camion T
  for (j = 1; j < int(routes[T].size() - 1); j++)
  {
    if (farmQuality[ routes[T][j] ] > loadQ)
    {
      loadQ = farmQuality[ routes[T][j] ];
    }
  }
  
  // Busco en las granjas de la calidad actual
  int available = iQualityFarms[loadQ].size(), realAvailable = 0;
  // No quedan mas granjas para visitar
  if (available == 0)
  {
    return 0;
  }
  
  // Obtener las granjas posibles y sus beneficios
  int farmValues[available], availableFarms[available];
  int from = routes[T][rFarm - 1];
  int to = routes[T][rFarm + 1];
  if (from == origin)
  {
    from = 0;
  }
  if (to == origin)
  {
    to = 0;
  }
  
  for (j = 0; j < available; j++)
  {
    if ((production[iQualityFarms[loadQ][j]] > 0) && ( (capacity[T] + oProd[ routes[T][rFarm] ]) >= production[iQualityFarms[loadQ][j]]))
    {
      farmValues[realAvailable] = production[iQualityFarms[loadQ][j]] * profit[loadQ] - cost[from][iQualityFarms[loadQ][j]] - cost[iQualityFarms[loadQ][j]][to];
      availableFarms[realAvailable] = iQualityFarms[loadQ][j];
      realAvailable++;
    }
  }

  if (realAvailable == 0)
  {
    return 0;
  }

  // Generar las mejores randLenght granjas
  int l = (realAvailable < randLenght) ? realAvailable : randLenght; 
  int maxValues[l], pickFarm[l];
  for (j = 0; j < l; j++)
  {
    maxValues[j] = -999999;
  }
  int minValue, minPosition;
  // j son todas las granjas disponibles
  for (j = 0; j < realAvailable; j++)
  {
    minValue = 999999;
    // k son los elementos de la lista de pick
    for (k = 0; k < l; k++)
    {
      // Busco el valor minimo dentro de maxValues
      if (maxValues[k] < minValue)
      {
        minValue = maxValues[k];
        minPosition = k;
      }
    }
    // Si mi farmValues[j] es mayor que el valor minimo, lo reemplazo y guardo la granja en pickFarm
    if (farmValues[j] > minValue)
    {
      maxValues[minPosition] = farmValues[j];
      pickFarm[minPosition] = availableFarms[j];
    }
  }
  // Tomo una granja aleatoria en pickFarm para retornarla
  return pickFarm[int_rand(0, l)];
}

// Función miope
void miopeRand(int randLenght, int i, vector<int> iQualityFarms[], int T, int& randFarm, int capacity[], int oCap[], int position[], int production[], float profit[], int **cost){
  int j, k;

  // Caso especial instancias de juguete para produccion igual a la capacidad del camion en calidad mas alta
  if (i == 1)
  {
    if (capacity[T] == oCap[T])
    {
      for (j = 0; j < int(iQualityFarms[i].size()); j++)
      {
        if (production[ iQualityFarms[i][j] ] == capacity[T])
        {
          randFarm = iQualityFarms[i][j];
          return;
        }
      }
    }
  }

  // Busco en las granjas de la calidad actual
  int available = iQualityFarms[i].size(), realAvailable = 0;
  // No quedan mas granjas para visitar
  if (available == 0)
  {
    randFarm = 0;
    return;
  }
  // Obtener las granjas posibles y sus beneficios
  int farmValues[available], availableFarms[available];
  for (j = 0; j < available; j++)
  {
    if ((production[iQualityFarms[i][j]] > 0) && (capacity[T] >= production[iQualityFarms[i][j]]))
    {
      farmValues[realAvailable] = production[iQualityFarms[i][j]] * profit[i] - cost[position[T]][iQualityFarms[i][j]];
      availableFarms[realAvailable] = iQualityFarms[i][j];
      realAvailable++;
    }
  }

  if (realAvailable == 0)
  {
    randFarm = 0;
    return;
  }

  // Generar las mejores randLenght granjas
  int l = (realAvailable < randLenght) ? realAvailable : randLenght; 
  int maxValues[l], pickFarm[l];
  for (j = 0; j < l; j++)
  {
    maxValues[j] = -999999;
  }
  int minValue, minPosition;
  // j son todas las granjas disponibles
  for (j = 0; j < realAvailable; j++)
  {
    minValue = 999999;
    // k son los elementos de la lista de pick
    for (k = 0; k < l; k++)
    {
      // Busco el valor minimo dentro de maxValues
      if (maxValues[k] < minValue)
      {
        minValue = maxValues[k];
        minPosition = k;
      }
    }
    // Si mi farmValues[j] es mayor que el valor minimo, lo reemplazo y guardo la granja en pickFarm
    if (farmValues[j] > minValue)
    {
      maxValues[minPosition] = farmValues[j];
      pickFarm[minPosition] = availableFarms[j];
    }
  }
  // Tomo una granja aleatoria en pickFarm para retornarla
  randFarm = pickFarm[int_rand(0, l)];
}

//Main
int main(int argc, char** argv)
{
  Capture_Params(argc,argv);
  ifstream inFile;
  if (realAll == 1)
  {
    firstInstance = 1;
    lastInstance = 2;
  }
  int in;
  for (in = firstInstance; in < lastInstance; in++){
    cout << "Instancia: " << in << endl;
    int nFarms, nTrucks, i, j, origin;
    
    string line;
    string word;

    if (realAll == 1)
    {
      inFile.open("MCPSB/Real_Instances/instanciaALL.mcsb");
    }
    else
    {
      if (realInstance == 1)
      {
        inFile.open("MCPSB/Real_Instances/instancia" + to_string(in) + ".mcsb");
      }
      else
      {
        inFile.open("MCPSB/Instances/instancia" + to_string(in) + ".mcsb");
      }
    }

    // Obtener nodo origin

    while (inFile >> word)
    {
      if (word == "-")
      {
        inFile >> word;
        origin = stoi(word);
        break;
      }
    }

    // Obtener tamaño de arreglos

    // LLegar hasta predios productores
    while (inFile >> word)
    {
      if (word == "I:=")
      {
        break;
      }
    }

    nFarms = 1;
    // Recorrer predios
    while (inFile >> word)
    {
      nFarms = nFarms + 1;
      size_t found = word.find(";");
      if (found != string::npos) 
      {
        break;
      }
    }

    // LLegar hasta camiones
    while (inFile >> word)
    {
      if (word == "K:=")
      {
        break;
      }
    }

    // Recorrer camiones
    nTrucks = 1;
    while (inFile >> word)
    {
      nTrucks = nTrucks + 1;
      size_t found = word.find(";");
      if (found != string::npos) 
      {
        break;
      }
    }

    int capacity [nTrucks], oCap [nTrucks], position [nTrucks], loadQuality [nTrucks], production [nFarms], oProd [nFarms], farmQuality [nFarms];
    int **cost;

    cost = new int *[nFarms];
    for(i = 0; i < nFarms; i++)
    {
      cost[i] = new int[nFarms];
    }

    // LLegar hasta #capacidades
    while (inFile >> word)
    {
      if (word == "param")
      {
        break;
      }
    }

    // Salta 'Q:='
    std::getline(inFile, line);

    // Obtener capacidades
    i = 1;
    inFile >> word;
    while (word != ";")
    {
      // Capacidad
      inFile >> word;
      capacity[i] = stoi(word);
      oCap[i] = stoi(word);
      // N° Camion o ;
      inFile >> word;
      i = i + 1;
    }
    
    // Cantidades a recolectar
    int nQualities = 4;
    int minPrize [nQualities];
    while (inFile >> word)
    {
      if (word == "P:=")
      {
        for (int i = 1; i < nQualities; i++)
        {
          inFile >> word;
          minPrize[i] = stoi(word);
        }
        break;
      }
    }

    // Beneficio por calidad de leche

    float profit [nQualities] = {0, 0.03, 0.021, 0.009};

    // LLegar hasta #producción
    while (inFile >> word)
    {
      if (word == "param")
      {
        break;
      }
    }
    // Salto qu :=
    std::getline(inFile, line);
    // Salto producción de planta 0
    std::getline(inFile, line);

    // Obtener producciones
    i = 1;
    inFile >> word;
    while (word != ";")
    {
      // Produccion
      inFile >> word;
      production[i] = stoi(word);
      oProd[i] = stoi(word);
      // Calidad
      inFile >> word;
      farmQuality[i] = stoi(word);
      // N° granja ;
      inFile >> word;
      i = i + 1;
    }

    // LLegar hasta #costos :
    while (inFile >> word)
    {
      if (word == ":")
      {
        break;
      }
    }
    
    // Salto primera fila
    std::getline(inFile, line);

    for (i = 0; i < nFarms; i++)
    {
      // Salto la primera palabra de cada fila (La que corresponde al nodo de salida)
      inFile >> word;
      for (j = 0; j < nFarms; j++)
      {
        inFile >> word;
        if (i == j)
        {
          cost[i][j] = 0;
        }
        else
        {
          cost[i][j] = stoi(word);
        }
      }
    }

    inFile.close();

    // Record start time
    auto start = std::chrono::high_resolution_clock::now();
    ofstream outFile;
    if (realAll == 1)
    {
      outFile.open("outputsReal/ALL.txt");
    }
    else
    {
      if (realInstance == 1)
      {
        outFile.open ("outputsReal/" + to_string(in) + ".txt");
      }
      else
      {
        outFile.open ("outputs/" + to_string(in) + ".txt");
      }
    }

    outFile << "Instance " << in << "  nF: " << nFarms - 1 << "  nT: " << nTrucks - 1 << "  Min 1-2-3: " << minPrize[1] << "-" << minPrize[2] << "-" << minPrize[3] << endl;
    
    for (Seed = firstSeed; Seed < lastSeed; Seed++)
    {
      start = std::chrono::high_resolution_clock::now();
      outFile << endl << "Seed " << Seed << endl;
      cout << "Seed " << Seed << endl;
      srand48(Seed);

      // Crear nTrucks vectores dinamicos para generar las rutas de cada camion

      vector <int> actualRoutes[nTrucks];

      // Crear nQualities vectores dinamicos para agrupar granjas por calidad

      vector <int> iQualityFarms[nQualities];

      for (i = 1; i < nFarms; i++)
      {
        iQualityFarms[farmQuality[i]].push_back(i);
      }

      // Inicializar posiciones, carga y calidad de leche de los camiones en el origen: nodo 0

      for (i = 1; i < nTrucks; i++)
      {
        position[i] = 0;
        actualRoutes[i].push_back(origin);
        capacity[i] = oCap[i];
      }

      for (i = 1; i < nFarms; i++)
      {
        production[i] = oProd[i];
      }
      
      int randFarm, actualPrize [nQualities], actualRealPrize [nQualities], T = 1, excess;
      for (i = 0; i < nQualities; i++)
      {
        actualPrize[i] = 0;
      }

      for (i = 1; i < nQualities; i++)
      {
        for (; T < nTrucks; T++)
        {
          // Si con el excedente anterior ya llegue al mínimo, salto a la siguiente calidad
          if (minPrize[i] <= actualPrize[i])
          {
            break;
          }

          // Repetir ciclo hasta llenar el camión de producto
          while (true)
          {
            miopeRand(randLength, i, iQualityFarms, T, randFarm, capacity, oCap, position, production, profit, cost);
            // Cada uno de estos ciclos termina cuando el camión se llena de producto
            if (randFarm == 0 && i == 2)
            {
              miopeRand(randLength, 1, iQualityFarms, T, randFarm, capacity, oCap, position, production, profit, cost);
              if (randFarm == 0)
              {
                break;
              }
            }

            if (randFarm == 0 && i == 3)
            {
              miopeRand(randLength, 2, iQualityFarms, T, randFarm, capacity, oCap, position, production, profit, cost);
              if (randFarm == 0)
              {
                miopeRand(randLength, 1, iQualityFarms, T, randFarm, capacity, oCap, position, production, profit, cost);
                if (randFarm == 0)
                {
                  break;
                }
              }
            }
            if (randFarm == 0)
            {
              break;
            }
            
            actualRoutes[T].push_back(randFarm);
            position[T] = randFarm;
            capacity[T] -= production[randFarm];
            actualPrize[i] += production[randFarm];
            production[randFarm] = 0;
          }
          
          // Si terminé de recolectar la cantidad de leche minima de esta calidad
          // lo que sobra se podrá mezclar en planta para la siguiente calidad
          // Luego continua con la siguiente calidad
          // Esto no aplica para la menor calidad de leche
          if (minPrize[i] <= actualPrize[i])
          {
            if (i != (nQualities - 1))
            {
              excess = actualPrize[i] - minPrize[i];
              actualPrize[i] = minPrize[i];
              actualPrize[i+1] += excess;
            }
            T++;
            break;
          }
        }
      }

      // Volver a la planta

      for (i = 1; i < nTrucks; i++)
      {
        actualRoutes[i].push_back(origin);
      }

      // Beneficio por valor de leche
      int finalPrize[4];
      float actualQuality, bestIncome, bestCost, newIncome, newCost;

      getRealPrize(actualRealPrize, actualRoutes, nTrucks, nQualities, oProd, farmQuality);
      bestBlend(finalPrize, actualRealPrize, minPrize);
      getCapacity(capacity, oCap, oProd, actualRoutes, nTrucks);
      actualQuality = eval(actualRealPrize, minPrize, profit, cost, nTrucks, actualRoutes, bestIncome, bestCost);

      // Distancia de transporte y calidad de carga
      int distance [nTrucks];
      measureDist(distance, nTrucks, actualRoutes, cost);
      getLoadQuality(loadQuality, farmQuality, actualRoutes, nTrucks);

      // Escribir solución
          
      outFile << "** GRASP **  Q: " << actualQuality << ", Income: " << bestIncome << ", Cost: " << bestCost << "  Prize (1-2-3): " << finalPrize[1] << " " << finalPrize[2] << " " << finalPrize[3] << endl;
      outFile << "Routes / Distance / Load" << endl;
      for (i = 1; i < nTrucks; i++)
      {
        if (actualRoutes[i].size() != 0)
        { 
          outFile << "Truck " << i << " Q" << loadQuality[i] << " ";
          for (j = 0; j < int(actualRoutes[i].size()); j++)
          {
            if (actualRoutes[i][j] == origin)
            {
              if (j == 0)
              {
                outFile << origin << "-";
              }
              else
              {
                outFile << origin << " " << distance[i] << " " << oCap[i] - capacity[i] << endl;
              } 
            }
            else
            {
              outFile << actualRoutes[i][j] << "-";
            } 
          }
        }
      }
      // Record end time
      auto finish = std::chrono::high_resolution_clock::now();
      std::chrono::duration<double> elapsed = finish - start;
      outFile << "Elapsed time: " << elapsed.count() << " s\n";
      
      float bestQuality = actualQuality, newQuality;

      // realPrize
      int bestRealPrize [nQualities];
      int newRealPrize [nQualities];
      for (i = 0; i < nQualities; i++)
      {
        bestRealPrize[i] = actualRealPrize[i];
        newRealPrize[i] = actualRealPrize[i];
      }

      // Rutas
      
      vector <int> bestRoutes[nTrucks];
      vector <int> newRoutes[nTrucks];
      for (i = 1; i < nTrucks; i++)
      {
        bestRoutes[i] = actualRoutes[i];
        newRoutes[i] = bestRoutes[i];
      }

      // ******************************************************************************************************************
      // ******************************************************************************************************************
      // Simulated annealing
      // ******************************************************************************************************************
      // ******************************************************************************************************************


      start = std::chrono::high_resolution_clock::now();


      // Iterador SA
      ofstream data;
      data.open("outputs/data.csv");
      data << "it,temp,actualQ,bestQ,pAvg,diversification\n";
      
      int itRes, it, r, rFarm, rFarmExternal, rTruck, nAvailableF, availableF[nFarms], minDist, dist, minDistPos;
      int totalIt = 0, acceptedDowngrades = 0, nUpdates = 0;
      float p, operatorP;
      float Temp, addPm, capRatio;
      bool updt;
      vector <float> bestSolutions;
      for (itRes = 0; itRes < nResets; itRes++)
      {
        Temp = T0;
        for (it = 0; it < nIterations; it++)
        {
          updt = false;
          //visualizar calidad de solucion actual
          /*
          if ((it % (nIterations/10) == 0))
          {
            cout << "Reset: " << itRes << "." << (it*10)/nIterations << ", Temp: " << Temp << ", actQ: " << actualQuality << endl;
          }*/
          //cout << "antes" << endl;
          operatorP = float_rand(0,1);
          rTruck = int_rand(1, nTrucks);
          // Control de parametro add, afectado por m y por capacidad restante de truck
          capRatio = 1 - float(capacity[rTruck]) / float(oCap[rTruck]);
          addPm = addP - (addP * alphaCtrl * capRatio);
          //cout << "addPm: " <<addPm << endl;
          // Operador Add
          if (addPm > operatorP)
          {
            // Añadir nodo factible
            // Generar una lista de granjas factibles para la capacidad de rTruck
            nAvailableF = 0;
            for (i = 1; i < nFarms; i++)
            {
              if ((production[i] > 1) && (oProd[i] <= capacity[rTruck]))
              {
                availableF[nAvailableF] = i;
                nAvailableF +=1;
              }
            }
            // cout << " add for 1 \n";
            // Si el tamaño de la lista es mayor a 0 escojer una granja al azar
            if (nAvailableF > 0)
            {
              r = int_rand(0, nAvailableF);
              rFarm = availableF[r];
              // Actualizar newRoutes, añadiendo rFarm a la posicion con menor costo
              if (int(actualRoutes[rTruck].size() == 2))
              {
                newRoutes[rTruck].insert(newRoutes[rTruck].begin() + 1, rFarm);
              }
              else
              {
                minDist = cost[ 0 ][ rFarm ] + cost[ rFarm ][ actualRoutes[rTruck][1] ];
                minDistPos = 1;
                for (i = 1; i < int(actualRoutes[rTruck].size() - 1); i++)
                {
                  if (i == int(actualRoutes[rTruck].size() - 2))
                  {
                    dist = cost[ actualRoutes[rTruck][i] ][ rFarm ] + cost[ rFarm ][ 0 ];
                  }
                  else
                  {
                    dist = cost[ actualRoutes[rTruck][i] ][ rFarm ] + cost[ rFarm ][ actualRoutes[rTruck][i+1] ];
                  }
                  if (dist < minDist)
                  {
                    minDist = dist;
                    minDistPos = i+1;
                  }
                }
                newRoutes[rTruck].insert(newRoutes[rTruck].begin() + minDistPos, rFarm);
              }
              // Nueva cantidad de recolección por calidad
              getRealPrize(newRealPrize, newRoutes, nTrucks, nQualities, oProd, farmQuality);
              if (feasible(newRealPrize, minPrize, newRoutes))
              {
                updt = true;
              }
              else
              {
                // Restaurar actualRealPrize
                for (i = 1; i < nQualities; i++)
                {
                  newRealPrize[i] = actualRealPrize[i];
                }
                // Restaurar newRoutes
                newRoutes[rTruck] = actualRoutes[rTruck];
              }
            }
          }
          
          else if ((swapP + addP) > operatorP){
            // Operador Swap
            // usar funcion, darle rutas, camion y nodo
            // Se puede hacer swap solo si existe un nodo presente
            if (int(actualRoutes[rTruck].size()) > 2)
            {
              rFarm = int_rand(1, actualRoutes[rTruck].size() - 1);
              rFarmExternal = getTopRandomExternalFarm(actualRoutes, cost, production, oProd, farmQuality, rTruck, rFarm, nTrucks, nFarms, nQualities, randLength, profit, capacity, iQualityFarms, origin);

              if (rFarmExternal != 0)
              {
                newRoutes[rTruck][rFarm] = rFarmExternal;

                // Nueva cantidad de recolección por calidad
                getRealPrize(newRealPrize, newRoutes, nTrucks, nQualities, oProd, farmQuality);
                if (feasible(newRealPrize, minPrize, newRoutes))
                {
                  updt = true;
                }
                else
                {
                  // Restaurar actualRealPrize
                  for (i = 1; i < nQualities; i++)
                  {
                    newRealPrize[i] = actualRealPrize[i];
                  }
                  // Restaurar newRoutes
                  newRoutes[rTruck] = actualRoutes[rTruck];
                }
              }
            }
          }

          else
          {
            // Operador Drop

            //cout << "remove\n";
            // Quitar nodo
            // La ruta siempre tiene el nodo de origen y destino
            if (int(actualRoutes[rTruck].size()) > 2)
            {
              r = int_rand(1, actualRoutes[rTruck].size() - 1);
              rFarm = actualRoutes[rTruck][r];
              newRoutes[rTruck].erase(newRoutes[rTruck].begin() + r);
              getRealPrize(newRealPrize, newRoutes, nTrucks, nQualities, oProd, farmQuality);
              if (feasible(newRealPrize, minPrize, newRoutes))
              {
                updt = true;
              }
              else
              {
                // Restaurar actualRealPrize
                for (i = 1; i < nQualities; i++)
                {
                  newRealPrize[i] = actualRealPrize[i];
                }
                // Restaurar newRoutes
                newRoutes[rTruck] = actualRoutes[rTruck];
              }
            }
            //cout << "despues remove\n";
          }

          // Factibilidad del cambio
          if (updt)
          {
            nUpdates++;
            newQuality = eval(newRealPrize, minPrize, profit, cost, nTrucks, newRoutes, newIncome, newCost);
            p = exp((newQuality - actualQuality)/Temp);
            /*
            if (it % 50 == 0)
            {
              cout << "itRes: " << itRes << ", it: " << it << ", diff: " << newQuality - actualQuality << ", Temp: " << Temp << ", p: " << p << endl;;
            }
            */
            if (p > float_rand(0,1))
            {
              if (newQuality < actualQuality)
              {
                acceptedDowngrades++;
              }
              // Actualizar actualQuality, actualRoutes, actualRealPrize
              actualQuality = newQuality;

              for (i = 1; i < nTrucks; i++){
                actualRoutes[i] = newRoutes[i];
              }

              for (i = 0; i < nQualities; i++)
              {
                actualRealPrize[i] = newRealPrize[i];
              }

              // actualizar production, capacity
              getCapacity(capacity, oCap, oProd, actualRoutes, nTrucks);
              getProduction(production, oProd, actualRoutes, nTrucks, nFarms);

              // Checkear actualquality con bestQuality
              if (bestQuality < actualQuality)
              {
                //cout << "Instancia: " << in << ", itRes: " << itRes << " bQuality Upd: " << bestQuality << " -> "<< actualQuality << endl;
                //cout << "RP 1: " << bestRealPrize[1] << " -> "<< actualRealPrize[1] << ", RP 2: " << bestRealPrize[2] << " -> "<< actualRealPrize[2] << ", RP 3: " << bestRealPrize[3] << " -> "<< actualRealPrize[3] << endl;
                
                bestQuality = actualQuality;
                bestCost = newCost;
                bestIncome = newIncome;

                bestRoutes[rTruck] = actualRoutes[rTruck];
                for (i = 1; i < nTrucks; i++){
                  bestRoutes[i] = actualRoutes[i];
                }
                
                for (i = 1; i < nQualities; i++)
                {
                  bestRealPrize[i] = actualRealPrize[i];
                }
              }
            }
            else
            {
              // Restaurar actualRealPrize
              for (i = 1; i < nQualities; i++)
              {
                newRealPrize[i] = actualRealPrize[i];
              }
              // Restaurar newRoutes
              newRoutes[rTruck] = actualRoutes[rTruck];
            } 
            //cout << "despues update\n";
          }
          totalIt++;
          if (totalIt % 1000 == 0)
          {
            p = std::ceil(p * 100.0) / 100.0;
            data << totalIt << "," << Temp << "," << actualQuality << "," << bestQuality << "," << float(acceptedDowngrades)/float(nUpdates)*100 << "\n";
            nUpdates = 0;
            acceptedDowngrades = 0;
          }
          Temp *= alpha;
        }
        // Mejores soluciones cada 100 resets
        /*if (itRes % w == 0)
        {
          bestSolutions.push_back(bestQuality);
          cout << "Reset: " << itRes << ", bestQuality: " << bestQuality << ", Income: " << bestIncome << ", Cost: " << bestCost <<  endl;
        }*/
      }
      data.close();
      
      // Guardar mejor solución  actual en archivo

      //Calcular loadQuality final
      getCapacity(capacity, oCap, oProd, bestRoutes, nTrucks);
      getLoadQuality(loadQuality, farmQuality, bestRoutes, nTrucks);

      //Calcular distancia final
      measureDist(distance, nTrucks, bestRoutes, cost);

      // Calcular la mejor mezcla en planta final
      getRealPrize(bestRealPrize, bestRoutes, nTrucks, nQualities, oProd, farmQuality);
      bestBlend(finalPrize, bestRealPrize, minPrize);

      // Escribir mejor solución
      outFile << "\n** SA ** nReset: " << nResets << "  nIt: " << nIterations << endl;
      outFile << "Quality: " << bestQuality << ", Income: " << bestIncome << ", Cost: " << bestCost <<  "  Prize (1-2-3): " << finalPrize[1] << " " << finalPrize[2] << " " << finalPrize[3] << endl;
      outFile << "Routes / Distance / Load" << endl;
      for (i = 1; i < nTrucks; i++)
      {
        if (bestRoutes[i].size() != 0)
        { 
          outFile << "Truck " << i << " Q" << loadQuality[i] << " ";
          for (j = 0; j < int(bestRoutes[i].size()); j++)
          {
            if (bestRoutes[i][j] == origin)
            {
              if (j == 0)
              {
                outFile << origin << "-";
              }
              else
              {
                outFile << origin << " " << distance[i] << " " << oCap[i] - capacity[i] << endl;
              } 
            }
            else
            {
              outFile << bestRoutes[i][j] << "-";
            } 
          }
        }
      }

      // Mejores Soluciones
      /*outFile << "Mejores soluciones cada " << w << " resets:\n";*/
      for (i = 0; i < int(bestSolutions.size()); i++)
      {
        if (i == 0)
        {
          outFile << bestSolutions[i];
        }
        else
        {
          outFile << " - " << bestSolutions[i];
        }
      }
      outFile << endl;
      

      finish = std::chrono::high_resolution_clock::now();
      elapsed = finish - start;
      outFile << "Elapsed time: " << elapsed.count() << " s\n";
    }
    outFile.close();
  }
 return 0;
}
