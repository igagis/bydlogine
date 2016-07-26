include prorab.mk

this_name := assimp

this_soname := 0

this_srcs += $(wildcard src/*.cpp)
this_srcs += $(wildcard contrib/clipper/*.cpp)
this_srcs += $(wildcard contrib/ConvertUTF/*.cpp)
this_srcs += $(wildcard contrib/irrXML/*.cpp)
this_srcs += $(wildcard contrib/poly2tri/poly2tri/common/*.cpp)
this_srcs += $(wildcard contrib/poly2tri/poly2tri/sweep/*.cpp)


#this_cxxflags += -Wall
#this_cxxflags += -Wno-comment #no warnings on nested comments
#this_cxxflags += -Wno-format
#this_cxxflags += -DDEBUG
#this_cxxflags += -fstrict-aliasing #strict aliasing!!!
this_cxxflags += -g
#this_cxxflags += -std=c++11
this_cxxflags += -fPIC

this_cxxflags += -DASSIMP_BUILD_BOOST_WORKAROUND #we don't use boost
this_cxxflags += -DASSIMP_BUILD_NO_OWN_ZLIB #use shared zlib library
this_cxxflags += -Isrc/BoostWorkaround/


this_libunzip_a_path := $(prorab_this_dir)contrib/unzip/libunzip.a


this_ldflags += $(this_libunzip_a_path)

#ifeq ($(debug), true)
#    this_cxxflags += -DDEBUG
#endif

#ifeq ($(prorab_os),windows)
#    this_ldlibs += -lmingw32 #these should go first, otherwise linker will complain about undefined reference to WinMain
#    this_ldflags += -L/usr/lib -L/usr/local/lib
#    this_ldlibs +=  -lglew32 -lopengl32 -lting -lstob -lmorda -lpng -ljpeg -lz -lfreetype -mwindows
#
#    this_cxxflags += -I/usr/include -I/usr/local/include
#
#    #WORKAROUND for MinGW bug:
#    this_cxxflags += -D__STDC_FORMAT_MACROS
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
