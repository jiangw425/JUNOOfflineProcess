#!/bin/bash
path0=`pwd`
steps=(0 1 2 3 4)
localenv=LOCALENV
if [[ $# -ne 1 ]];then
    echo "Input choose from: ${steps[@]}"
    exit 1
fi
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

input1=$1; shift
echo "`date` on `hostname`: `whoami` runs step $input1." >> recorder_pdf.txt
sname=(Ge68 Laser0.05)
if [[ $input1 == 0 ]];then
    source $localenv
    bash build.sh
elif [[ $input1 == 1 ]];then
    cd GenCalibPDFAlg/share
    bash gen.sh
    cd $path0
elif [[ $input1 == 2 ]];then
    cd GenCalibPDFAlg/share
    source /hpcfs/juno/junogpu/jiangw/miniconda3/bin/activate
    conda activate Cern
    for s in ${sname[@]}
    do
        mkdir -p $s/GridMu_RealAdd
        cd $s/GridMu_RealAdd
        sed "s#SOURCE#$s#g" $path0/SampleFiles/genpemap.py > genpemap.py
        python genpemap.py
        cd - > /dev/null
    done
    conda deactivate
    conda deactivate
    cd $path0
    echo "Now you need to copy files to local PC and process using matlab."
elif [[ $input1 == 3 ]];then
    cd GenCalibPDFAlg/share
    for s in ${sname[@]}
    do
        cd $s/GridMu_RealAdd
        checkExist gridr.txt gridtheta.txt lgridmu_${s}_R1.txt lgridmu_${s}_R3.txt sgridmu_${s}_R1.txt sgridmu_${s}_R3.txt
        if [[ $s == "Ge68" ]];then
            EkOpt=""
        elif [[ $s == "Laser0.05" ]];then
            EkOpt="_Ek"
        fi
        sed "s#SOURCE#$s#g" $path0/SampleFiles/Collect_R1.C > Collect_R1.C
        sed -e "s#SOURCE#$s#g" -e "s#JW#$EkOpt#g" $path0/SampleFiles/Collect_comb.C > Collect_comb.C
        root -l -q -b Collect_R1.C
        root -l -q -b Collect_comb.C
        cd ../..
    done

    mkdir -p timePDF
    cd timePDF
    root -b -q -l $path0/SampleFiles/pComb.C
    root -b -q -l $path0/SampleFiles/DNnPETimePdf_GL.C
    cd $path0

    cd GenQPDFAlg/share
    bash gen-rec.sh
    cd $path0

elif [[ $input1 == 4 ]];then
    mkdir -p GenQPDFAlg/NPEQ
    cd GenQPDFAlg/NPEQ
    source $localenv
    root -l -b -q $path0/SampleFiles/AvgNPEQpdf.C
    cd $path0

    mkdir -p recMap
    cd recMap
    mkdir -p CalibPMTPara ChargeSpec TimePdf nPEMap
    cp ../GenCalibPDFAlg/share/Ge68/GridMu_RealAdd/LnPEMapFile.root nPEMap
    cp ../GenCalibPDFAlg/share/Laser0.05/GridMu_RealAdd/LnPEMapFile_Ek.root nPEMap
    cp ../GenCalibPDFAlg/share/timePDF/TimePdfFile.root TimePdf
    cp ../GenQPDFAlg/NPEQ/AvgNPEQpdf.root ChargeSpec
    ### CalibPMTPara not used in OMILREC or other alg
    cd $path0
fi