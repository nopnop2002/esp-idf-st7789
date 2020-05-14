set(COMPONENT_SRCS main.c st7789.c decode_image.c fontx.c pngle.c)
set(COMPONENT_ADD_INCLUDEDIRS ".")

register_component()
spiffs_create_partition_image(storage ../font FLASH_IN_PROJECT)
