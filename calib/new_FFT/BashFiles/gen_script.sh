#!/bin/bash
if [[ $#<=1 ]]; then
    echo "You have not input source name, step number, 0(deonv)/1(coti) and forceTrigger?(0/1)."
    exit 1
fi

localenv=YOURENV
path0=`pwd`
rm -rf $1/step$2/run/*
rm -rf $1/step$2/root/*
rm -rf $1/step$2/user-root/*
rm -rf $1/step$2/log/*

if [[ $3 -eq 1 ]];then
	cotiopt="--waverec-method cotiwaverec"
fi
if [[ $4 -eq 1 ]];then
  if [[ ${2:0:1} -eq 0 ]];then
    forceTriggerOpt="--enableForceTriggerInput"
  else
    forceTriggerOpt="--dnstart DNSTART --dnend DNEND --enableForceTriggerInput"
  fi
fi
listname="Elecsim_path/$1_elec.list"
m_length=9
if [[ $2 == "2" ]] || [[ $2 -eq 12 ]];then
  listname="$1/step${2}/${1}_calib.list"
  m_length=7
fi
if [[ ! -f "$listname" ]];then
  echo "$listname doesn't exist!"
  exit 10
fi
count=0
for filename in `cat ${listname}`;
do
  echo $filename
  # dir_num=`echo ${filename} | wc -L`
  # end_num=`echo ${filename} | grep -b -o "\.root" | cut -d: -f1`
  # start_num=`echo ${filename} | rev | grep -b -o "-" | cut -d: -f1`
  # start_num=$(($dir_num-`echo $start_num | awk '{print $1}'`))
  # num_length=$(($end_num - $start_num)) 
  # end_path=$(($start_num-$m_length))
  # run=${filename:$start_num:$num_length}
  # dir=${filename:0:$end_path}
  # echo ${dir}, ${run}
  sed -e "s#LOCALENV#$localenv#g" -e "s#MAIND#$path0#g" -e "s#COTIOPT#$cotiopt#g" -e "s/RUN/${count}/g" -e "s#INPUTFILE#${filename}#g" -e "s/PARA/$1/g" -e "s#FORCETRIGGEROPT#$forceTriggerOpt#g" BashFiles/run-sample-step${2}.sh > $1/step${2}/run/$1-SPE-step${2}-$count.sh
  let ++count
done

chmod +x $1/step$2/run/*.sh
