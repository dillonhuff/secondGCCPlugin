HOST_GCC=g++
TARGET_GCC=g++
PLUGIN_SOURCE_FILES= plugin.cpp div-zero-check.cpp
GCCPLUGINS_DIR:= $(shell $(TARGET_GCC) -print-file-name=plugin)
CXXFLAGS+= -I$(GCCPLUGINS_DIR)/include -fPIC -fno-rtti -O2     

plugin.so: $(PLUGIN_SOURCE_FILES)
	$(HOST_GCC) -shared $(CXXFLAGS) $^ -o $@

clean:
	find . -type f -name '*.so' -delete
	find . -type f -name '*.o' -delete
