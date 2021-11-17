#-- start of make_header -----------------

#====================================
#  Library GenQPDF
#
#   Generated Wed Oct 13 22:58:51 2021  by jiangw
#
#====================================

include ${CMTROOT}/src/Makefile.core

ifdef tag
CMTEXTRATAGS = $(tag)
else
tag       = $(CMTCONFIG)
endif

cmt_GenQPDF_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_GenQPDF_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_GenQPDF

GenQPDF_tag = $(tag)

#cmt_local_tagfile_GenQPDF = $(GenQPDF_tag)_GenQPDF.make
cmt_local_tagfile_GenQPDF = $(bin)$(GenQPDF_tag)_GenQPDF.make

else

tags      = $(tag),$(CMTEXTRATAGS)

GenQPDF_tag = $(tag)

#cmt_local_tagfile_GenQPDF = $(GenQPDF_tag).make
cmt_local_tagfile_GenQPDF = $(bin)$(GenQPDF_tag).make

endif

include $(cmt_local_tagfile_GenQPDF)
#-include $(cmt_local_tagfile_GenQPDF)

ifdef cmt_GenQPDF_has_target_tag

cmt_final_setup_GenQPDF = $(bin)setup_GenQPDF.make
cmt_dependencies_in_GenQPDF = $(bin)dependencies_GenQPDF.in
#cmt_final_setup_GenQPDF = $(bin)GenQPDF_GenQPDFsetup.make
cmt_local_GenQPDF_makefile = $(bin)GenQPDF.make

else

cmt_final_setup_GenQPDF = $(bin)setup.make
cmt_dependencies_in_GenQPDF = $(bin)dependencies.in
#cmt_final_setup_GenQPDF = $(bin)GenQPDFsetup.make
cmt_local_GenQPDF_makefile = $(bin)GenQPDF.make

endif

#cmt_final_setup = $(bin)setup.make
#cmt_final_setup = $(bin)GenQPDFsetup.make

#GenQPDF :: ;

dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	$(echo) 'GenQPDF'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = GenQPDF/
#GenQPDF::
#	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

${CMTROOT}/src/Makefile.core : ;
ifdef use_requirements
$(use_requirements) : ;
endif

#-- end of make_header ------------------
#-- start of libary_header ---------------

GenQPDFlibname   = $(bin)$(library_prefix)GenQPDF$(library_suffix)
GenQPDFlib       = $(GenQPDFlibname).a
GenQPDFstamp     = $(bin)GenQPDF.stamp
GenQPDFshstamp   = $(bin)GenQPDF.shstamp

GenQPDF :: dirs  GenQPDFLIB
	$(echo) "GenQPDF ok"

cmt_GenQPDF_has_prototypes = 1

#--------------------------------------

ifdef cmt_GenQPDF_has_prototypes

GenQPDFprototype :  ;

endif

GenQPDFcompile : $(bin)GenQPDF.o ;

#-- end of libary_header ----------------
#-- start of libary ----------------------

GenQPDFLIB :: $(GenQPDFlib) $(GenQPDFshstamp)
	$(echo) "GenQPDF : library ok"

$(GenQPDFlib) :: $(bin)GenQPDF.o
	$(lib_echo) "static library $@"
	$(lib_silent) [ ! -f $@ ] || \rm -f $@
	$(lib_silent) $(ar) $(GenQPDFlib) $(bin)GenQPDF.o
	$(lib_silent) $(ranlib) $(GenQPDFlib)
	$(lib_silent) cat /dev/null >$(GenQPDFstamp)

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

$(GenQPDFlibname).$(shlibsuffix) :: $(GenQPDFlib) requirements $(use_requirements) $(GenQPDFstamps)
	$(lib_echo) "shared library $@"
	$(lib_silent) if test "$(makecmd)"; then QUIET=; else QUIET=1; fi; QUIET=$${QUIET} bin="$(bin)" ld="$(shlibbuilder)" ldflags="$(shlibflags)" suffix=$(shlibsuffix) libprefix=$(library_prefix) libsuffix=$(library_suffix) $(make_shlib) "$(tags)" GenQPDF $(GenQPDF_shlibflags)
	$(lib_silent) cat /dev/null >$(GenQPDFshstamp)

$(GenQPDFshstamp) :: $(GenQPDFlibname).$(shlibsuffix)
	$(lib_silent) if test -f $(GenQPDFlibname).$(shlibsuffix) ; then cat /dev/null >$(GenQPDFshstamp) ; fi

GenQPDFclean ::
	$(cleanup_echo) objects GenQPDF
	$(cleanup_silent) /bin/rm -f $(bin)GenQPDF.o
	$(cleanup_silent) /bin/rm -f $(patsubst %.o,%.d,$(bin)GenQPDF.o) $(patsubst %.o,%.dep,$(bin)GenQPDF.o) $(patsubst %.o,%.d.stamp,$(bin)GenQPDF.o)
	$(cleanup_silent) cd $(bin); /bin/rm -rf GenQPDF_deps GenQPDF_dependencies.make

