ifneq ($(TARGET_SIMULATOR),true)

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_SRC_FILES := \
        npsimple.c

LOCAL_C_INCLUDES += \
	${MOZILLA_HOME}/modules/plugin/base/public

LOCAL_SHARED_LIBRARIES :=
LOCAL_CPPFLAGS := -DANDROID -D_GNU_SOURCE
LOCAL_LDFLAGS :=
LOCAL_LDLIBS :=
LOCAL_MODULE := npsimple

include $(BUILD_SHARED_LIBRARY)

endif
