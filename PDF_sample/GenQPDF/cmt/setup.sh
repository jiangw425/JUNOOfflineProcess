# echo "setup GenQPDF  in /afs/ihep.ac.cn/users/l/luoxj/scratchfs_juno_500G/GenCalibPDFVertexAndEnergy/J21v1r0-Pre0"

if test "${CMTROOT}" = ""; then
  CMTROOT=/cvmfs/juno.ihep.ac.cn/centos7_amd64_gcc830/Pre-Release/J21v1r0-branch-python3/ExternalLibs/CMT/v1r26; export CMTROOT
fi
. ${CMTROOT}/mgr/setup.sh
cmtGenQPDFtempfile=`${CMTROOT}/mgr/cmt -quiet build temporary_name`
if test ! $? = 0 ; then cmtGenQPDFtempfile=/tmp/cmt.$$; fi
${CMTROOT}/mgr/cmt setup -sh -pack=GenQPDF -version= -path=/afs/ihep.ac.cn/users/l/luoxj/scratchfs_juno_500G/GenCalibPDFVertexAndEnergy/J21v1r0-Pre0  -no_cleanup $* >${cmtGenQPDFtempfile}
if test $? != 0 ; then
  echo >&2 "${CMTROOT}/mgr/cmt setup -sh -pack=GenQPDF -version= -path=/afs/ihep.ac.cn/users/l/luoxj/scratchfs_juno_500G/GenCalibPDFVertexAndEnergy/J21v1r0-Pre0  -no_cleanup $* >${cmtGenQPDFtempfile}"
  cmtsetupstatus=2
  /bin/rm -f ${cmtGenQPDFtempfile}
  unset cmtGenQPDFtempfile
  return $cmtsetupstatus
fi
cmtsetupstatus=0
. ${cmtGenQPDFtempfile}
if test $? != 0 ; then
  cmtsetupstatus=2
fi
/bin/rm -f ${cmtGenQPDFtempfile}
unset cmtGenQPDFtempfile
return $cmtsetupstatus

