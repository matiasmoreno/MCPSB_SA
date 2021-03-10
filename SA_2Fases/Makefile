flags=-Wno-deprecated -g -ggdb
comp=g++
prog=MCPSB_SA

#PARAMS = Seed nResets nIterations T0 addP swapI kImprovements intensificationRatio path
PARAMS = 1 500 500000 30 0.5 0.5 0.01 0.2 MCPSB/Real_Instances/instancia1.0.mcsb

debug=0

exe:$(prog)
	./$(prog) $(PARAMS)

$(prog): MCPSB_SA.cpp
	$(comp) $(prog).cpp -o $(prog)

clean:
	rm -f *~
	rm -f *.o
	rm -f $(prog)
	rm -f core*