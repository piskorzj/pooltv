################################################################################
#
# ptv_fmt
#
################################################################################

PTV_FMT_VERSION = 5.0.0
PTV_FMT_SITE = $(call github,fmtlib,fmt,$(PTV_FMT_VERSION))
PTV_FMT_LICENSE = BSD-2-Clause
PTV_FMT_LICENSE_FILES = LICENSE.rst
PTV_FMT_INSTALL_STAGING = YES

PTV_FMT_CONF_OPTS = \
	-DHAVE_OPEN=ON \
	-DFMT_INSTALL=ON \
	-DFMT_TEST=OFF

$(eval $(cmake-package))
