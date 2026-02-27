set(srcs "st7789.c" "fontx.c")

idf_component_register(
	SRCS "${srcs}"
	PRIV_REQUIRES driver esp_driver_spi esp_driver_gpio
	INCLUDE_DIRS "."
)
