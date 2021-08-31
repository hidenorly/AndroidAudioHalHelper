# compiler env.
UNAME := $(shell uname -s)
ifeq ($(UNAME),Linux)
	CXX=ccache clang++
	LDLIBS=-ldl -lasound
	CXXFLAGS=-std=c++2a -MMD -MP -Wall -fPIC
	SHARED_CXXFLAGS= -fPIC -shared
endif
ifeq ($(UNAME),Darwin)
	CXX=ccache clang++
	LDLIBS=-stdlib=libc++
	CXXFLAGS=-std=c++2a -MMD -MP -Wall
#	SHARED_CXXFLAGS= -flat_namespace -dynamiclib
	SHARED_CXXFLAGS=-dynamiclib
endif

LDFLAGS=-pthread

# project config
HELPER_DIR ?= ./src
AFW_DIR=../audioframework
INC_DIR=$(AFW_DIR)/include
LIB_DIR=$(AFW_DIR)/lib
LIB_HELPER_DIR=./lib
OBJ_DIR=./out

ANDROID_HOME := $(shell printenv ANDROID_HOME)
ANDROID_MEDIA_INC=$(ANDROID_HOME)/system/media/audio/include
ANDROID_CUTIL_INC=$(ANDROID_HOME)/system/core/libcutils/include
ANDROID_FMQ_INC=$(ANDROID_HOME)/system/libfmq/include
ANDROID_UTILS_INC=$(ANDROID_HOME)/system/core/libutils/include
ANDROID_HIDL_INC=$(ANDROID_HOME)/system/libhidl/base/include
ANDROID_LOG_INC=$(ANDROID_HOME)/system/core/liblog/include

# --- source code config --------------
HELPER_SRCS = $(wildcard $(HELPER_DIR)/*.cpp)

# --- the object files config --------------
HELPER_OBJS = $(addprefix $(OBJ_DIR)/, $(notdir $(HELPER_SRCS:.cpp=.o)))

# --- Build for sink example(shared) ------------
UNAME := $(shell uname -s)
ifeq ($(UNAME),Linux)
	HELPER_SO_TARGET = $(LIB_HELPER_DIR)/libaudiohalhelper.so
	AFW_SO_TARGET = $(LIB_DIR)/libafw.so
endif
ifeq ($(UNAME),Darwin)
	HELPER_SO_TARGET = $(LIB_HELPER_DIR)/libaudiohalhelper.dylib
	AFW_SO_TARGET = $(LIB_DIR)/libafw.dylib
endif
HELPER_DEPS = $(HELPER_OBJS:.o=.d)

default: $(HELPER_SO_TARGET)
.PHONY: default

$(HELPER_SO_TARGET): $(HELPER_OBJS)
	@[ -d $(LIB_DIR) ] || mkdir -p $(LIB_DIR)
	@[ -d $(LIB_HELPER_DIR) ] || mkdir -p $(LIB_HELPER_DIR)
	$(CXX) $(LDFLAGS) $(SHARED_CXXFLAGS) $(HELPER_OBJS) -o $@ $(LDLIBS) $(AFW_SO_TARGET)

$(HELPER_OBJS): $(HELPER_SRCS)
	echo Android home is $(ANDROID_HOME)

	@[ -d $(OBJ_DIR) ] || mkdir -p $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -I $(INC_DIR) -I $(ANDROID_MEDIA_INC) -I $(ANDROID_CUTIL_INC) -I $(ANDROID_FMQ_INC) -I $(ANDROID_UTILS_INC) -I $(ANDROID_HIDL_INC) -I $(ANDROID_LOG_INC) -c $(HELPER_DIR)/$(notdir $(@:.o=.cpp)) -o $@

-include $(HELPER_DEPS)

# --- clean up ------------------------
clean:
	rm -f $(HELPER_OBJS) $(HELPER_SO_TARGET)
