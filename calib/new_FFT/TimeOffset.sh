#!/bin/bash
if [[ $#<=0 ]]; then
    echo "You have not input step number. 1 / 2"
    exit 1
fi
steps=(1 2)
if [[ ! ${steps[@]} =~ $1 ]];then
    echo "Wrong input step number. 1 / 2"
    exit 1
fi
input=$1; shift
path0=`pwd`
# sn=`basename $path0`
sn="Laser0.1"
sfile="${path0}/Elecsim_path/${sn}_elec.list"
if [[ ! -f $sfile ]];then
	echo  "$sfile not exist!"
	exit 1
fi
if [[ $input -eq 1 ]];then
	mkdir -p TimeOffset
	cd TimeOffset
	mkdir -p run
	mkdir -p log
	mkdir -p user-root

	count=`sed -n '$=' ${sfile}`
	echo "Total file num: $count."
	run=0
	for filename in `cat $sfile`
	do
		jobname="run/${sn}_TimeOffset_${run}.sh"
		echo $filename
		echo "#!/bin/bash" > ${jobname}
		echo "source /junofs/users/jiangw/juno_dev/bashrc" >> ${jobname}
		echo "cd $path0/TimeOffset" >> ${jobname}
		echo "(time python ${path0}/CppFiles/run.py --evtmax -1 --input $filename --user-output user-root/user_out_${run}.root --option linear --positions 0 0 0 --hitsOption all --trigger force --forceTriggerSigma 0.0 ) &> log/log_${run}.txt" >> ${jobname}
		let ++run
	done
	chmod +x run/${sn}_TimeOffset_*.sh
	hep_sub run/${sn}_TimeOffset_"%{ProcId}".sh -n $count
	cd -
elif [[ $input -eq 2 ]];then
	cd TimeOffset
	source /junofs/users/jiangw/juno_dev/bashrc
	if [[ ! -f "time_corr.root" ]];then
		hadd -f time_corr.root user-root/user_out_*.root
	fi
	if [[ -f "time_corr.root" ]];then
		rm -rf user-root/user_out_*.root
	fi
	if [[ ! -f "timeOffset_roofit.txt" ]];then
		time root -l -b -q "$path0/CppFiles/rooRectimeH.C" > fit.log
	fi
	root "${path0}/CppFiles/drawTFS.C"
	mv TimeOffset.png $path0/CheckStation
	cp timeOffset_sub_roofit.txt $path0/Parameters
fi
