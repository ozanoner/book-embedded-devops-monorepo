idf_component_register(SRCS "button-app.c"
                    INCLUDE_DIRS ".")
target_add_binary_data(${COMPONENT_TARGET} "insights_auth_key.txt" TEXT)