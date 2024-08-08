# Paths
SYSROOT_PATH=/home/ash/Downloads/RK3568/MediaPlayer/BuildRoot/proj2/new/rk356x_sdk-linux5.10/buildroot/output/rockchip_rk3568/host/aarch64-buildroot-linux-gnu/sysroot
CROSS_COMPILE_PATH=/home/ash/Downloads/RK3568/MediaPlayer/BuildRoot/proj2/new/rk356x_sdk-linux5.10/buildroot/output/rockchip_rk3568/host/bin
CROSS_COMPILE=aarch64-buildroot-linux-gnu-

# Compiler and flags
CC=$(CROSS_COMPILE_PATH)/$(CROSS_COMPILE)gcc
CXX=$(CROSS_COMPILE_PATH)/$(CROSS_COMPILE)g++
CXXFLAGS=-I$(SYSROOT_PATH)/usr/include/gstreamer-1.0 \
         -I$(SYSROOT_PATH)/usr/include/gst-plugins-base-1.0 \
         -I$(SYSROOT_PATH)/usr/include/glib-2.0 \
         -I$(SYSROOT_PATH)/usr/lib/glib-2.0/include \
         -std=c++20
LDFLAGS=-L$(SYSROOT_PATH)/usr/lib \
        -lgstreamer-1.0 \
        -lgstapp-1.0 \
        -lgstbase-1.0 \
        -lgobject-2.0 \
        -lglib-2.0 \
        -lX11

# Source and target
SRC_FILES=$(wildcard *.cpp)
OBJ_FILES=$(SRC_FILES:.cpp=.o)
TARGET=media_player

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ_FILES)
