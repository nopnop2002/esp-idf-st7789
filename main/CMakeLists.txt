set(COMPONENT_SRCS main.c st7789.c fontx.c decode_image.c)
set(COMPONENT_ADD_INCLUDEDIRS ".")

register_component()
spiffs_create_partition_image(storage ../font FLASH_IN_PROJECT)