#-----------------------------------------------------------------
#
#  New section for automatic installation
#
#-----------------------------------------------------------------

install_dir = ${CMTINSTALLAREA}/$(tag)/lib
GenQPDFinstallname = $(library_prefix)GenQPDF$(library_suffix).$(shlibsuffix)

GenQPDF :: GenQPDFinstall ;

install :: GenQPDFinstall ;

GenQPDFinstall :: $(install_dir)/$(GenQPDFinstallname)
ifdef CMTINSTALLAREA
	$(echo) "installation done"
endif

$(install_dir)/$(GenQPDFinstallname) :: $(bin)$(GenQPDFinstallname)
ifdef CMTINSTALLAREA
	$(install_silent) $(cmt_install_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(GenQPDFinstallname)" \
	    -out "$(install_dir)" \
	    -cmd "$(cmt_installarea_command)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

##GenQPDFclean :: GenQPDFuninstall

uninstall :: GenQPDFuninstall ;

GenQPDFuninstall ::
ifdef CMTINSTALLAREA
	$(cleanup_silent) $(cmt_uninstall_action) \
	    -source "`(cd $(bin); pwd)`" \
	    -name "$(GenQPDFinstallname)" \
	    -out "$(install_dir)" \
	    -cmtpath "$($(package)_cmtpath)"
endif

#-- end of libary -----------------------
#-- start of dependencies ------------------
ifneq ($(MAKECMDGOALS),GenQPDFclean)
ifneq ($(MAKECMDGOALS),uninstall)
ifneq ($(MAKECMDGOALS),GenQPDFprototype)

$(bin)GenQPDF_dependencies.make : $(use_requirements) $(cmt_final_setup_GenQPDF)
	$(echo) "(GenQPDF.make) Rebuilding $@"; \
	  $(build_dependencies) -out=$@ -start_all $(src)GenQPDF.cc -end_all $(includes) $(app_GenQPDF_cppflags) $(lib_GenQPDF_cppflags) -name=GenQPDF $? -f=$(cmt_dependencies_in_GenQPDF) -without_cmt

-include $(bin)GenQPDF_dependencies.make

endif
endif
endif

GenQPDFclean ::
	$(cleanup_silent) \rm -rf $(bin)GenQPDF_deps $(bin)GenQPDF_dependencies.make
#-- end of dependencies -------------------
#-- start of cpp_library -----------------

ifneq (,)

ifneq ($(MAKECMDGOALS),GenQPDFclean)
ifneq ($(MAKECMDGOALS),uninstall)
-include $(bin)$(binobj)GenQPDF.d

$(bin)$(binobj)GenQPDF.d :

$(bin)$(binobj)GenQPDF.o : $(cmt_final_setup_GenQPDF)

$(bin)$(binobj)GenQPDF.o : $(src)GenQPDF.cc
	$(cpp_echo) $(src)GenQPDF.cc
	$(cpp_silent) $(cppcomp)  -o $@ $(use_pp_cppflags) $(GenQPDF_pp_cppflags) $(lib_GenQPDF_pp_cppflags) $(GenQPDF_pp_cppflags) $(use_cppflags) $(GenQPDF_cppflags) $(lib_GenQPDF_cppflags) $(GenQPDF_cppflags) $(GenQPDF_cc_cppflags)  $(src)GenQPDF.cc
endif
endif

else
$(bin)GenQPDF_dependencies.make : $(GenQPDF_cc_dependencies)

$(bin)GenQPDF_dependencies.make : $(src)GenQPDF.cc

$(bin)$(binobj)GenQPDF.o : $(GenQPDF_cc_dependencies)
	$(cpp_echo) $(src)GenQPDF.cc
	$(cpp_silent) $(cppcomp) -o $@ $(use_pp_cppflags) $(GenQPDF_pp_cppflags) $(lib_GenQPDF_pp_cppflags) $(GenQPDF_pp_cppflags) $(use_cppflags) $(GenQPDF_cppflags) $(lib_GenQPDF_cppflags) $(GenQPDF_cppflags) $(GenQPDF_cc_cppflags)  $(src)GenQPDF.cc

endif

#-- end of cpp_library ------------------
#-- start of cleanup_header --------------

clean :: GenQPDFclean ;
#	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(GenQPDF.make) $@: No rule for such target" >&2
else
.DEFAULT::
	$(error PEDANTIC: $@: No rule for such target)
endif

GenQPDFclean ::
#-- end of cleanup_header ---------------
#-- start of cleanup_library -------------
	$(cleanup_echo) library GenQPDF
	-$(cleanup_silent) cd $(bin) && \rm -f $(library_prefix)GenQPDF$(library_suffix).a $(library_prefix)GenQPDF$(library_suffix).$(shlibsuffix) GenQPDF.stamp GenQPDF.shstamp
#-- end of cleanup_library ---------------
