#!/bin/bash
basepath="BASEPATH"
if [[ ${basepath:0:7} == "root://" ]];then
    echo "EOS dir need generate manually"
    exit 0
fi
sname=(Co60 Cs137 AmC Ge68 Laser0.1 Laser0.05 C14)

calibS=(Co60 Cs137 AmC Ge68 Laser0.1 Laser0.05)
for s in ${sname[@]}
do
    if [[ -f "Elecsim_path/${s}_elec.list" ]];then
        echo "${s}_elec.list exist, skip..."
        continue
    fi
    echo "Finding $s elecsim path..."
    # basedir=`find $basepath -maxdepth 2 -name $s`
    if [[ ${calibS[@]} =~ $s ]];then
        basedir=`find $basepath -maxdepth 2 -name $s`
        dir=$basedir/${s}_0_0_0/elecsim/root
    else
        basedir=`find $basepath -maxdepth 3 -name $s`
        dir=$basedir/elecsim/root
    fi

    if [[ -d $dir ]] && [[ `ls $dir | grep elecsim | wc -l` -gt 0 ]];then
        ls $dir/elecsim* > Elecsim_path/${s}_elec.list
    else
        echo "xxxxxxxxxx $s elecsim root file path not available, please check! xxxxxxxxxxxxx"
    fi
done
echo "Elecsim path generated!"
