SUBDIRS := src

-include def.mk

all: $(SUBDIRS)

.PHONY: $(SUBDIRS)

$(SUBDIRS):
	make -C $@ -f ../$(STACK)Makefile STACK=../$(STACK)
