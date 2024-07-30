################################################################################
#
# python-ffmpeg-python
#
################################################################################

PYTHON_FFMPEG_VERSION = 0.2.0
PYTHON_FFMPEG_SOURCE = ffmpeg-python-$(PYTHON_FFMPEG_VERSION).tar.gz
PYTHON_FFMPEG_SITE = https://github.com/kkroening/ffmpeg-python/archive/refs/tags/$(PYTHON_FFMPEG_VERSION).tar.gz
PYTHON_FFMPEG_LICENSE = Apache-2.0
PYTHON_FFMPEG_LICENSE_FILES = LICENSE

PYTHON_FFMPEG_SETUP_TYPE = setuptools
PYTHON_FFMPEG_DEPENDENCIES = ffmpeg python-future
HOST_PYTHON_FFMPEG_DEPENDENCIES = host-python3
PYTHON_FFMPEG_INSTALL_STAGING = NO

define PYTHON_FFMPEG_EXTRACT_CMDS
$(call python-package-extract,$(PYTHON_FFMPEG_SOURCE))
endef

define PYTHON_FFMPEG_BUILD_CMDS
cd $(@D) && $(HOST_PYTHON3) setup.py build
endef

define PYTHON_FFMPEG_INSTALL_TARGET_CMDS
cd $(@D) && $(HOST_DIR)/bin/python3 -m pip install . --prefix=/usr --root=$(TARGET_DIR) --no-deps --use-pep517
endef

$(eval $(python-package))

