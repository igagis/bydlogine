include prorab.mk

this_name := bydlogine_test


include $(prorab_this_dir)sources.mk


this_cflags := -Wall
this_cflags += -Wno-comment #no warnings on nested comments
this_cflags += -Wno-format
this_cflags += -Werror
this_cflags += -DDEBUG
this_cflags += -fstrict-aliasing #strict aliasing!!!
this_cflags += -g
this_cflags += -std=c++11

ifeq ($(debug), true)
    this_cflags += -DDEBUG
endif


this_libassimp_a_path := $(prorab_this_dir)3rdParty/assimp/libassimp.a


this_cflags += -I$(prorab_this_dir)3rdParty/assimp/include
this_ldlibs += $(this_libassimp_a_path)

ifeq ($(prorab_os),windows)
    this_ldlibs += -lmingw32 #these should go first, otherwise linker will complain about undefined reference to WinMain
    this_ldflags += -L/usr/lib -L/usr/local/lib
    this_ldlibs +=  -lglew32 -lopengl32 -lpng -ljpeg -lz -lfreetype -mwindows

    this_cflags += -I/usr/include -I/usr/local/include

    #WORKAROUND for MinGW bug:
    this_cflags += -D__STDC_FORMAT_MACROS
else ifeq ($(prorab_os),macosx)
    this_ldlibs += -lGLEW -framework OpenGL -framework Cocoa -lpng -ljpeg -lfreetype -lz
else ifeq ($(prorab_os),linux)
    this_ldlibs += -lGLEW -pthread -lGL -lz
endif

this_ldlibs += -lmorda -lpapki -lnitki -lstob

$(eval $(prorab-build-app))



define this_rules
test:: $(prorab_this_name)
	@echo running $$^...
	$(prorab_echo)(cd $(prorab_this_dir); $$^)
endef
$(eval $(this_rules))



#add dependency on libassimp.a:
$(prorab_this_name): $(this_libassimp_a_path)

#rule to make libassimp.a
$(this_libassimp_a_path):
	$(prorab_echo)(cd $(dir $@); $(MAKE))
