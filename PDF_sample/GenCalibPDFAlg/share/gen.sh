#!/bin/bash
mytop=MYTOP

job_totN=293
sname=( "Ge68" "Laser0.05")
# sname=( "Laser0.05")
for s in ${sname[@]}
do
    mkdir -p $s
    cd $s
    path1=`pwd`
    mkdir -p root log run PEMeanMap Tdyn Tmcp
    jnb=run/${s}_pdf_
    for((n=0;n<$job_totN;n++))
    do
        job_name=${jnb}${n}.sh
        echo "#!/bin/bash" > $job_name
        echo "cd ${path1}" >> $job_name
        echo "source $mytop/bashrc" >> $job_name
        echo "(time python ../tut_calib2rec.py --JOBA ${n} --JOBB $[$n+1] --PMTtype 1 --SignalWindow 420 --RfrIndxLS 1.54 --RfrIndxWR 1.355 --calibdir ACUCLSPATH --usercalibdir ACUCLSPATH --paradir ${mytop}/data/Calibration/PMTCalibSvc/data --calibsource $s --startseed 0 --filenum FILENUM) >& log/pdflog-${n}.txt" >> $job_name
        echo "mv PE-theta-distribution-${n}.root root/" >> $job_name
        echo "mv PEMeanMap_${n}.pdf PEMeanMap/" >> $job_name
        echo "mv Tdyn_pos${n}.txt Tdyn" >> $job_name
        echo "mv Tmcp_pos${n}.txt Tmcp" >> $job_name
    done
    chmod +x ${jnb}*.sh
    hep_sub ${jnb}"%{ProcId}".sh -n $job_totN -e /dev/null -o /dev/null
    cd ..
done
