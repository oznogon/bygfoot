## Process this file with automake to produce Makefile.in

AM_CFLAGS = -Wall

SUBDIRS = src po

EXTRA_DIST = \
	autogen.sh \
	bygfoot.glade \
	bygfoot.gladep

install-data-local:
	@$(NORMAL_INSTALL)
	if test -d $(srcdir)/support_files; then \
	  $(mkinstalldirs) $(DESTDIR)$(pkgdatadir)/support_files; \
	  for pixmap in $(srcdir)/support_files/*; do \
	    if test -f $$pixmap; then \
		if test -x $$pixmap; then \
		      $(INSTALL_PROGRAM) $$pixmap $(DESTDIR)$(pkgdatadir)/support_files; \
		else \
		      $(INSTALL_DATA) $$pixmap $(DESTDIR)$(pkgdatadir)/support_files; \
		fi \
	    fi \
	  done \
	fi
	@$(NORMAL_INSTALL)
	if test -d $(srcdir)/support_files/text_files; then \
	  $(mkinstalldirs) $(DESTDIR)$(pkgdatadir)/support_files/text_files; \
	  for pixmap in $(srcdir)/support_files/text_files/*; do \
	    if test -f $$pixmap; then \
		if test -x $$pixmap; then \
		      $(INSTALL_PROGRAM) $$pixmap $(DESTDIR)$(pkgdatadir)/support_files/text_files; \
		else \
		      $(INSTALL_DATA) $$pixmap $(DESTDIR)$(pkgdatadir)/support_files/text_files; \
		fi \
	    fi \
	  done \
	fi

dist-hook:
	if test -d support_files; then \
	  mkdir $(distdir)/support_files; \
	  for pixmap in support_files/*; do \
	    if test -f $$pixmap; then \
	      cp -p $$pixmap $(distdir)/support_files; \
	    fi \
	  done \
	fi
	if test -d support_files/text_files; then \
	  mkdir $(distdir)/support_files/text_files; \
	  for pixmap in support_files/text_files/*; do \
	    if test -f $$pixmap; then \
	      cp -p $$pixmap $(distdir)/support_files/text_files; \
	    fi \
	  done \
	fi

uninstall:
	rm -rf $(DESTDIR)$(pkgdatadir)
	rm -rf $(prefix)/bin/bygfoot
	rm -rf $(prefix)/man/man6/bygfoot.6
