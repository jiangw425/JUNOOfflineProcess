#-- start of make_header -----------------

#====================================
#  Document GenQPDF_python
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

cmt_GenQPDF_python_has_no_target_tag = 1

#--------------------------------------------------------

ifdef cmt_GenQPDF_python_has_target_tag

tags      = $(tag),$(CMTEXTRATAGS),target_GenQPDF_python

GenQPDF_tag = $(tag)

#cmt_local_tagfile_GenQPDF_python = $(GenQPDF_tag)_GenQPDF_python.make
cmt_local_tagfile_GenQPDF_python = $(bin)$(GenQPDF_tag)_GenQPDF_python.make

else

tags      = $(tag),$(CMTEXTRATAGS)

GenQPDF_tag = $(tag)

#cmt_local_tagfile_GenQPDF_python = $(GenQPDF_tag).make
cmt_local_tagfile_GenQPDF_python = $(bin)$(GenQPDF_tag).make

endif

include $(cmt_local_tagfile_GenQPDF_python)
#-include $(cmt_local_tagfile_GenQPDF_python)

ifdef cmt_GenQPDF_python_has_target_tag

cmt_final_setup_GenQPDF_python = $(bin)setup_GenQPDF_python.make
cmt_dependencies_in_GenQPDF_python = $(bin)dependencies_GenQPDF_python.in
#cmt_final_setup_GenQPDF_python = $(bin)GenQPDF_GenQPDF_pythonsetup.make
cmt_local_GenQPDF_python_makefile = $(bin)GenQPDF_python.make

else

cmt_final_setup_GenQPDF_python = $(bin)setup.make
cmt_dependencies_in_GenQPDF_python = $(bin)dependencies.in
#cmt_final_setup_GenQPDF_python = $(bin)GenQPDFsetup.make
cmt_local_GenQPDF_python_makefile = $(bin)GenQPDF_python.make

endif

#cmt_final_setup = $(bin)setup.make
#cmt_final_setup = $(bin)GenQPDFsetup.make

#GenQPDF_python :: ;

dirs ::
	@if test ! -r requirements ; then echo "No requirements file" ; fi; \
	  if test ! -d $(bin) ; then $(mkdir) -p $(bin) ; fi

javadirs ::
	@if test ! -d $(javabin) ; then $(mkdir) -p $(javabin) ; fi

srcdirs ::
	@if test ! -d $(src) ; then $(mkdir) -p $(src) ; fi

help ::
	$(echo) 'GenQPDF_python'

binobj = 
ifdef STRUCTURED_OUTPUT
binobj = GenQPDF_python/
#GenQPDF_python::
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

GenQPDF_python :: GenQPDF_pythoninstall

install :: GenQPDF_pythoninstall

GenQPDF_pythoninstall :: $(install_python_dir)
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

GenQPDF_pythonclean :: GenQPDF_pythonuninstall

uninstall :: GenQPDF_pythonuninstall

GenQPDF_pythonuninstall ::
	@if test "$(installarea)" = ""; then \
	  echo "Cannot uninstall header files, no installation source specified"; \
	else \
	  echo "Uninstalling files from $(dest)"; \
	  $(uninstall_command) "$(dest)" ; \
	fi


#-- end of install_python_header ------
#-- start of cleanup_header --------------

clean :: GenQPDF_pythonclean ;
#	@cd .

ifndef PEDANTIC
.DEFAULT::
	$(echo) "(GenQPDF_python.make) $@: No rule for such target" >&2
else
.DEFAULT::
	$(error PEDANTIC: $@: No rule for such target)
endif

GenQPDF_pythonclean ::
#-- end of cleanup_header ---------------
