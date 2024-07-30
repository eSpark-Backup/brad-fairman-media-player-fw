################################################################################
#
# python-future
#
################################################################################

PYTHON_FUTURE_VERSION = 1.0.0
PYTHON_FUTURE_SOURCE = future-$(PYTHON_FUTURE_VERSION).tar.gz
PYTHON_FUTURE_SITE = https://files.pythonhosted.org/packages/a7/b2/4140c69c6a66432916b26158687e821ba631a4c9273c474343badf84d3ba
PYTHON_FUTURE_SETUP_TYPE = setuptools
PYTHON_FUTURE_LICENSE = MIT
PYTHON_FUTURE_LICENSE_FILES = LICENSE.txt

define PYTHON_FUTURE_EXTRACT_CMDS
$(call python-package-extract,$(PYTHON_FFMPEG_SOURCE))
endef

define PYTHON_FUTURE_BUILD_CMDS
cd $(@D) && $(HOST_PYTHON3) setup.py build
endef

define PYTHON_FUTURE_INSTALL_TARGET_CMDS
cd $(@D) && $(HOST_DIR)/bin/python3 -m pip install . --prefix=/usr --root=$(TARGET_DIR) --no-deps --use-pep517
endef

$(eval $(python-package))
