# echo "setup J21v1r0-Pre0 GenQPDF_v2 in /afs/ihep.ac.cn/users/l/luoxj/scratchfs_juno_500G/GenCalibPDFVertexAndEnergy"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J21v1r0-branch-python3/ExternalLibs/CMT/v1r26
endif
source ${CMTROOT}/mgr/setup.csh
set cmtJ21v1r0_Pre0tempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set cmtJ21v1r0_Pre0tempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt setup -csh -pack=J21v1r0-Pre0 -version=GenQPDF_v2 -path=/afs/ihep.ac.cn/users/l/luoxj/scratchfs_juno_500G/GenCalibPDFVertexAndEnergy  -no_cleanup $* >${cmtJ21v1r0_Pre0tempfile}
if ( $status != 0 ) then
  echo "${CMTROOT}/mgr/cmt setup -csh -pack=J21v1r0-Pre0 -version=GenQPDF_v2 -path=/afs/ihep.ac.cn/users/l/luoxj/scratchfs_juno_500G/GenCalibPDFVertexAndEnergy  -no_cleanup $* >${cmtJ21v1r0_Pre0tempfile}"
  set cmtsetupstatus=2
  /bin/rm -f ${cmtJ21v1r0_Pre0tempfile}
  unset cmtJ21v1r0_Pre0tempfile
  exit $cmtsetupstatus
endif
set cmtsetupstatus=0
source ${cmtJ21v1r0_Pre0tempfile}
if ( $status != 0 ) then
  set cmtsetupstatus=2
endif
/bin/rm -f ${cmtJ21v1r0_Pre0tempfile}
unset cmtJ21v1r0_Pre0tempfile
exit $cmtsetupstatus

