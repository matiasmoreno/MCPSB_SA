flags=-Wno-deprecated -g -ggdb
comp=g++
prog=MCPSB_SA

#PARAMS = Seed nResets nIterations T0 addP swapI kImprovements intensificationRatio path
PARAMS = 1 10 1000 73 0.55 0.27 0.011 0.721 MCPSB/Test_Instances/instancia5.mcsb
#PARAMS = 1 200 500000 350.5 0.0 0.5 0.5 0.01 MCPSB/Test_Instances/instancia1.mcsb

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