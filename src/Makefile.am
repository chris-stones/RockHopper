
bin_PROGRAMS = RockHopper
RockHopper_SOURCES = \
  Main.cpp \
  Libs/IoCC/IoCC.cpp

if platform_GNULinux
  RockHopper_SOURCES += Platforms/GNULinux/PlatformBootstrapper.cpp
endif

if platform_RaspberryPi
  RockHopper_SOURCES += Platforms/RaspberryPi/PlatformBootstrapper.cpp
endif
