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

junoenv="/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J21v2r0-Pre0/setup.sh"
localenv="/junofs/users/jiangw/22/bashrc"
C14_detsim_path="/afs/ihep.ac.cn/users/v/valprod0/Pre-Releases/J21v1r0-Pre2/11/C14"
SpaNeu_input="/junofs/production/data-production/Pre-Releases/J21v1r0-Pre0/output_neu.root"
# 5 mask opts are default open from J21v2r0-Pre0
# maskOpt="--real-surface-in-cd --enable-optical-surface-in-cd --enable-strut-optical-surface --real-mask-tail --enable-mask-tail-surface"
maskOpt=" "
recMapPath="/scratchfs/juno/jiangw/Time_Grid/SIGNALWINDOW/recMap"

detsim_gen(){
    echo "source ${junoenv}" >> ${name}
    echo "(time python \$TUTORIALROOT/share/tut_detsim.py ${splitOpt} --evtmax ${evtPerJob} --seed ${seed} ${OriginUser} ${commonOut} ${maskOpt} ${detsim_special_opt}) >& log/log-${sim_type}-${n}.txt" >> ${name}
}
elecsim_gen(){
    C14_n=$(($n%50))
    echo "source ${junoenv}" >> ${name}
    echo "(time python \$TUTORIALROOT/share/tut_det2elec.py --evtmax ${evtPerJob} --seed ${seed} --loglevel Fatal --input eventinput:../detsim/root/detsim-${n}.root ${splitOpt} ${commonOut} --rate eventinput:${eventRate} --input C14:${C14_detsim_path}/detsim/root/detsim-${C14_n}.root --rate C14:40000 --loop C14:1 --nHitsThreshold 500) >& log/log-${sim_type}-${n}.txt" >> ${name}
}
elecsim_woC14_gen(){
    if [[ $sname == "C14" ]];then
        NHITSTHRESHOLD=30
        TRIGGER_FIREDPMTNUM=100
    else
        NHITSTHRESHOLD=200
        TRIGGER_FIREDPMTNUM=200
    fi
    echo "source ${junoenv}" >> ${name}
    echo "(time python \$TUTORIALROOT/share/tut_det2elec.py --evtmax ${evtPerJob} --seed ${seed} --loglevel Fatal --input /afs/ihep.ac.cn/users/v/valprod0/Pre-Releases/J21v1r0-Pre0/${sname}/${sname}_${pos_xyz}/${e_energy}MeV/detsim/root/detsim-${n}.root ${splitOpt} ${commonOut} --rate ${eventRate} --nHitsThreshold ${NHITSTHRESHOLD} --Trigger_FiredPmtNum ${TRIGGER_FIREDPMTNUM}) >& log/log-${sim_type}-${n}.txt" >> ${name}
}
calib_gen(){
    echo "source ${junoenv}" >> ${name}
#	echo "source /junofs/users/jiangw/J21v1r0-Pre0/offline/Calibration/PMTCalibSvc/cmt/setup.sh" >> ${name}
#    echo "(time python \$TUTORIALROOT/share/tut_elec2calib.py --evtmax ${evtPerJob} --enableElecTruth --input ../elecsim/root/elecsim-${n}.root ${commonOut}) >& log/log-${sim_type}-${n}.txt" >> ${name}
	echo "export WORKTOP=/junofs/users/jiangw/J21v1r0-Pre0" >> ${name}
	echo "(time python \$TUTORIALROOT/share/tut_elec2calib.py --evtmax ${evtPerJob} --enableElecTruth --CalibFile /junofs/users/jiangw/give2lxj/J21v1r0-Pre0/SPE.root --Filter /junofs/users/jiangw/give2lxj/J21v1r0-Pre0/filter.root --input root://junoeos01.ihep.ac.cn//eos/juno/valprod/valprod0/review/${e_energy}MeV/elecsim/root/elecsim-${n}.root ${commonOut}) >& log/log-${sim_type}-${n}.txt" >> ${name}
}
recQTMLE_gen(){
    Opt_ERec="--method energy-point --enableQTimePdf --enableUseEkMap --enableLTSPEs --enableTimeInfo"
    if [[ $sname == "e+" ]];then
        inputroot="/junofs/production/data-production/Pre-Releases/J21v1r0-Pre2/22/e+/e+_Uniform/${e_energy}MeV/calib/root/calib-${n}.root"
    elif [[ $sname == "Ge68" ]];then
        inputroot="/junofs/production/data-production/Pre-Releases/J21v1r0-Pre2/22/ACU-CLS/Ge68/Ge68_0_0_0/calib/root/calib-${n}.root"
    fi
    echo "source ${localenv}" >> ${name}
    echo "(time python \${TUTORIALROOT}/share/tut_calib2rec.py --evtmax ${evtPerJob} --gdml ${Opt_ERec} --RecMapPath ${recMapPath} --input ${inputroot} ${commonOut} --SignalWindowL SIGNALWINDOW --elec yes) >& log/log-${sim_type}-${n}.txt" >> ${name}
#    echo "rm -f root/${sim_type}-${n}.root" >> ${name}
}

