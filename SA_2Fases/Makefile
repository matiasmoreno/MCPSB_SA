flags=-Wno-deprecated -g -ggdb
comp=g++
prog=MCPSB_SA

#PARAMS = Seed nResets nIterations T0 alpha addP swapI kImprovements intensificationRatio scheduling path
PARAMS = 1 50 100000 35 0.99999 0.5 0.5 0.01 0.5 1 MCPSB/Real_Instances/instancia1.mcsb
#PARAMS = 1 200 500000 35 0.99999 0.5 0.0 0.5 0.5 0.01 1 MCPSB/Test_Instances/instancia1.mcsb

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