#!/bin/bash
# used to help generate elecsim list for files in EOS
eosurl="root://junoeos01.ihep.ac.cn"
basepath="/eos/juno/valprod/valprod0/J22.1.0-rc4"
sname=(Co60 Cs137 AmC Ge68 Laser0.1 Laser0.05 C14)

# They're in specific path
ACUs=(Co60 Cs137)
CLSs=(AmC Ge68 Laser0.1 Laser0.05)
UNIs=(C14)

if [[ `basename $(pwd)` == "Elecsim_path" ]];then
    wpath="."
elif [[ `basename $(pwd)` == "BashFiles" ]];then
    wpath="../Elecsim_path"
else
    wpath="Elecsim_path"
fi

for s in ${sname[@]}
do
    listname="${wpath}/${s}_elec.list"
    if [[ -f ${listname} ]];then
        echo "${listname} exist, skip..."
        continue
    fi

    if [[ ${ACUs[@]} =~ $s ]];then
        elecsim_path="${basepath}/ACU/$s/${s}_0_0_0/elecsim/root"
    elif [[ ${CLSs[@]} =~ $s ]];then
        elecsim_path="${basepath}/ACU-CLS/$s/${s}_0_0_0/elecsim/root"
    elif [[ ${UNIs[@]} =~ $s ]];then
        elecsim_path="${basepath}/$s/${s}_Uniform/elecsim/root"
    fi

    for f in `eos ls $elecsim_path`
    do
        echo "$eosurl/$elecsim_path/$f" >> $listname
    done
    echo "Generated $listname"
done