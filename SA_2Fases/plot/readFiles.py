import os.path

# Returns a dictionary with production by farm in instance {farm: production}
# Returns a list with farms by quality
# Returns a matrix with travel cost between nodes
def readInstance(real, i):
    if real:
        file = open(os.path.dirname(__file__) + '/../MCPSB/Real_Instances/instancia{}.mcsb'.format(i), 'r')
    else:
        file = open(os.path.dirname(__file__) + '/../MCPSB/Test_Instances/instancia{}.mcsb'.format(i), 'r')

    found = False
    while not found:
        line = file.readline()
        if line.strip() == 'param qu:=':
            found = True

    found = False
    prodInstancia = dict()
    farmByQuality = [list(), list(), list(), list()]
    while not found:
        line = file.readline()
        if line.strip() == ';':
            found = True
            break
        splitted = line.split()
        splitted = [int(i) for i in splitted]
        if len(splitted) == 3:
            prodInstancia[splitted[0]] = (splitted[1], splitted[2])
            if splitted[2] == 1:
                farmByQuality[1].append(splitted[0])
            elif splitted[2] == 2:
                farmByQuality[2].append(splitted[0])
            else:
                farmByQuality[3].append(splitted[0])

    found = False
    while not found:
        line = file.readline()
        if line.strip() == 'param c[*,*]':
            found = True

    found = False
    line = file.readline()
    cost = list()
    c = 0
    while not found:
        line = file.readline()
        if line.strip() == ';':
            found = True
            break
        splitted = line.split()
        splitted.pop(0)
        splitted[c] = 0
        splitted = [int(i) for i in splitted]
        cost.append(splitted)
        c += 1
        

    file.close()
    return prodInstancia, farmByQuality, cost

#Returns a dictionary {production: farm} with productions of farms
def readProductions():
    file = open(os.path.dirname(__file__) + '/producciones.dat', 'r')
    
    productions = dict()
    line = file.readline()
    while True:
        line = file.readline()
        splitted = line.split()
        splitted = [int(i) for i in splitted]
        if len(splitted) == 0:
            break
        else:
            productions[splitted[1]] = splitted[0]
    return productions

# Returns a dictionary {farm: coordinates} with real coordinates of farms
def readCoordinates():
    file = open(os.path.dirname(__file__) + '/coordenadas.dat', 'r')
    
    coordinates = dict()
    line = file.readline()
    line = file.readline()
    splitted = line.split()
    coordOrigin = (float(splitted[1]), float(splitted[2]))
    while True:
        line = file.readline()
        splitted = line.split()
        
        if len(splitted) == 0:
            break
        
        coordinates[int(splitted[0])] = (float(splitted[1]), float(splitted[2]))
    
    return coordOrigin, coordinates

# Returns a list of Routes
def readRoutes(i, origin = 1000):
    file = open(os.path.dirname(__file__) + '/Instance{}/routes{}.dat'.format(i, i), 'r')

    routes = list()

    while True:
        line = file.readline()
        line = line.replace('\n', '')
        splitted = line.split("-")
        if len(splitted) <= 1:
            break
        splitted = [0 if int(i) == origin else int(i) for i in splitted]
        routes.append(splitted)
    
    return routes
