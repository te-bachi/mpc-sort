
CC       = gcc
AR       = ar
CFLAGS   = -I$(TOPLEVEL)/include -D_REENTRANT -Wall -std=gnu99 -ggdb
LDFLAGS  = -lpthread -lrt
OBJECT   = $(SOURCES:%.c=%.o)

all_sub:
	if test "$(SUBDIRS)" != ""; then \
		for subdir in $(SUBDIRS); do \
			(cd $$subdir; make all); \
		done; \
	fi;

clean: clean_sub
	rm -f *.o *.a $(CLEANFILES)

clean_sub:
	if test "$(SUBDIRS)" != ""; then \
		for subdir in $(SUBDIRS); do \
			(cd $$subdir; make clean); \
		done; \
	fi;

$(LIBRARY): $(OBJECT)
	$(AR) rcu $(LIBRARY) $(OBJECT)
#	$(AR) rcs $(LIBRARY) $(OBJECT)

%.o: $(TOPLEVEL)/$(CURRENT)/%.c
	$(CC) -c $(CFLAGS) $<

.SILENT: all_sub clean_sub

