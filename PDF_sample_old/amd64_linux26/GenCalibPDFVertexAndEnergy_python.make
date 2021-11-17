#-- start of make_header -----------------

#====================================
#  Document GenCalibPDFVertexAndEnergy_python
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

cmt_GenCalibPDFVertexAndEnergy_python_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_GenCalibPDFVertexAndEnergy_python_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_GenCalibPDFVertexAndEnergy_python

GenCalibPDFVertexAndEnergy_tag = $(tag)

#cmt_local_tagfile_GenCalibPDFVertexAndEnergy_python = $(GenCalibPDFVertexAndEnergy_tag)_GenCalibPDFVertexAndEnergy_python.make
cmt_local_tagfile_GenCalibPDFVertexAndEnergy_python = $(bin)$(GenCalibPDFVertexAndEnergy_tag)_GenCalibPDFVertexAndEnergy_python.make

else

tags      = $(tag),$(CMTEXTRATAGS)

GenCalibPDFVertexAndEnergy_tag = $(tag)

#cmt_local_tagfile_GenCalibPDFVertexAndEnergy_python = $(GenCalibPDFVertexAndEnergy_tag).make
cmt_local_tagfile_GenCalibPDFVertexAndEnergy_python = $(bin)$(GenCalibPDFVertexAndEnergy_tag).make

endif

include $(cmt_local_tagfile_GenCalibPDFVertexAndEnergy_python)
#-include $(cmt_local_tagfile_GenCalibPDFVertexAndEnergy_python)

ifdef cmt_GenCalibPDFVertexAndEnergy_python_has_target_tag

cmt_final_setup_GenCalibPDFVertexAndEnergy_python = $(bin)setup_GenCalibPDFVertexAndEnergy_python.make
cmt_dependencies_in_GenCalibPDFVertexAndEnergy_python = $(bin)dependencies_GenCalibPDFVertexAndEnergy_python.in
#cmt_final_setup_GenCalibPDFVertexAndEnergy_python = $(bin)GenCalibPDFVertexAndEnergy_GenCalibPDFVertexAndEnergy_pythonsetup.make
cmt_local_GenCalibPDFVertexAndEnergy_python_makefile = $(bin)GenCalibPDFVertexAndEnergy_python.make

else

cmt_final_setup_GenCalibPDFVertexAndEnergy_python = $(bin)setup.make
cmt_dependencies_in_GenCalibPDFVertexAndEnergy_python = $(bin)dependencies.in
#cmt_final_setup_GenCalibPDFVertexAndEnergy_python = $(bin)GenCalibPDFVertexAndEnergysetup.make
cmt_local_GenCalibPDFVertexAndEnergy_python_makefile = $(bin)GenCalibPDFVertexAndEnergy_python.make

endif

#cmt_final_setup = $(bin)setup.make
#cmt_final_setup = $(bin)GenCalibPDFVertexAndEnergysetup.make

#GenCalibPDFVertexAndEnergy_python :: ;

dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	$(echo) 'GenCalibPDFVertexAndEnergy_python'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = GenCalibPDFVertexAndEnergy_python/
#GenCalibPDFVertexAndEnergy_python::
#	@if test ! -d $(bin)$(binobj) ; then $(mkdir) -p $(bin)$(binobj) ; fi
#	$(echo) "STRUCTURED_OUTPUT="$(bin)$(binobj)
endif

${CMTROOT}/src/Makefile.core : ;
ifdef use_requirements
$(use_requirements) : ;
endif

#-- end of make_header ------------------
#-- start of install_python_header ------


installarea = ${CMTINSTALLAREA}
install_python_dir = $(installarea)

ifneq ($(strip "$(source)"),"")
src = ../$(source)
dest = $(install_python_dir)/python
else
src = ../python
dest = $(install_python_dir)
endif

ifneq ($(strip "$(offset)"),"")
dest = $(install_python_dir)/python
endif

GenCalibPDFVertexAndEnergy_python :: GenCalibPDFVertexAndEnergy_pythoninstall

install :: GenCalibPDFVertexAndEnergy_pythoninstall

GenCalibPDFVertexAndEnergy_pythoninstall :: $(install_python_dir)
	@if [ ! "$(installarea)" = "" ] ; then\
	  echo "installation done"; \
	fi

$(install_python_dir) ::
	@if [ "$(installarea)" = "" ] ; then \
	  echo "Cannot install header files, no installation source specified"; \
	else \
	  if [ -d $(src) ] ; then \
	    echo "Installing files from $(src) to $(dest)" ; \
	    if [ "$(offset)" = "" ] ; then \
	      $(install_command) --exclude="*.py?" $(src) $(dest) ; \
	    else \
	      $(install_command) --exclude="*.py?" $(src) $(dest) --destname $(offset); \
	    fi ; \
	  else \
	    echo "no source  $(src)"; \
	  fi; \
	fi

GenCalibPDFVertexAndEnergy_pythonclean :: GenCalibPDFVertexAndEnergy_pythonuninstall

uninstall :: GenCalibPDFVertexAndEnergy_pythonuninstall

GenCalibPDFVertexAndEnergy_pythonuninstall ::
	@if test "$(installarea)" = ""; then \
	  echo "Cannot uninstall header files, no installation source specified"; \
	else \
	  echo "Uninstalling files from $(dest)"; \
	  $(uninstall_command) "$(dest)" ; \
	fi


#-- end of install_python_header ------
#-- start of cleanup_header --------------

clean :: GenCalibPDFVertexAndEnergy_pythonclean ;
#	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(GenCalibPDFVertexAndEnergy_python.make) $@: No rule for such target" >&2
else
.DEFAULT::
	$(error PEDANTIC: $@: No rule for such target)
endif

GenCalibPDFVertexAndEnergy_pythonclean ::
#-- end of cleanup_header ---------------
