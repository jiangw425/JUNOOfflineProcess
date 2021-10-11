#!/bin/bash
case $# in 
    0)
        echo "You have not input step number."
        echo -e "One input number:  FFT step.\nTwo input numbers: COTI step + (any)."
        exit 1
    ;;
    1)
        echo "Default calibration for FFT."
    ;;
    2)
        echo "The calibration for COTI."
    ;;
    *)
        echo "Wrong inputs!"
        exit 2
    ;;
esac

### SPE charge spectra source name (C14 or forceTrigger) ###
scssn=C14
steps=(01 02 03 04 1 2 3 4 11 12 13 14)
cname=deconv
opt=0
if [[ $# -eq 2 ]];then
    echo "Specified for COTI calibration, in order to remind." > COTI_calibration.remind
elif [[ -f "COTI_calibration.remind" ]];then
    if [[ $# -eq 1 ]];then
        echo -e "I find that you have run COTI calibration but entered one input. Don't worry, I know and have fixed that!\n"
    fi
    steps=(1 2 3 4 11 12 13 14)
    cname=inte
    opt=1
fi

if [[ ! ${steps[@]} =~ $1 ]];then
    echo "Wrong step number."
    echo "Please choose from: ${steps[@]}"
    exit 3
fi

echo "`date` on `hostname`: `whoami` runs step $1." >> calibration_recorder.txt

path0=`pwd`
bpath=$path0/BashFiles
cpath=$path0/CppFiles
junoenv=JUNOOFFLINE
envpath=ENVPATH

mv2CheckStation(){
    mv -f $1 $path0/CheckStation/$2
}

if [[ ${1:0:1} -eq 0 ]];then
    sname=(Ge68)
    for s in ${sname[@]};do
        filenum=`sed -n '$=' $path0/Elecsim_path/${s}_elec.list`
        tmpfilename=`head -n +1 $path0/Elecsim_path/${s}_elec.list`
        tfn=`echo ${tmpfilename%[-]*}`
        ### tfn= xxx/xx/elecsim
        if [[ ${1:1:1} -eq 2 ]];then
            if [[ ! -f "$s/step01/user-root/SPE_step1.root" ]] || [[ ! -f "$s/step01/user-root/Integral.txt" ]];then
                source $junoenv
                cd $s/step01/user-root
                bash $bpath/hadd_per100.sh SPE_step1.root ../root/SPE_step01_
                root -b -l -q "$cpath/script1.C"
                mv2CheckStation SPE_step1_fit.pdf
                cd $path0
            fi
        elif [[ ${1:1:1} -eq 3 ]];then
            source $junoenv
            cd $s/step02/user-root
            if [[ ! -f "SPE_bck.root" ]];then
                bash $bpath/hadd_per100.sh SPE_bck.root ../root/SPE_step02_
            # else
            #     rm -rf !\(SPE_bck.root\)
            fi
            if [[ ! -f "SPE.root" ]];then
                root -b -l -q "$cpath/script2.C"
                mv2CheckStation SPE_spectra.png SPE.png
                mv2CheckStation SPE_amp_F.png
                mv2CheckStation SPE_re_F.png
                mv2CheckStation SPE_im_F.png
            fi
            cd $path0

            ####### here to generate filter
            cd $s/step03/root
            if true;then
                for((n=0;n<${filenum};n++))
                do
                    sed -e "s#ELECROOTFILE#$tfn#g" -e "s#NUM#$n#g" $cpath/get_raw2D.C > get_raw2D_${n}.C
                    sed -e "s#ENV#$junoenv#g" -e "s#NUM#$n#g" -e "s#PATH#`pwd`#g" $bpath/run-sample-step03.sh > ../run/filter-job-${n}.sh
                done
                chmod +x ../run/*.sh
                bash $bpath/subAll.sh $path0/$s/step$1/run $1 ${s}_calib_step$1
            fi
            continue

        elif [[ ${1:1:1} -eq 4 ]];then
            source $junoenv
            cd $s/step03/user-root
            if [[ ! -f "../../step02/user-root/filter.root" ]];then
                sed -e "s#FIUM#$filenum#g" $cpath/getFilter.C > getFilter.C
                root -b -l -q 'getFilter.C'
                mv2CheckStation filter_all.png filter.png
                mv filter.root ../../step02/user-root
            fi
            cd $path0
            cd $s/step02/user-root
            if [[ ! -f "waves.root" ]];then
                sed "s#ELECROOTFILE#${tmpfilename%[0-9]*}\*#g" $cpath/extractWaves.C > extractWaves.C
                root -b -l -q 'extractWaves.C'
            fi
            if [[ ! -f "waves_trans.root" ]];then
                root -b -l -q "$cpath/filterWaves.C"
            fi
            if [[ ! -f "res_tmp.root" ]];then
                root -b -l -q "$cpath/drawamp.C"
                mv2CheckStation amprmshn_tmp.png inteVSinteWidth.png
            fi
            # echo "INFO: step$1 complete."
            cd $path0
            # continue
        # elif [[ ${1:1:1} -eq 4 ]];then
            # echo "Auto modify the inteW according to the results from res_tmp.root"
            # source $junoenv
            cd $s/step02/user-root
            root -b -l -q $cpath/getInteW.C
            if [[ ! -f "Amp_threshold_rate.txt" ]];then
                # sed -e "s#NNVTWIDTH#12#g" -e "s#HMMTWIDTH#12#g" -e "s#NNVTINTEW#`head -n 1 inteW.txt`#g" -e "s#HMMTINTEW#`tail -n 1 inteW.txt`#g" $cpath/drawamp2.C > drawamp2.C
                sed -e "s#NNVTWIDTH#12#g" -e "s#HMMTWIDTH#12#g" -e "s#NNVTINTEW#8#g" -e "s#HMMTINTEW#8#g" $cpath/drawamp2.C > drawamp2.C
                root -b -l -q 'drawamp2.C'
                mv2CheckStation inteW.txt
                mv2CheckStation figwvsq_tmp.gif AmpThreshold.gif
            fi
            cp $cpath/getThreshold.C .
            root -b -l -q getThreshold.C
            mv2CheckStation Amp_threshold_1d.png
            cd $path0
            echo "INFO: step$1 complete."
            continue
        fi
        if [[ ${1:1:1} -le 2 ]];then
            bash $bpath/gen_script.sh $s $1 $opt
            if [[ $? -ne 0 ]];then
                echo "Scripts generation fail at step$1."
                exit 4
            fi
            bash $bpath/subAll.sh $path0/$s/step$1/run $1 ${s}_calib_step$1
        fi
    done
elif [[ ${1: -1} -eq 1 ]] || [[ ${1: -1} -eq 2 ]] || [[ ${1: -1} -eq 3 ]]; then
    if [[ $1 -eq 1 ]];then
        echo "Before this step, make sure you have modified relatative parameters: Amp, width threshold and inteW"
        bash $bpath/mkdir.sh
    fi
    if [[ $1 -lt 10 ]];then
        sname=(AmC Ge68 Co60 Cs137 Laser0.1 Laser0.05)
    elif [[ ${scssn} == "C14" ]];then
        sname=(${scssn} Laser0.1 Laser0.05)
    elif [[ ${scssn} == "forceTrigger" ]];then
        sname=(${scssn})
    fi
    for s in ${sname[@]}
    do
        if [[ ${1: -1} -eq 2 ]];then
            cd $s/step$(($1-1))/root
            echo -e $(ls *.root | sed "s:^:`pwd`/: ")|tr ' ' '\n' > ../../step$1/${s}_calib.list
            cd $path0 >/dev/null 2>&1
        fi
        if [[ ${1: -1} -eq 3 ]];then
            hep_sub 3Third_calib.sh -argu $s $1
            continue
        fi
        bash $bpath/gen_script.sh $s $1 $opt
        if [[ $? -ne 0 ]];then
            echo "Scripts generation fail at step$1."
            exit 4
        fi
        bash $bpath/subAll.sh $path0/$s/step$1/run $1 ${s}_calib_step$1
    done
elif [[ $1 -eq 4 ]];then
    rm -rf 3Third_calib.sh.*
    cd Parameters
    if [[ ! -f "timeOffset_sub_roofit.txt" ]];then
        if [[ ! -f "../../TFS/Laser0.1/timeOffset_sub_roofit.txt" ]];then
            echo "timeOffset parameters should be copied here: `pwd`"
            exit 6
        else
            cp ../../TFS/Laser0.1/timeOffset_sub_roofit.txt .
        fi
    else
        root -b -l -q "$cpath/calib_gain.C"
    fi
    if [[ -f "CalibPars.txt" ]];then
        mkdir -p $envpath/data/Calibration/PMTCalibSvc/data/
        cp CalibPars.txt $envpath/data/Calibration/PMTCalibSvc/data/PmtPrtData_${cname}.txt
    fi
    cd $path0
elif [[ $1 -eq 14 ]];then
    rm -rf 3Third_calib.sh.*
    cd Parameters
    source $junoenv
    if [[ ! -f "CalibPars_dn.txt" ]];then
        sed "s#SCSSN#$scssn#g" $cpath/script4.C > script4.C
        root -b -l -q "script4.C"
    fi
    if [[ ! -f "CalibPars_DN.txt" ]];then
        sed "s#SCSSN#$scssn#g" $cpath/calib_dn.C > calib_dn.C
        root -b -l -q "calib_dn.C"
    fi

    if [[ ! -f "all_CalibPars.txt" ]] && [[ ! -f "moved_all_CalibPars.txt" ]];then
        sed "s#SCSSN#$scssn#g" $cpath/merge.C > merge.C
        root -b -l -q "merge.C"
    fi
    if [[ -f "all_CalibPars.txt" ]];then
        cp all_CalibPars.txt ${envpath}/data/Calibration/PMTCalibSvc/data/PmtPrtData_${cname}.txt
        mv all_CalibPars.txt moved_all_CalibPars.txt
    fi
    cd $path0
    if [[ ! -f "SPE_moved.remind" ]];then
        cp C14/step12/user-root/user_calibCorr.root ${envpath}/data/Calibration/PMTCalibSvc/data/output_${cname}.root
        echo -e "This file reminds that the C14 SPE spectra file has been copied to:\n${envpath}/data/Calibration/PMTCalibSvc/data/out_${cname}.root" > SPE_moved.remind
        echo "SPE spectra file from C14 has been moved to your envdir."
    fi
fi

