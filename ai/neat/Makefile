SUBDIRS := tests src
SUBDIRSCLEAN = $(addsuffix clean,$(SUBDIRS))

-include def.mk

all: $(SUBDIRS)

.PHONY: $(SUBDIRS)

$(SUBDIRS):
	make -C $@ -f ../$(STACK)Makefile STACK=../$(STACK)

clean: $(SUBDIRSCLEAN)

clean_curdir:
	rm -rfv *.o *~ *.gcno *.gcda *.gcov main

%clean: %
	make -C $< -f ../$(STACK)Makefile clean_curdir
