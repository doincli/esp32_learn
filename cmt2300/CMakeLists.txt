idf_build_get_property(enable DEV_CMT2300_ENABLE)
if(NOT enable)
    return()
endif()

idf_component_register(SRCS "cmt2300_regs.c"
                            "cmt2300_spi_sw.c"
                            "cmt2300_spi_hw.c"
                            "rfm300_driver.c"
                        INCLUDE_DIRS . include
                        REQUIRES driver
                        )