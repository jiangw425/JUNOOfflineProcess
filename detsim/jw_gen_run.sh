#!/bin/bash
# path : path0 / path1 / sname / sname_x_y_z / sim_type 
path0=`pwd`
radioS=(Co60 Cs137 AmC Ge68)

path1=$1
sname=$2
pos_xyz=$3
if [[ $pos_xyz == *_*_* ]];then
    pos_x=`echo $pos_xyz | cut -d_ -f1`
    pos_y=`echo $pos_xyz | cut -d_ -f2`
    pos_z=`echo $pos_xyz | cut -d_ -f3`
    pos_opt=""
elif [[ $pos_xyz == "Uniform" ]];then
    pos_opt="--volume pTarget --material LS"
fi
sim_type=$4

evtPerJob=$5
jobnum=$6
seed_start=$7

laserN=${8}
e_energy=${9}
eventRate=${10}

junoenv="/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J21v1r0-Pre2/setup.sh"
localenv="/junofs/users/jiangw/J21v1r0-Pre2/bashrc"
C14_detsim_path="/afs/ihep.ac.cn/users/v/valprod0/Pre-Releases/J21v1r0-Pre2/11/C14"
SpaNeu_input="/junofs/production/data-production/Pre-Releases/J21v1r0-Pre0/SpallationNeutron/output_neu.root"
maskOpt="--real-surface-in-cd --enable-optical-surface-in-cd --enable-strut-optical-surface --real-mask-tail --enable-mask-tail-surface"
detsim_gen(){
    echo "(time python \$TUTORIALROOT/share/tut_detsim.py ${splitOpt} --evtmax ${evtPerJob} --seed ${seed} ${OriginUser} ${commonOut} ${maskOpt} ${detsim_special_opt}) >& log/log-${sim_type}-${n}.txt" >> ${name}
}
elecsim_gen(){
    C14_n=$(($n%50))
    echo "(time python \$TUTORIALROOT/share/tut_det2elec.py --evtmax ${evtPerJob} --seed ${seed} --loglevel Fatal --input eventinput:../detsim/root/detsim-${n}.root ${splitOpt} ${commonOut} --rate eventinput:${eventRate} --input C14:${C14_detsim_path}/detsim/root/detsim-${C14_n}.root --rate C14:40000 --loop C14:1 --nHitsThreshold 500) >& log/log-${sim_type}-${n}.txt" >> ${name}
}

dir=${path1}/${sname}/${sname}_${pos_xyz}/${sim_type}
if [[ ! -d $dir ]];then
    mkdir -p $dir/run
    mkdir -p $dir/user-root
    mkdir -p $dir/root
    mkdir -p $dir/log
fi
cd $dir
rm -rf run/*${sim_type}*.sh

for((n=0;n<$jobnum;n++))
do
    name=run/${sname}_${pos_xyz}_${sim_type}_${n}.sh
    seed=$(($seed_start+$n))
    echo "#!/bin/bash" > $name
    echo "cd ${path0}/${dir}" >> $name
    echo "source ${junoenv}" >> $name
    # echo "\$JUNOTESTROOT/production/libs/jobmom.sh \$\$ >& log/log-${sim_type}-${n}.txt.mem.usage &" >> $name
    if [[ $sim_type == "detsim" ]];then
        if [[ ${radioS[@]} =~ ${sname} ]];then
            detsim_special_opt="--source_weight_QC --OffsetInX ${pos_x} --OffsetInY ${pos_y} --OffsetInZ ${pos_z} hepevt --exe ${sname} --global-position ${pos_x} ${pos_y} ${pos_z}"
        elif [[ ${sname:0:5} == "Laser" ]];then
            detsim_special_opt="photon --totalphotons ${laserN} --global-position ${pos_x} ${pos_y} ${pos_z}"
        elif [[ $sname == "C14" ]];then
            detsim_special_opt="gendecay --nuclear ${sname} ${pos_opt}"
        elif [[ $sname == "e+" ]];then
            detsim_special_opt="gun --particles ${sname} ${pos_opt} --momentums ${e_energy} --momentums-interp KineticEnergy"
        elif [[ $sname == "SpaNeu" ]];then
            SNindex=$(($n*$evtPerJob))
            detsim_special_opt="neutron --input ${SpaNeu_input} --index ${SNindex}"
        fi

        if [[ $sname == "Ge68" ]];then
            OriginUser="--anamgr-normal-hit"
        else
            OriginUser=""
        fi

        if [[ $sname == "SpaNeu" ]] || [[ $sname == "Muon" ]];then
            splitOpt="--pmtsd-v2 --pmt-hit-type 2 --pmtsd-merge-twindow 1.0 --no-anamgr-optical-parameter --split-maxhits 100000"
        else
            splitOpt=""
        fi

    elif [[ $sim_type == "elecsim" ]];then
        if [[ $sname == "SpaNeu" ]] || [[ $sname == "Muon" ]];then
            splitOpt="--enableSplitInput"
        else
            splitOpt=""
        fi
    fi

    commonOut="--output root/${sim_type}-${n}.root --user-output user-root/user-${sim_type}-${n}.root"
    ${sim_type}_gen
done
chmod +x run/*.sh
cd $path0
