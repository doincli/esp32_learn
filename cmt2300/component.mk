#
# "main" pseudo-component makefile.
#
# (Uses default behaviour of compiling all source files in directory, adding 'include' to include path.)

#ifdef CONFIG_IOT_LED_ENABLE
COMPONENT_ADD_INCLUDEDIRS := ./status_led/include
COMPONENT_SRCDIRS := ./status_led
#else
# Disable component
#COMPONENT_ADD_INCLUDEDIRS :=
#COMPONENT_ADD_LDFLAGS :=
#COMPONENT_SRCDIRS :=
#endif