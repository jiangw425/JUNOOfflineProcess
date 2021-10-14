#!/bin/bash
path0=`pwd`
mytop=MYTOP

mkdir -p NPEPDF run log

job_totN=881
jnb=run/qpdf_
for((n=0;n<$job_totN;n++))
do
    job_name=${jnb}${n}.sh
    echo "#!/bin/bash" > $job_name
    echo "source $mytop/bashrc" >> $job_name
    joba=$((20*$n))
    echo "(time python Genqpdf.py --JOBA ${joba} --JOBB $[$joba+20] --enableUserSPEs --runmode 2 --peRange 100 --userSPEFile ${mytop}/data/Calibration/PMTCalibSvc/data/output_deconv.root --outdir ./ ) >&  log/qpdflog-${n}.txt" >> $job_name
done
chmod +x ${jnb}*.sh
hep_sub ${jnb}"%{ProcId}".sh -n $job_totN -e /dev/null -o /dev/null