#!/bin/bash
### not complete! don't use this script.

### used to generate elecsim-calib-recQTMLE mock data in one script!
### e+ as example: 
### Note: no C14 mix in

### run this script on detsim dir. So `ls` will get "detsim" dir output.
path0=`pwd`
junoenv="/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J21v2r0-Pre0/setup.sh"
localenv="/junofs/users/jiangw/22/bashrc"
recMapPath="/scratchfs/juno/jiangw/Time_Grid/420/recMap"
signalwindowl="420"

jobnum=`ls detsim/root/*.root | wc -l`
sim_types=(elecsim calib recQTMLE)
for dir in ${sim_types[@]}; do if [[ ! -d $dir ]];then mkdir $dir; cd $dir; mkdir root user-root log; cd ..; fi; done
mkdir -p full_run
jnb=full_run/${1:-"elec2rec_full_"}

for((n=0;n<${jobnum};n++))
do
    # inputfile="/junofs/users/wangyg/Software/juno-dev/J21v2r0-Pre0/sim_data/Ge68/output/eplus-0MeV-z${z}m-$((n+10001)).root"
    jobname="${jnb}${n}.sh"
    echo "#!/bin/bash" > $jobname

    sim_type="elecsim"
    commonOut="--output root/${sim_type}-${n}.root --user-output user-root/user-${sim_type}-${n}.root"
    echo "source ${junoenv}" >> $jobname
    echo "cd ${path0}/${sim_type}" >> $jobname
    echo "(time python \$TUTORIALROOT/share/tut_det2elec.py --evtmax -1 --seed $((42000+n)) --loglevel Fatal --input ../detsim/root/detsim-${n}.root ${commonOut} --rate 1 --nHitsThreshold 500 --Trigger_FiredPmtNum 200) >& log/log-${sim_type}-${n}.txt" >> $jobname

    sim_type="calib"
    commonOut="--output root/${sim_type}-${n}.root --user-output user-root/user-${sim_type}-${n}.root"
    echo "source ${localenv}" >> $jobname
    echo "cd ${path0}/${sim_type}" >> $jobname
    echo "(time python \$TUTORIALROOT/share/tut_elec2calib.py --evtmax -1 --enableElecTruth --input ../elecsim/root/elecsim-${n}.root ${commonOut}) >& log/log-${sim_type}-${n}.txt" >> $jobname

    sim_type="recQTMLE"
    commonOut="--output root/${sim_type}-${n}.root --user-output user-root/user-${sim_type}-${n}.root"
    Opt_ERec="--method energy-point --enableQTimePdf --enableUseEkMap --enableLTSPEs --enableTimeInfo"
    echo "source ${localenv}" >> $jobname
    echo "cd ${path0}/${sim_type}" >> $jobname
    echo "(time python \${TUTORIALROOT}/share/tut_calib2rec.py --evtmax -1 --gdml ${Opt_ERec} --RecMapPath ${recMapPath} --input ../calib/root/calib-${n}.root ${commonOut} --SignalWindowL ${signalwindowl} --elec yes) >& log/log-${sim_type}-${n}.txt" >> $jobname
    echo "rm -f root/${sim_type}-${n}.root" >> $jobname
done
chmod +x ${jnb}*.sh
bash ~jiangw/hepNsub.sh ${jnb} ${jobnum}
