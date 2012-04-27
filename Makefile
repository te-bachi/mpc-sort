
TOPLEVEL = .
SUBDIRS = \
    obj

CLEANFILES = sort05

all: all_sub sort05

include $(TOPLEVEL)/Makefile.inc

STATIC_LIBRARY = obj/lib.a

sort05: $(STATIC_LIBRARY)
	$(CC) -o $@ obj/main.o $(STATIC_LIBRARY) $(LDFLAGS)

