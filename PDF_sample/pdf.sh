#!/bin/bash
path0=`pwd`
steps=(0 1 2 3 4)
localenv=LOCALENV
if [[ ! ${steps[@]} =~ $1 ]];then
    echo "Wrong step number."
    echo "Please choose from: ${steps[@]}"
    exit 1
fi

checkExist(){
    for file in $*
    do
        if [[ ! -f "${file}" ]];then
            echo "$file not exist."
            exit 1
        fi
    done
}

sname=(Ge68 Laser0.05)
if [[ $1 == 0 ]];then
    cd cmt
    source $localenv
    make
    cd $path0
elif [[ $1 == 1 ]];then
    cd share
    for s in ${sname[@]}
    do
        mkdir -p $s
        cd $s 
        sed -e "s/NAMESOURCE/$s/g" ../sample-detsim-e+_parent.sh >gen-detsim-e+.sh
        . gen-detsim-e+.sh $s
        cd ..
    done
    cd $path0
elif [[ $1 == 2 ]];then
    cd share
    source /hpcfs/juno/junogpu/jiangw/miniconda3/bin/activate
    conda activate Cern
    for s in ${sname[@]}
    do
        mkdir -p $s/GridMu_RealAdd
        cd $s/GridMu_RealAdd
        sed "s#SOURCE#$s#g" $path0/SampleFiles/genpemap.py    > $s/GridMu_RealAdd/genpemap.py
        python genpemap.py
    done
    conda deactivate
    conda deactivate
    cd $path0
    echo "Now you need to copy files to local PC and process using matlab."
elif [[ $1 == 3 ]];then
    cd share
    for s in ${sname[@]}
    do
        cd $s/GridMu_RealAdd
        checkExist gridr.txt gridtheta.txt lgridmu_${s}_R1.txt lgridmu_${s}_R3.txt sgridmu_${s}_R1.txt sgridmu_${s}_R3.txt
        if [[ $s == "Ge68" ]];then
            EkOpt=""
        elif [[ $s == "Laser0.05" ]];then
            EkOpt="_Ek"
        fi
        sed "s#SOURCE#$s#g" $path0/SampleFiles/Collect_R1.C   > $s/GridMu_RealAdd/Collect_R1.C
        sed -e "s#SOURCE#$s#g" -e "s#JW#$EkOpt#g" $path0/SampleFiles/Collect_comb.C > $s/GridMu_RealAdd/Collect_comb.C
        root -l -q -b Collect_R1.C
        root -l -q -b Collect_comb.C
        cd ../..
    done

    mkdir -p timePDF
    cd timePDF
    root -b -q -l $path0/SampleFiles/pComb.C
    root -b -q -l $path0/SampleFiles/DNnPETimePdf_GL.C
    cd $path0

    cd GenQPDF/cmt
    source $localenv
    make
    cd ../share



fi

echo "step$1 done."