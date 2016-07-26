include prorab.mk

this_name := bydlogine_test


include $(prorab_this_dir)sources.mk


this_cxxflags := -Wall
this_cxxflags += -Wno-comment #no warnings on nested comments
this_cxxflags += -Wno-format
this_cxxflags += -Werror
this_cxxflags += -DDEBUG
this_cxxflags += -fstrict-aliasing #strict aliasing!!!
this_cxxflags += -g
this_cxxflags += -std=c++11

this_cxxflags += -D_GLIBCXX_USE_CXX11_ABI=0 #use old ABI

ifeq ($(debug), true)
    this_cxxflags += -DDEBUG
endif


this_libassimp_a_path := $(prorab_this_dir)3rdParty/assimp/libassimp.a


this_cxxflags += -I$(prorab_this_dir)3rdParty/assimp/include
this_ldlibs += $(this_libassimp_a_path)

ifeq ($(prorab_os),windows)
    this_ldlibs += -lmingw32 #these should go first, otherwise linker will complain about undefined reference to WinMain
    this_ldflags += -L/usr/lib -L/usr/local/lib
    this_ldlibs +=  -lglew32 -lopengl32 -lpng -ljpeg -lz -lfreetype -mwindows

    this_cxxflags += -I/usr/include -I/usr/local/include

    #WORKAROUND for MinGW bug:
    this_cxxflags += -D__STDC_FORMAT_MACROS
else ifeq ($(prorab_os),macosx)
    this_ldlibs += -lGLEW -framework OpenGL -framework Cocoa -lpng -ljpeg -lfreetype -lz
else ifeq ($(prorab_os),linux)
    this_ldlibs += -lGLEW -pthread -lGL -lz
endif

this_ldlibs += -lmorda -lpapki -lnitki -lstob

this_ldflags += -rdynamic

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
