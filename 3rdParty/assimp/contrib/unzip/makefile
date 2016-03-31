include prorab.mk


this_srcs += unzip.c
this_srcs += ioapi.c

this_srcs := $(addprefix $(prorab_this_dir), $(this_srcs))

this_cflags += -Wall -O2 -fPIC
this_cflags += -DASSIMP_BUILD_NO_OWN_ZLIB

all: $(prorab_this_dir)libunzip.a

$(prorab_this_dir)libunzip.a: $(patsubst %.c,%.o,$(this_srcs))
	$(prorab_echo)ar rcs $@ $^

$(prorab_this_dir)%.o: $(prorab_this_dir)%.c
	$(prorab_echo)$(CC) -x c -c -ansi $(this_cflags) $? -o $@

clean::
	$(prorab_echo)rm *.o
	$(prorab_echo)rm libunzip.a