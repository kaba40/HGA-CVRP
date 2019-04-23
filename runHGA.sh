namedir="Result"
mkdir -p $namedir

for numNode in 31
do
  for numVeh in 7
  do
    for numIds in 5
    do 
      for maxIt in 100 
      do
        for stuckM in 0
        do 
          for dgFact in 1.08
          do
            for prob in 0.5
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

