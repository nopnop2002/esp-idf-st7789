set(srcs "main.c"
    "st7789.c"
    "fontx.c"
    "pngle.c"
    )

# tjpgd library does not exist in ESP32-S2 ROM.
if(IDF_TARGET STREQUAL "esp32")
    list(APPEND srcs "decode_image.c")
endif()

idf_component_register(SRCS ${srcs}
                    INCLUDE_DIRS ".")
