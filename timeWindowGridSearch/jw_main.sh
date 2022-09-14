#!/bin/bash
sim_type=${1:-"recQTMLE"}; shift

# sname=(C14)
# sname=(Laser0.05 Ge68 Laser0.1 Co60 Cs137 AmC)
# sname=(AmC Laser0.05 Ge68 Laser0.1 Co60 Cs137)
# sname=(Ge68 e+)
# sname=(SpaNeu)
sname=(AmC e+)

sim_types=(detsim elecsim calib recQTMLE)
if [[ ! ${sim_types[@]} =~ $sim_type ]];then
    echo "Wrong sim_type! Choose from: ${sim_types[@]}"
    exit 1
fi
radioS=(Co60 Cs137 Ge68 AmC)
calibS=(Co60 Cs137 Ge68 AmC Laser0.1 Laser0.05)

seed_start=0
evtPerJob=1000
jobnum=20
laserN=1
e_energies=0.1
eventRate=1

for s in ${sname[@]}
do
    echo $s
    # if [[ $s == "Co60" ]] || [[ $s == "Cs137" ]];then
    #     path1="ACU"
    # elif [[ $s == "Ge68" ]] || [[ $s == "AmC" ]] || [[ ${s:0:5} == "Laser" ]];then
    #     path1="ACU-CLS"
    # else
        path1="."
    # fi

    if [[ $s == "e+" ]];then
        # e_energies=0_1_2_3_4_5_6_7_8_9_10_0~10
        # e_energies=0_1_2_3_4_5_6_7_8_9_10

        e_energies=0_0.5_1_2_3_4_5_8
		# e_energies=1
        jobnum=100
    elif [[ $s == "SpaNeu" ]];then
        jobnum=5000
    elif [[ $s == "C14" ]];then
        jobnum=50
    else
        jobnum=20
    fi

    if [[ $sim_type == "detsim" ]];then
        if [[ $s == "Laser0.05" ]];then
            laserN=11522
			evtPerJob=1000
        elif [[ $s == "Laser0.1" ]];then
            laserN=22000
			evtPerJob=1000
        elif [[ $s == "e+" ]];then
            evtPerJob=500
        elif [[ $s == "SpaNeu" ]];then
            evtPerJob=100
        elif [[ $s == "C14" ]];then
            evtPerJob=400000
        else
            evtPerJob=1000
        fi
    elif [[ $sim_type == "elecsim" ]];then
        evtPerJob=-1
        if [[ $s == "e+" ]];then
            eventRate=1 ## shoud be 1, modify later!
        elif [[ ${calibS[@]} =~ $s ]];then
            eventRate=100
        elif [[ $s == "SpaNeu" ]];then
            eventRate=6
        elif [[ $s == "C14" ]];then
            eventRate=40000
            evtPerJob=20000
        fi
    elif [[ $sim_type == "calib" ]];then
        evtPerJob=-1
    elif [[ $sim_type == "recQTMLE" ]];then
        evtPerJob=-1
    fi

    pos_xyz=()
    if [[ $s == "e+" ]];then
        pos_xyz[0]="Uniform"
    elif [[ $s == "Ge68" ]] || [[ $s == "AmC" ]];then
        pos_xyz[0]="0_0_0"
    # elif [[ ${radioS[@]} =~ $s ]] || [[ $s == "Laser0.05" ]];then
    #     posnum_counter=0
    #     while read lines
    #     do
    #         pos_xyz[${posnum_counter}]=`echo $lines | tr ' ' _`
    #         let ++posnum_counter
    #     done < pos/${path1}_XYZPos.txt
    else
        pos_xyz[0]="Uniform"
    fi

    echo ${#pos_xyz[@]}
    for x_y_z in ${pos_xyz[@]}
    do
        echo "$s $x_y_z"
        n_energy=`echo $e_energies | grep -o _ | wc -l`
        for((ne=0;ne<=$n_energy;ne++))
        do
            e_energy=`echo $e_energies | cut -d_ -f$[$ne+1]`
            bash jw_gen_run.sh $path1 $s $x_y_z $sim_type $evtPerJob $jobnum $seed_start $laserN $e_energy $eventRate
            seed_start=$(($seed_start+$jobnum))
        done
    done

    seed_start=$(($seed_start+10000))
done
