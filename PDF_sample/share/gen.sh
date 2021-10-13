#!/bin/bash
name_sources=( "Ge68" "Laser0.05")
# name_sources=( "Laser0.05")
for s in ${name_sources[@]}
do
    mkdir -p $s
    cd $s 
    sed -e "s/NAMESOURCE/$s/g" ../sample-detsim-e+_parent.sh >gen-detsim-e+.sh
    # if [[ $1 = sub ]];then
    . gen-detsim-e+.sh $s
    # fi
    cd ..

    if [[ $s == "Ge68" ]];then
        jw=""
    elif [[ $s == "Laser0.05" ]];then
        jw="_Ek"
    fi

    mkdir -p $s/GridMu_RealAdd
	sed "s#SOURCE#$s#g" SampleFiles/genpemap.py    > $s/GridMu_RealAdd/genpemap.py
    sed "s#SOURCE#$s#g" SampleFiles/Collect_R1.C   > $s/GridMu_RealAdd/Collect_R1.C
    sed -e "s#SOURCE#$s#g" -e "s#JW#$jw#g" SampleFiles/Collect_comb.C > $s/GridMu_RealAdd/Collect_comb.C
done

