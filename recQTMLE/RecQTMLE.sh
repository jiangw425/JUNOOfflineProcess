#!/bin/bash
path0=`pwd`
steps=(1 2 3 4)
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
echo "`date` on `hostname`: `whoami` runs step $input1." >> recorder_RecQTMLE.txt
sname=(AmC Co60 Cs137 Ge68)
if [[ $input1 == 1 ]];then
    mkdir -p AssembFiles
    cd AssembFiles
    for s in ${sname[@]}
    do
        mkdir -p $s
        cd $s
        mkdir -p run log cfile err
        cp ${path0}/SampleFiles/RecAnalysis_User_${s}.C .
        bash ${path0}/gen.sh $s
        # bash ${path0}/find_errfiles.sh $s
        cd ..
    done
    cd $path0
    mkdir -p e+
    cd e+
    cp ${path0}/SampleFiles/RecAnalysis_User_eplus.C .
    bash ${path0}/gen_eplus.sh
    cd $path0
elif [[ $input1 == 2 ]];then
    echo "Energy Resolution e+"
    cd Eresolution_e+
    root -b -l -q Resolution_Draw_1fig.C
    root -b -l -q Resolution_Draw_1fig.C
    root Resolution_Draw_1fig.C
    cd $path0
elif [[ $input1 == 3 ]];then
    echo "Radial Resolution e+"
    cd Rresolution_e+
    root -b -l -q RRes.C
    root -b -l -q RRes.C
    root RRes.C
    cd $path0
elif [[ $input1 == 4 ]];then
    echo ""
fi