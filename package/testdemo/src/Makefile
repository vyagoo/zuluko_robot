Target = testdemo
## set the include path for compile flags.
SourceIncludePath := -I$(STAGING_DIR)/usr/include/allwinner/include -I$(STAGING_DIR)/usr/include/allwinner
CompileFlags = $(CFLAGS) $(SourceIncludePath)

LoadFlags += -lstandby -lpthread -ldl -lstdc++ -lrt -lutils -lcutils -lmediaserver -lwifiswitch

$(Target): testdemo.c CameraHardware.c iio_utils.c water_mark.c
	$(CC) -o $@ $^ $(CompileFlags) $(LDFLAGS) $(LoadFlags)
