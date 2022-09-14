#!/bin/bash
path0=`pwd`
s=eplus
# energies=(0 1 2 3 4 5 6 7 8 9 10)
energies=(0 0.5 1 2 3 4 5 8)
# energies=(3 4)
# energies=(1)

jnb="recAnalysis_${s}_"
mkdir -p run
for((n=0;n<${#energies[@]};n++))
do
    mkdir -p ${energies[$n]}MeV
    sed "s#ENERGY#${energies[$n]}#g" RecAnalysis_User_${s}.C > ${energies[$n]}MeV/RecAnalysis_User_${s}.C
    jobname="run/${jnb}${n}.sh"
    echo "#!/bin/bash" > $jobname
    echo "cd ${path0}/${energies[$n]}MeV" >> $jobname
    echo "(time root -b -q -l RecAnalysis_User_${s}.C) >& log.txt" >> $jobname
    # cd ${energies[$n]}MeV
    # root -b -q -l RecAnalysis_User_${s}.C
    # cd ..
done
chmod +x run/${jnb}*
hep_sub run/${jnb}"%{ProcId}".sh -n ${#energies[@]} -e /dev/null -o /dev/null
