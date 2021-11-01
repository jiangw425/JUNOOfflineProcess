This is used to generate detsim files.


========== Get your offline: take J21v1r0-Pre2 as example: ==========
svn co https://juno.ihep.ac.cn/svn/offline/tags/J21v1r0-Pre2 offline

echo "export JUNOTOP=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J21v1r0-Pre2/" > bashrc
echo -e "export WORKTOP=`pwd`\n" >> bashrc
echo -e "export JUNO_OFFLINE_OFF=1 # disable official offline before source setup.sh\n" >> bashrc
echo -e "source \$JUNOTOP/setup.sh\n" >> bashrc
echo -e "export CMTPROJECTPATH=\$WORKTOP:\$CMTPROJECTPATH # prepend WORKTOP\n" >> bashrc
echo "if [ -d \"\$WORKTOP/offline/Examples/Tutorial/cmt/\" ]; then" >> bashrc
echo "    pushd \$WORKTOP/offline/Examples/Tutorial/cmt" >> bashrc
echo "    source setup.sh" >> bashrc
echo "    popd" >> bashrc
echo "fi" >> bashrc

source /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J21v1r0-Pre2/setup.sh
cd offline/Examples/Tutorial/cmt
cmt br cmt config
source setup.sh
source cleanup.sh
cmt br cmt make