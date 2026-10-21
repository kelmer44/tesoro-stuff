MODULE := engines/tesoro
MODULE_OBJS = resources.o script.o tesoro.o metaengine.o
ifeq ($(ENABLE_TESORO), DYNAMIC_PLUGIN)
PLUGIN := 1
endif
include $(srcdir)/rules.mk
DETECT_OBJS += $(MODULE)/detection.o
