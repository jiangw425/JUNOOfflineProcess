#!/bin/bash
if [[ $#<=1 ]];then
    echo "Wrong input."
    exit 1
fi
# $1: target name, $2: hadd source file path + specified name
specified_name=`basename $2`

tmp_userfile_list=`ls ${2}*.root`
filenum=`echo "$tmp_userfile_list" | wc -l`
jw_N=100
if [[ $filenum -gt $jw_N ]];then
    for((i=0;i<=$(($filenum/$jw_N));i++))
    do
        # hadd ${i}.root `sed -n '$((i*100)),$((i*100+100))p' tmp_userfile.list`
        hadd ${specified_name}_tmp_${i}.root `echo "$tmp_userfile_list" | tail -n +$((i*jw_N)) | head -n $jw_N`
        if [[ -f "${specified_name}_tmp_${i}.root" ]];then
            rm -rf `echo "$tmp_userfile_list" | tail -n +$((i*jw_N)) | head -n $jw_N`
        fi
    done
    hadd -f $1 ${specified_name}_tmp_*.root
elif [[ $filenum -gt 0 ]];then
    hadd -f $1 ${2}*.root
else
    echo "No files to hadd."
    exit 1
fi

if [[ -f "$1" ]];then
    rm -rf ${2}*.root
    rm -rf ${specified_name}_tmp_*.root
fi