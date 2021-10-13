#!/bin/bash
# todo: e+ spaneu
# sim_type=detsim
# sim_type=elecsim
# sim_type=calib
sim_type=recQTMLE

# sname=(C14)
# sname=(Co60 Cs137 Ge68 AmC Laser0.1 Laser0.05)
sname=(Co60 Cs137 Ge68 AmC Laser0.1 Laser0.05)
# sname=(e+ SpaNeu)


radioS=(Co60 Cs137 Ge68 AmC)
calibS=(Co60 Cs137 Ge68 AmC Laser0.1 Laser0.05)
evtPerJob=1000
jobnum=20
seed_start=0

laserN=0
e_energies=0
eventRate=0

for s in ${sname[@]}
do
    echo $s
    if [[ $s == "Co60" ]] || [[ $s == "Cs137" ]];then
        path1="ACU"
    elif [[ $s == "Ge68" ]] || [[ $s == "AmC" ]] || [[ ${s:0:5} == "Laser" ]];then
        path1="ACU-CLS"
    else
        path1="."
    fi

    if [[ $s == "e+" ]];then
        e_energies=0_1_2_3_4_5_6_7_8_9_10_0~10
    fi

    if [[ $sim_type == "detsim" ]];then
        if [[ $s == "Laser0.05" ]];then
            laserN=11522
        elif [[ $s == "Laser0.1" ]];then
            laserN=22000
        fi
    elif [[ $sim_type == "elecsim" ]];then
        evtPerJob=-1
        if [[ ${calibS[@]} =~ $s ]];then
            eventRate=100
        elif [[ $s == "SpaNeu" ]];then
            eventRate=6
        elif [[ $s == "C14" ]];then
            eventRate=40000
            evtPerJob=10000
        fi
    elif [[ $sim_type == "calib" ]];then
        evtPerJob=-1
    elif [[ $sim_type == "rec_QTMLE" ]];then
        evtPerJob=-1
    fi

    pos_xyz=()
    if [[ $s == "Laser0.1" ]];then
        pos_xyz[0]="0_0_0"
    elif [[ ${radioS[@]} =~ $s ]] || [[ $s == "Laser0.05" ]];then
        posnum_counter=0
        while read lines
        do
            pos_xyz[${posnum_counter}]=`echo $lines | tr ' ' _`
            let ++posnum_counter
        done < pos/${path1}_XYZPos.txt
    else
        pos_xyz[0]="Uniform"
    fi

    echo ${#pos_xyz[@]}
    for x_y_z in ${pos_xyz[@]}
    do
        echo "$s $x_y_z"
        bash jw_gen_run.sh $path1 $s $x_y_z $sim_type $evtPerJob $jobnum $seed_start $laserN $e_energies $eventRate
        seed_start=$(($seed_start+$jobnum))
    done

    seed_start=$(($seed_start+10000))
done