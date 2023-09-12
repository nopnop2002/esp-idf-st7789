set(srcs "main.c"
    "st7789.c"
    "fontx.c"
    "decode_png.c"
    "decode_jpeg_v5.c"
    "pngle.c"
    )

idf_component_register(SRCS ${srcs} INCLUDE_DIRS "include" REQUIRES "driver" "spiffs" "vfs")
