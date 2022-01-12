#!/bin/bash
echo "You are in `pwd`"
#echo "Now, checking job logfiles."
snum=0
stime=0
totN=293
if [[ `ls *.txt | wc -l` -ne $totN ]];then
	for n in {0..292}
	do
		if [[ ! -f pdflog-${n}.txt ]];then
			job_name=`ls ../run | grep "[-,_]${n}.sh" | awk 'NR==1'`
			hep_sub ../run/$job_name -e /dev/null -o /dev/null -wt short
		fi
	done
fi

filelist=`ls | grep .txt`
path0=`pwd`
resub=0
if [[ $1 == "resub" ]];then
	resub=1
elif [[ $# -eq 1 ]];then
	filelist=$1
	# one log txt to check
elif [[ `ls | grep ".txt"` == "" ]];then
	echo "Erorr! No log txt files in `pwd`"
	exit 1
fi
echo "tmp" > tmp_running_jobs.tmp
hep_q -u | grep "R" >> tmp_running_jobs.tmp
for f in `echo $filelist`
do
	tail30=`tail -n 40 $f`
	if [[ `echo "$tail30" | tail -n 7 | grep -w "Successfully"` ]] && [[ `echo "$tail30" | grep "rror\|#"` == "" ]];then
		# COTI log files are not recommended
		let "snum=snum+1"
		dtime=`echo "$tail30" | tail -n 3 | head -n 1 | awk '{print $2}'` 
		tmp_m=`echo "${dtime%m*}"`
		tmp_s=`echo "${dtime#*m}" | cut -ds -f1`
		stime=`echo "scale=2; $stime+$tmp_m*60+$tmp_s" | bc`
	else
		run=`echo ${f##*-} | cut -d. -f1`
		job_name=`ls ../run | grep "[-,_]${run}.sh" | awk 'NR==1'`
		#echo -e "$job_name\c"
		if [[ -f "${path0}/tmp_running_jobs.tmp" ]] && [[ `cat ${path0}/tmp_running_jobs.tmp | grep -w "$job_name"` ]];then
			echo "$job_name is still running!"
		else
			echo "$path0/$f not success"
	
			if [[ $resub -eq 1 ]];then
				hep_sub ../run/$job_name -e /dev/null -o /dev/null -wt short
				# -mem 3000
				cd $path0
			fi
		fi
	fi
done
if [[ $snum -eq 0 ]];then
	echo "No success files!"
else
	stime=`echo "$stime/$snum" | bc`
	echo "success file number: $snum"
	echo "success file time avg: `echo "$stime/60" | bc`m`echo "$stime%60" | bc`s"
fi
rm -f tmp_running_jobs.tmp
#rm -f cktmpjwbash.txt

