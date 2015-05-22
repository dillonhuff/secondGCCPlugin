HOST_GCC=g++
TARGET_GCC=g++
PLUGIN_SOURCE_FILES= $(shell find ./ -type f -name '*.cpp')
PLUGIN_HEADERS = $(shell find ./ -type f -name '*.h')
PLUGIN_OBJS = $(patsubst %.cpp, %.o, $(PLUGIN_SOURCE_FILES))
GCCPLUGINS_DIR:= $(shell $(TARGET_GCC) -print-file-name=plugin)
CXXFLAGS+= -I$(GCCPLUGINS_DIR)/include -fPIC -fno-rtti -O2     

plugin.so: $(PLUGIN_OBJS) $(PLUGIN_HEADERS)
	$(HOST_GCC) -shared $(CXXFLAGS) $^ -o $@

clean:
	find . -type f -name '*.so' -delete
	find . -type f -name '*.o' -delete
