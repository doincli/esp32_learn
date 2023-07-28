idf_component_register(SRCS "ebyte.c"
                            "ebyte/ebyte_port.c"
                            "ebyte/ebyte_core.c"
                            "ebyte/ebyte_callback.c"
                            "ebyte/ebyte_e220x.c"
                    INCLUDE_DIRS "."
                                 "ebyte/include"
                    PRIV_REQUIRES driver)
