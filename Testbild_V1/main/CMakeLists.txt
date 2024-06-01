idf_component_register(SRCS "main.c"
                    INCLUDE_DIRS "."
                    EMBED_FILES "samples/A7100.bin" "samples/PC1715.bin" "samples/EC1834.bin" )
set_target_properties(${COMPONENT_LIB} PROPERTIES COMPILE_FLAGS "-save-temps=obj")