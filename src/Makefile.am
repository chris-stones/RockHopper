
bin_PROGRAMS = update_popup
update_popup_SOURCES = \
  Popup.cpp \
  Libs/IoCC/IoCC.cpp

if platform_GNULinux
  update_popup_SOURCES += Platforms/GNULinux/PlatformBootstrapper.cpp
endif

if platform_RaspberryPi
  update_popup_SOURCES += Platforms/RaspberryPi/PlatformBootstrapper.cpp
endif

if platform_Mali400
  update_popup_SOURCES += Platforms/Mali400/PlatformBootstrapper.cpp
endif
