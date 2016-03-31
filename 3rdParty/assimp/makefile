include prorab.mk

this_name := assimp

this_soname := 0

this_srcs += $(wildcard src/*.cpp)
this_srcs += $(wildcard contrib/clipper/*.cpp)
this_srcs += $(wildcard contrib/ConvertUTF/*.cpp)
this_srcs += $(wildcard contrib/irrXML/*.cpp)
this_srcs += $(wildcard contrib/poly2tri/poly2tri/common/*.cpp)
this_srcs += $(wildcard contrib/poly2tri/poly2tri/sweep/*.cpp)


#this_cflags += -Wall
#this_cflags += -Wno-comment #no warnings on nested comments
#this_cflags += -Wno-format
#this_cflags += -DDEBUG
#this_cflags += -fstrict-aliasing #strict aliasing!!!
this_cflags += -g
#this_cflags += -std=c++11
this_cflags += -fPIC

this_cflags += -DASSIMP_BUILD_BOOST_WORKAROUND #we don't use boost
this_cflags += -DASSIMP_BUILD_NO_OWN_ZLIB #use shared zlib library
this_cflags += -Isrc/BoostWorkaround/


this_libunzip_a_path := $(prorab_this_dir)contrib/unzip/libunzip.a


this_ldflags += $(this_libunzip_a_path)

#ifeq ($(debug), true)
#    this_cflags += -DDEBUG
#endif

#ifeq ($(prorab_os),windows)
#    this_ldlibs += -lmingw32 #these should go first, otherwise linker will complain about undefined reference to WinMain
#    this_ldflags += -L/usr/lib -L/usr/local/lib
#    this_ldlibs +=  -lglew32 -lopengl32 -lting -lstob -lmorda -lpng -ljpeg -lz -lfreetype -mwindows
#
#    this_cflags += -I/usr/include -I/usr/local/include
#
#    #WORKAROUND for MinGW bug:
#    this_cflags += -D__STDC_FORMAT_MACROS
#else ifeq ($(prorab_os),macosx)
#    this_ldlibs += -lGLEW -framework OpenGL -framework Cocoa -lstob -lting -lmorda -lpng -ljpeg -lfreetype
#else ifeq ($(prorab_os),linux)
#    this_ldlibs += -lting -lmorda -lGLEW -lstob -pthread -lGL
#endif



$(eval $(prorab-build-lib))

#add dependency on libunzip.a
$(prorab_this_name): $(this_libunzip_a_path)

#rule to build libunzip.a
$(this_libunzip_a_path):
	$(prorab_echo)(cd $(dir $@); $(MAKE))
