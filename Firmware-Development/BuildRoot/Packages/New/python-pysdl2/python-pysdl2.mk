################################################################################
#
# python-pysdl2
#
################################################################################

PYTHON_PYSDL2_VERSION = 0.9.16
PYTHON_PYSDL2_SOURCE = pysdl2-$(PYTHON_PYSDL2_VERSION).tar.gz
PYTHON_PYSDL2_SITE = https://github.com/py-sdl/py-sdl2/releases/download/$(PYTHON_PYSDL2_VERSION)
PYTHON_PYSDL2_LICENSE = zlib
PYTHON_PYSDL2_LICENSE_FILES = LICENSE.txt

PYTHON_PYSDL2_SETUP_TYPE = setuptools
PYTHON_PYSDL2_DEPENDENCIES = sdl2
HOST_PYTHON_PYSDL2_DEPENDENCIES = host-python3
PYTHON_PYSDL2_INSTALL_STAGING = NO

$(eval $(python-package))
$(eval $(host-python-package))

