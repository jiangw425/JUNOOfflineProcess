#!/bin/bash
path0=`pwd`
s=eplus
# energies=(0 1 2 3 4 5 6 7 8 9 10)
energies=(0 2 3 4 5 8)
# energies=(3 4)

# jnb="recAnalysis_${s}_"
for((n=0;n<${#energies[@]};n++))
do
    mkdir -p ${energies[$n]}MeV
    sed "s#ENERGY#${energies[$n]}#g" RecAnalysis_User_${s}.C > ${energies[$n]}MeV/RecAnalysis_User_${s}.C
    # jobname="run/${jnb}${n}.sh"
    # echo "#!/bin/bash" > $jobname
    # echo "cd ${path0}" >> $jobname
    # echo "(time root -b -q -l cfile/RecAnalysis_User_${s}_${n}.C) >& log/log-${n}.txt" >> $jobname
    cd ${energies[$n]}MeV
    root -b -q -l RecAnalysis_User_${s}.C
    cd ..
done
# chmod +x run/${jnb}*
# hep_sub run/${jnb}"%{ProcId}".sh -n ${jobnum} -e /dev/null -o /dev/null