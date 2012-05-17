# npsimple version
VERSION = 0.3

# Customize below to fit your system

ifeq (${shell uname}, Darwin)
CPPFLAGS = -DVERSION=\"${VERSION}\" -DUSE_SDK_WEBKIT_DARWIN
LDFLAGS = -dynamiclib #-framework Carbon -framework CoreFoundation -framework WebKit
else
ifeq (${shell pkg-config npapi-sdk && echo ok}, ok)
CPPFLAGS = -DVERSION=\"${VERSION}\" -DUSE_SDK_NPAPI_SDK `pkg-config --cflags npapi-sdk`
else
INCS = -I/home/anselm/aplix/code/third_party/xulrunner-sdk # apt-get install xulrunner-dev
CPPFLAGS = -DVERSION=\"${VERSION}\" -DUSE_SDK_XULRUNNER
#LDFLAGS = -L/usr/lib -lc
endif
endif
CFLAGS = -g -pedantic -Wall -O2 ${INCS} ${CPPFLAGS}

# compiler and linker
CC = cc
