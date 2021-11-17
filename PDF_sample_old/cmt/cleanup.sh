# echo "cleanup J21v1r0-Pre0 GenQPDF_v2 in /afs/ihep.ac.cn/users/l/luoxj/scratchfs_juno_500G/GenCalibPDFVertexAndEnergy"

if test "${CMTROOT}" = ""; then
  CMTROOT=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J21v1r0-branch-python3/ExternalLibs/CMT/v1r26; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh
cmtJ21v1r0_Pre0tempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then cmtJ21v1r0_Pre0tempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt cleanup -sh -pack=J21v1r0-Pre0 -version=GenQPDF_v2 -path=/afs/ihep.ac.cn/users/l/luoxj/scratchfs_juno_500G/GenCalibPDFVertexAndEnergy  $* >${cmtJ21v1r0_Pre0tempfile}
if test $? != 0 ; then
  echo >&2 "${CMTROOT}/mgr/cmt cleanup -sh -pack=J21v1r0-Pre0 -version=GenQPDF_v2 -path=/afs/ihep.ac.cn/users/l/luoxj/scratchfs_juno_500G/GenCalibPDFVertexAndEnergy  $* >${cmtJ21v1r0_Pre0tempfile}"
  cmtcleanupstatus=2
  /bin/rm -f ${cmtJ21v1r0_Pre0tempfile}
  unset cmtJ21v1r0_Pre0tempfile
  return $cmtcleanupstatus
fi
cmtcleanupstatus=0
. ${cmtJ21v1r0_Pre0tempfile}
if test $? != 0 ; then
  cmtcleanupstatus=2
fi
/bin/rm -f ${cmtJ21v1r0_Pre0tempfile}
unset cmtJ21v1r0_Pre0tempfile
return $cmtcleanupstatus

