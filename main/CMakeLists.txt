set(srcs "main.c"
    "decode_png.c"
    "decode_jpeg_v5.c"
    "pngle.c"
    )

idf_component_register(SRCS ${srcs} INCLUDE_DIRS ".")
