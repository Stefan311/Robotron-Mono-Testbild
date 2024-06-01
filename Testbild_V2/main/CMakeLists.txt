idf_component_register(SRCS "A7100.c" "EC1834.c" "PC1715.c" "main.c"
                    INCLUDE_DIRS ".")
set_target_properties(${COMPONENT_LIB} PROPERTIES COMPILE_FLAGS "-save-temps=obj")