if [[ $sname == "e+" ]];then
    if [[ `echo $e_energy | grep -o \~ | wc -l` -eq 1 ]];then
        is_energy_range=1
        e_min=`echo $e_energy | cut -d~ -f1`
        e_max=`echo $e_energy | cut -d~ -f2`
    else
        is_energy_range=0
    fi
    dir=${path1}/${sname}/${sname}_${pos_xyz}/${e_energy}MeV/${sim_type}
else
    dir=${path1}/${sname}/${sname}_${pos_xyz}/${sim_type}
fi
if [[ ! -d $dir ]];then
    mkdir -p $dir/run
    mkdir -p $dir/user-root
    mkdir -p $dir/root
    mkdir -p $dir/log
    echo "$dir/run" >> $path0/${sim_type}_subdirs.txt
fi
cd $dir
rm -rf run/*${sim_type}*.sh*

for((n=0;n<$jobnum;n++))
do
    if [[ $jobnum -gt 200 ]] && [[ $n*10%$jobnum -eq 0 ]];then
        echo "Generating... $sname $sim_type ($n/$jobnum)"
    fi

    if [[ $sname == "e+" ]];then
        name=run/SIGNALWINDOW_${sname}_${e_energy}MeV_${sim_type}_${n}.sh
    else
        name=run/${sname}_${pos_xyz}_${sim_type}_${n}.sh
    fi
    seed=$(($seed_start+$n))
    echo "#!/bin/bash" > $name
	echo "export XRD_STREAMTIMEOUT=600" >> $name
    echo "cd ${path0}/${dir}" >> $name
    # echo "\$JUNOTESTROOT/production/libs/jobmom.sh \$\$ >& log/log-${sim_type}-${n}.txt.mem.usage &" >> $name
    if [[ $sim_type == "detsim" ]];then
        if [[ $sname == "e+" ]];then
            detsim_special_opt="gun --particles ${sname} ${pos_opt} "
            if [[ $is_energy_range -eq 0 ]];then
                detsim_special_opt+="--momentums ${e_energy} "
            else
                detsim_special_opt+="--momentums ${e_min} --momentums-mode Range --momentums-extra-params ${e_max} "
            fi
            detsim_special_opt+="--momentums-interp KineticEnergy"
        elif [[ ${sname:0:5} == "Laser" ]];then
            detsim_special_opt="photon --totalphotons ${laserN} --global-position ${pos_x} ${pos_y} ${pos_z}"
        elif [[ $sname == "C14" ]];then
            detsim_special_opt="gendecay --nuclear ${sname} ${pos_opt}"
        elif [[ ${radioS[@]} =~ ${sname} ]];then
            detsim_special_opt="--source_weight_QC --OffsetInX ${pos_x} --OffsetInY ${pos_y} --OffsetInZ ${pos_z} hepevt --exe ${sname} --global-position ${pos_x} ${pos_y} ${pos_z}"
        elif [[ $sname == "SpaNeu" ]];then
            SNindex=$(($n*$evtPerJob%3445741))
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

    if [[ $sname == "e+" ]];then
        tmpEOSjw="root://junoeos01.ihep.ac.cn//eos/juno/user/jiangw/TS/SIGNALWINDOW/${e_energy}MeV/${sim_type}/"
    elif [[ $sname == "Ge68" ]];then
        tmpEOSjw="root://junoeos01.ihep.ac.cn//eos/juno/user/jiangw/TS/SIGNALWINDOW/Ge68/${sim_type}/"
    fi
	
    commonOut="--output ${tmpEOSjw}root/${sim_type}-${n}.root --user-output ${tmpEOSjw}user-root/user-${sim_type}-${n}.root"
    if [[ $sname == "e+" ]] || [[ $sname == "C14" ]] && [[ $sim_type == "elecsim" ]];then
        ${sim_type}_woC14_gen
    else
        ${sim_type}_gen
    fi
done
chmod +x run/*.sh

cd run
bash ~jiangw/hepNsub.sh -1 -1
cd ..

cd $path0
