# npsimple version
VERSION = 0.3

# Customize below to fit your system

ifeq (${shell uname}, Darwin)
CPPFLAGS = -DVERSION=\"${VERSION}\" -DWEBKIT_DARWIN_SDK
LDFLAGS = -dynamiclib #-framework Carbon -framework CoreFoundation -framework WebKit
else
ifeq (${shell pkg-config npapi-sdk && echo ok}, ok)
CPPFLAGS = -DVERSION=\"${VERSION}\" -DNPAPI_SDK `pkg-config --cflags npapi-sdk`
else
INCS = -I/home/anselm/aplix/code/third_party/xulrunner-sdk # apt-get install xulrunner-dev
CPPFLAGS = -DVERSION=\"${VERSION}\" -DXULRUNNER_SDK
#LDFLAGS = -L/usr/lib -lc
endif
endif
CFLAGS = -g -pedantic -Wall -O2 ${INCS} ${CPPFLAGS}

# compiler and linker
CC = cc
