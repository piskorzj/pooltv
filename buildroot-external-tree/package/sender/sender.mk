################################################################################
#
# sender
#
################################################################################

SENDER_VERSION = 1.0
SENDER_SITE = $(SENDER_PKGDIR)/../../../sender
SENDER_SITE_METHOD = local
SENDER_DEPENDENCIES = host-pkgconf systemd ptv_fmt

$(eval $(cmake-package))

