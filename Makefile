SOURCES = send.c

APPMOD = PD4

PROJDIR = $(CURDIR)
ifndef MOSROOT
  MOSROOT = /srv/mans_os
endif

# Include the main makefile
include ${MOSROOT}/mos/make/Makefile
