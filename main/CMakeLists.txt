set(srcs "main.c"
    "st7789.c"
    "fontx.c"
    "pngle.c"
    )

# Only ESP32 has enough memory to do jpeg decoding
if(IDF_TARGET STREQUAL "esp32")
    list(APPEND srcs "decode_image.c")
endif()

idf_component_register(SRCS ${srcs}
                    INCLUDE_DIRS ".")


# Create a SPIFFS image from the contents of the 'font' directory
# that fits the partition named 'storage'. FLASH_IN_PROJECT indicates that
# the generated image should be flashed when the entire project is flashed to
# the target with 'idf.py -p PORT flash
spiffs_create_partition_image(storage ../font FLASH_IN_PROJECT)
