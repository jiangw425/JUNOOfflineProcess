#-- start of make_header -----------------

#====================================
#  Library GenCalibPDF
#
#   Generated Mon Jun 28 20:46:27 2021  by luoxj
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_GenCalibPDF_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_GenCalibPDF_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_GenCalibPDF

GenCalibPDFVertexAndEnergy_tag = $(tag)

#cmt_local_tagfile_GenCalibPDF = $(GenCalibPDFVertexAndEnergy_tag)_GenCalibPDF.make
cmt_local_tagfile_GenCalibPDF = $(bin)$(GenCalibPDFVertexAndEnergy_tag)_GenCalibPDF.make

else

tags      = $(tag),$(CMTEXTRATAGS)

GenCalibPDFVertexAndEnergy_tag = $(tag)

#cmt_local_tagfile_GenCalibPDF = $(GenCalibPDFVertexAndEnergy_tag).make
cmt_local_tagfile_GenCalibPDF = $(bin)$(GenCalibPDFVertexAndEnergy_tag).make

endif

include $(cmt_local_tagfile_GenCalibPDF)
#-include $(cmt_local_tagfile_GenCalibPDF)

ifdef cmt_GenCalibPDF_has_target_tag

cmt_final_setup_GenCalibPDF = $(bin)setup_GenCalibPDF.make
cmt_dependencies_in_GenCalibPDF = $(bin)dependencies_GenCalibPDF.in
#cmt_final_setup_GenCalibPDF = $(bin)GenCalibPDFVertexAndEnergy_GenCalibPDFsetup.make
cmt_local_GenCalibPDF_makefile = $(bin)GenCalibPDF.make

else

cmt_final_setup_GenCalibPDF = $(bin)setup.make
cmt_dependencies_in_GenCalibPDF = $(bin)dependencies.in
#cmt_final_setup_GenCalibPDF = $(bin)GenCalibPDFVertexAndEnergysetup.make
cmt_local_GenCalibPDF_makefile = $(bin)GenCalibPDF.make

endif

#cmt_final_setup = $(bin)setup.make
#cmt_final_setup = $(bin)GenCalibPDFVertexAndEnergysetup.make

#GenCalibPDF :: ;

dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	$(echo) 'GenCalibPDF'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = GenCalibPDF/
#GenCalibPDF::
#	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

${CMTROOT}/src/Makefile.core : ;
ifdef use_requirements
$(use_requirements) : ;
endif

#-- end of make_header ------------------
#-- start of libary_header ---------------

GenCalibPDFlibname   = $(bin)$(library_prefix)GenCalibPDF$(library_suffix)
GenCalibPDFlib       = $(GenCalibPDFlibname).a
GenCalibPDFstamp     = $(bin)GenCalibPDF.stamp
GenCalibPDFshstamp   = $(bin)GenCalibPDF.shstamp

GenCalibPDF :: dirs  GenCalibPDFLIB
	$(echo) "GenCalibPDF ok"

cmt_GenCalibPDF_has_prototypes = 1

#--------------------------------------

ifdef cmt_GenCalibPDF_has_prototypes

GenCalibPDFprototype :  ;

endif

GenCalibPDFcompile : $(bin)GenCalibPDF.o ;

#-- end of libary_header ----------------
#-- start of libary ----------------------

GenCalibPDFLIB :: $(GenCalibPDFlib) $(GenCalibPDFshstamp)
	$(echo) "GenCalibPDF : library ok"

$(GenCalibPDFlib) :: $(bin)GenCalibPDF.o
	$(lib_echo) "static library $@"
	$(lib_silent) [ ! -f $@ ] || \rm -f $@
	$(lib_silent) $(ar) $(GenCalibPDFlib) $(bin)GenCalibPDF.o
	$(lib_silent) $(ranlib) $(GenCalibPDFlib)
	$(lib_silent) cat /dev/null >$(GenCalibPDFstamp)

#------------------------------------------------------------------
#  Future improvement? to empty the object files after
#  storing in the library
#
##	  for f in $?; do \
##	    rm $${f}; touch $${f}; \
##	  done
#------------------------------------------------------------------

#
# We add one level of dependency upon the true shared library 
# (rather than simply upon the stamp file)
# this is for cases where the shared library has not been built
# while the stamp was created (error??) 
#

