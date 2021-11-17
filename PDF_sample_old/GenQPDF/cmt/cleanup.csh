# echo "cleanup GenQPDF  in /afs/ihep.ac.cn/users/l/luoxj/scratchfs_juno_500G/GenCalibPDFVertexAndEnergy/J21v1r0-Pre0"

if ( $?CMTROOT == 0 ) then
  setenv CMTROOT /cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J21v1r0-branch-python3/ExternalLibs/CMT/v1r26
endif
source ${CMTROOT}/mgr/setup.csh
set cmtGenQPDFtempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if $status != 0 then
  set cmtGenQPDFtempfile=/tmp/cmt.$$
endif
${CMTROOT}/mgr/cmt cleanup -csh -pack=GenQPDF -version= -path=/afs/ihep.ac.cn/users/l/luoxj/scratchfs_juno_500G/GenCalibPDFVertexAndEnergy/J21v1r0-Pre0  $* >${cmtGenQPDFtempfile}
if ( $status != 0 ) then
  echo "${CMTROOT}/mgr/cmt cleanup -csh -pack=GenQPDF -version= -path=/afs/ihep.ac.cn/users/l/luoxj/scratchfs_juno_500G/GenCalibPDFVertexAndEnergy/J21v1r0-Pre0  $* >${cmtGenQPDFtempfile}"
  set cmtcleanupstatus=2
  /bin/rm -f ${cmtGenQPDFtempfile}
  unset cmtGenQPDFtempfile
  exit $cmtcleanupstatus
endif
set cmtcleanupstatus=0
source ${cmtGenQPDFtempfile}
if ( $status != 0 ) then
  set cmtcleanupstatus=2
endif
/bin/rm -f ${cmtGenQPDFtempfile}
unset cmtGenQPDFtempfile
exit $cmtcleanupstatus

