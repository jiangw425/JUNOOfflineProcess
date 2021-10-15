#!/bin/bash
# $1 = 293 / 43
if [[ $# -lt 1 ]];then
    echo "\$1: calib pos num(293/43), \$2(optional): sub / rm"
    exit 1
fi
input1=$1; shift
input2=$1; shift

path0=`pwd`
jnb="rec_analysis_"
if [[ $input2 == "sub" ]];then
    hep_sub ${jnb}"%{ProcId}".sh -n $1
    exit 0
elif [[ $input2 == "rm" ]];then
    rm -rf ${jnb}*
    for((n=0;n<$1;n++));do rm -rf $n;done
    exit 0
fi
for((n=0;n<$1;n++))
do
    echo $n
    mkdir -p $n
    sed "s#START#$n#g" RecAnalysis_User.C > $n/RecAnalysis_User.C
    jobname="${jnb}${n}.sh"
    echo "#!/bin/bash" > $jobname
    echo "cd ${path0}" >> $jobname
    echo "root -b -q -l ${n}/RecAnalysis_User.C" >> $jobname
done
chmod +x ${jnb}*
