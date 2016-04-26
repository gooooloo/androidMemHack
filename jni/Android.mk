LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := androidMemHack
LOCAL_SRC_FILES := main.cpp
LOCAL_CPPFLAGS := -std=gnu++0x -Wall         # whatever g++ flags you like
LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog   # whatever ld flags you like
LOCAL_CFLAGS += -pie -fPIE
LOCAL_CPPFLAGS += -pie -fPIE
LOCAL_LDFLAGS += -pie -fPIE

include $(BUILD_EXECUTABLE)    # <-- Use this to build an executable.
