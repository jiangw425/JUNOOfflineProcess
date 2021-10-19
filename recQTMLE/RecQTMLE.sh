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
echo "`date` on `hostname`: `whoami` runs step $input1." >> recorder_recQTMLE.txt
sname=(AmC Co60 Cs137 Ge68)
if [[ $input1 == 1 ]];then
    mkdir -p AssembFiles
    cd AssembFiles
    for s in ${sname[@]}
    do
        mkdir -p $s
        cd $s
        mkdir -p run log cfile
        cp ${path0}/SampleFiles/RecAnalysis_User_${s}.C .
        bash ${path0}/gen.sh $s
        cd ..
    done
    cd $path0
elif [[ $input1 == 2 ]];then
    echo ""
elif [[ $input1 == 3 ]];then
    echo ""
elif [[ $input1 == 4 ]];then
    echo ""
fi