$(GenCalibPDFlibname).$(shlibsuffix) :: $(GenCalibPDFlib) requirements $(use_requirements) $(GenCalibPDFstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin="$(bin)" ld="$(shlibbuilder)" ldflags="$(shlibflags)" suffix=$(shlibsuffix) libprefix=$(library_prefix) libsuffix=$(library_suffix) $(make_shlib) "$(tags)" GenCalibPDF $(GenCalibPDF_shlibflags)
	$(lib_silent) cat /dev/null >$(GenCalibPDFshstamp)

$(GenCalibPDFshstamp) :: $(GenCalibPDFlibname).$(shlibsuffix)
	$(lib_silent) if test -f $(GenCalibPDFlibname).$(shlibsuffix) ; then cat /dev/null >$(GenCalibPDFshstamp) ; fi

GenCalibPDFclean ::
	$(cleanup_echo) objects GenCalibPDF
	$(cleanup_silent) /bin/rm -f $(bin)GenCalibPDF.o
	$(cleanup_silent) /bin/rm -f $(patsubst %.o,%.d,$(bin)GenCalibPDF.o) $(patsubst %.o,%.dep,$(bin)GenCalibPDF.o) $(patsubst %.o,%.d.stamp,$(bin)GenCalibPDF.o)
	$(cleanup_silent) cd $(bin); /bin/rm -rf GenCalibPDF_deps GenCalibPDF_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
GenCalibPDFinstallname = $(library_prefix)GenCalibPDF$(library_suffix).$(shlibsuffix)

GenCalibPDF :: GenCalibPDFinstall ;

install :: GenCalibPDFinstall ;

GenCalibPDFinstall :: $(install_dir)/$(GenCalibPDFinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(GenCalibPDFinstallname) :: $(bin)$(GenCalibPDFinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(GenCalibPDFinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##GenCalibPDFclean :: GenCalibPDFuninstall

uninstall :: GenCalibPDFuninstall ;

GenCalibPDFuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(GenCalibPDFinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependencies ------------------
ifneq ($(MAKECMDGOALS),GenCalibPDFclean)
ifneq ($(MAKECMDGOALS),uninstall)
ifneq ($(MAKECMDGOALS),GenCalibPDFprototype)

$(bin)GenCalibPDF_dependencies.make : $(use_requirements) $(cmt_final_setup_GenCalibPDF)
	$(echo) "(GenCalibPDF.make) Rebuilding $@"; \
	  $(build_dependencies) -out=$@ -start_all $(src)GenCalibPDF.cc -end_all $(includes) $(app_GenCalibPDF_cppflags) $(lib_GenCalibPDF_cppflags) -name=GenCalibPDF $? -f=$(cmt_dependencies_in_GenCalibPDF) -without_cmt

-include $(bin)GenCalibPDF_dependencies.make

endif
endif
endif

GenCalibPDFclean ::
	$(cleanup_silent) \rm -rf $(bin)GenCalibPDF_deps $(bin)GenCalibPDF_dependencies.make
#-- end of dependencies -------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),GenCalibPDFclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)GenCalibPDF.d

$(bin)$(binobj)GenCalibPDF.d :

$(bin)$(binobj)GenCalibPDF.o : $(cmt_final_setup_GenCalibPDF)

$(bin)$(binobj)GenCalibPDF.o : $(src)GenCalibPDF.cc
	$(cpp_echo) $(src)GenCalibPDF.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(GenCalibPDF_pp_cppflags) $(lib_GenCalibPDF_pp_cppflags) $(GenCalibPDF_pp_cppflags) $(use_cppflags) $(GenCalibPDF_cppflags) $(lib_GenCalibPDF_cppflags) $(GenCalibPDF_cppflags) $(GenCalibPDF_cc_cppflags)  $(src)GenCalibPDF.cc
endif
endif

else
$(bin)GenCalibPDF_dependencies.make : $(GenCalibPDF_cc_dependencies)

$(bin)GenCalibPDF_dependencies.make : $(src)GenCalibPDF.cc

$(bin)$(binobj)GenCalibPDF.o : $(GenCalibPDF_cc_dependencies)
	$(cpp_echo) $(src)GenCalibPDF.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(GenCalibPDF_pp_cppflags) $(lib_GenCalibPDF_pp_cppflags) $(GenCalibPDF_pp_cppflags) $(use_cppflags) $(GenCalibPDF_cppflags) $(lib_GenCalibPDF_cppflags) $(GenCalibPDF_cppflags) $(GenCalibPDF_cc_cppflags)  $(src)GenCalibPDF.cc

endif

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: GenCalibPDFclean ;
#	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(GenCalibPDF.make) $@: No rule for such target" >&2
else
.DEFAULT::
	$(error PEDANTIC: $@: No rule for such target)
endif

GenCalibPDFclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library GenCalibPDF
	-$(cleanup_silent) cd $(bin) && \rm -f $(library_prefix)GenCalibPDF$(library_suffix).a $(library_prefix)GenCalibPDF$(library_suffix).$(shlibsuffix) GenCalibPDF.stamp GenCalibPDF.shstamp
#-- end of cleanup_library ---------------
