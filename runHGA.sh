namedir="Result"
mkdir -p $namedir

for numNode in 51 #number of nodes
do
  for numVeh in 5 #number of vehicles
  do
    for numIds in 5 #number of individual
    do 
      for maxIt in 100 #maximum number of iteration
      do
        for stuckM in 0 #maximum number of iteration with no improvement
        do 
          for dgFact in 1.08 #diversity growth factor
          do
            for prob in 0.5 #localSearch call probability
            do
		
	      echo algoSplit --name_data /home/kkeita/git/algosplit/Data/E-n${numNode}-k${numVeh}.vrp  --numInds ${numIds} --maxIt ${maxIt} --stuckMax ${stuckM} --dgFactor ${dgFact} --probLS ${prob} >${namedir}/out_result_E_n${numNode}_k${numVeh}

		./Debug/algoSplit --name_data /home/kkeita/git/algosplit/Data/E-n${numNode}-k${numVeh}.vrp --numInds ${numIds} --maxIt ${maxIt} --stuckMax ${stuckM} --dgFactor ${dgFact} --probLS ${prob} >>${namedir}/out_result_E_n${numNode}_k${numVeh}

	              done
                   done
                done
            done
        done
    done
done

