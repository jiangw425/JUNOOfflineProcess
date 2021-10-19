#!/bin/bash
# $1 = 293 / 43
if [[ $# -lt 1 ]];then
    echo "\$1: calib pos source(Ge68/AmC/Cs137/Co60)"
    exit 1
fi
path0=`pwd`
s=$1; shift

if [[ $s == "Ge68" ]] || [[ $s == "AmC" ]];then
    jobnum=293
elif [[ $s == "Co60" ]] || [[ $s == "Cs137" ]];then
    jobnum=43
fi
# input2=$1; shift


jnb="recAnalysis_${s}_"
# if [[ $input2 == "sub" ]];then
#     hep_sub ${jnb}"%{ProcId}".sh -n $1 -e /dev/null -o /dev/null
#     exit 0
# elif [[ $input2 == "rm" ]];then
#     rm -rf ${jnb}*
#     for((n=0;n<$1;n++));do rm -rf $n;done
#     exit 0
# fi
for((n=0;n<$jobnum;n++))
do
    sed "s#START#$n#g" RecAnalysis_User_${s}.C > cfile/RecAnalysis_User_${s}_${n}.C
    jobname="run/${jnb}${n}.sh"
    echo "#!/bin/bash" > $jobname
    echo "cd ${path0}" >> $jobname
    echo "(time root -b -q -l cfile/RecAnalysis_User_${s}_${n}.C) >& log/log-${n}.txt" >> $jobname
done
chmod +x run/${jnb}*
hep_sub run/${jnb}"%{ProcId}".sh -n ${jobnum} -e /dev/null -o /dev/null