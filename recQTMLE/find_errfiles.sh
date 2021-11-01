#!/bin/bash
### get failed job id or correctly read file id
path0=`pwd`
s=${1:-AmC}

if [[ $s == "Co60" ]] || [[ $s == "Cs137" ]];then
    path1="ACU"
elif [[ $s == "Ge68" ]] || [[ $s == "AmC" ]];then
    path1="ACU-CLS"
fi
posfile=/junofs/users/jiangw/GitCode/JUNOOfflineProcess/detsim/pos/${path1}_XYZPos.txt

pos_xyz=()
posnum_counter=0
while read lines
do
    pos_xyz[${posnum_counter}]=`echo $lines | tr ' ' _`
    let ++posnum_counter
done < $posfile
posnum=${#pos_xyz[@]}
echo "posnum : ${posnum}"

checkLogfile(){
    filelist=`ls | grep .txt`
    for f in `echo $filelist`
    do
        # tail30=`tail -n 40 $f`
        # if [[ `echo "$tail30" | tail -n 7 | grep -w "Successfully"` ]] && [[ `echo "$tail30" | grep "rror\|#"` == "" ]];then
        mu=`head -n 191 $f | tail -n 1 | cut -d'	' -f2`
        if [[ `echo $mu | grep e` != "" ]];then
            echo ${f##*-} | cut -d. -f1 >> $path0/err/errorfileid_${n}.txt
        fi
        # else
            # echo ${f##*-} | cut -d. -f1 >> $path0/err/errorfileid_${n}.txt
        # fi
    done
}

for((n=0;n<${posnum};n++))
do
    rm -f err/errorfileid_${n}.txt
    x_y_z=${pos_xyz[$n]}
    echo "checking rec log: $s $x_y_z"
    cd DATAPATH/$path1/$s/${s}_${x_y_z}/recQTMLE/log
    checkLogfile
    cd $path0
done