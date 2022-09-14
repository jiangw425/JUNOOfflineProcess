#!/bin/bash
if [[ $#<3 ]]; then
    echo "You have not input parameters: script's full dir & tmpworkout dir id & subjobname."
    exit 1
fi
DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )"
subjob="$DIR/../tmpworkout/step$2/$3.sh"

cd $1
declare -a jbs
for script in `ls *.sh`
do
    jbs=("${jbs[@]}" "${script}")
done
cd -

echo -e "#!/bin/bash

procid=\$1

job_scripts=(${jbs[@]})

\"$1/\"\"\${job_scripts[\$procid]}\"

" > $subjob

chmod 755 $subjob
jobnum=${#jbs[@]}

hep_sub $subjob -argu "%{ProcId}" -n $jobnum
