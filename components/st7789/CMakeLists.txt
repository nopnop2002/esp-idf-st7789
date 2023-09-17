set(srcs "st7789.c" "fontx.c")

idf_component_register(SRCS "${srcs}"
                       PRIV_REQUIRES driver
                       INCLUDE_DIRS ".")
