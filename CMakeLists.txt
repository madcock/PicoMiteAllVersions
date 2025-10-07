
cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(pico_sdk_import.cmake)

project(PicoMite C CXX ASM)

pico_sdk_init()

add_executable(PicoMite
  PicoMite.c
  regex.c
  MMBasic.c
  Operators.c
  Custom.c
  Functions.c
  Commands.c
  FileIO.c
  ff.c
  ffsystem.c
  ffunicode.c
  mmc_stm32.c
  Draw.c
  Editor.c
  XModem.c
  MM_Misc.c
  External.c
  MATHS.c
  Onewire.c
  I2C.c
  SPI.c
  Serial.c
  SPI-LCD.c
  BmpDecoder.c
  GPS.c
  Audio.c
  CFunction.c
  picojpeg.c
  lfs.c
  lfs_util.c
  hxcmod.c
  VS1053.c
  aes.c
  Memory.c
  flash.c
  SSD1963.c
  Touch.c
  GUI.c
  Keyboard.c
  mouse.c
  picocalc/i2ckbd.c
)

# RP2350 specific configuration

if (PICO_PLATFORM MATCHES "^rp2350")
  message(NOTICE "Building for RP2350")

  target_sources(PicoMite PRIVATE
    psram.c
    upng.c
  )

  pico_set_float_implementation(PicoMite pico_dcp)

  target_compile_options(PicoMite PRIVATE
    -Drp2350
    -DPICO_FLASH_SPI_CLKDIV=4
    -DPICO_PIO_USE_GPIO_BASE
  )

  target_link_libraries(PicoMite
    pico_rand
  )
endif(PICO_PLATFORM MATCHES "^rp2350")

# WebMite specific configuration

if (PICO_CYW43_SUPPORTED EQUAL 1)
  message(NOTICE "Building WebMite")
  target_sources(PicoMite PRIVATE
    cJSON.c
    mqtt.c
    MMMqtt.c
    MMTCPclient.c
    MMtelnet.c
    MMntp.c
    MMtcpserver.c
    tftp.c
    MMtftp.c
    MMudp.c
  )
  set_source_files_properties(cJSON.c PROPERTIES COMPILE_FLAGS -Os)
  target_include_directories(PicoMite PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/.. # for our common lwipopts
  )

  target_compile_options(PicoMite PRIVATE
    -DPICOMITEWEB
    -DGUICONTROLS
    -DPICO_HEAP_SIZE=0x3000
    -DCYW43_HOST_NAME="WebMite"
    -DPICO_CYW43_ARCH_POLL
    -DPICO_CORE0_STACK_SIZE=0x4000
  )

  target_link_libraries(PicoMite
    pico_cyw43_arch_lwip_poll
  )

  set_target_properties(PicoMite PROPERTIES OUTPUT_NAME "WebMite_${PICO_BOARD}")

else(PICO_CYW43_SUPPORTED EQUAL 1)

  message(NOTICE "Building PicoMite")

  target_compile_options(PicoMite PRIVATE
    -DPICOMITE
    -DGUICONTROLS
    -DPICO_HEAP_SIZE=0x1000
    -DPICO_CORE0_STACK_SIZE=0x1000
    -DPICO_CORE1_STACK_SIZE=0x00
  )

  set_target_properties(PicoMite PROPERTIES OUTPUT_NAME "PicoMite_${PICO_BOARD}")

  target_link_libraries(PicoMite
    pico_multicore
  )

endif(PICO_CYW43_SUPPORTED EQUAL 1)

set_source_files_properties(mmc_stm32.c PROPERTIES COMPILE_FLAGS -O2)
set_source_files_properties(ff.c PROPERTIES COMPILE_FLAGS -Os)
set_source_files_properties(GUI.c PROPERTIES COMPILE_FLAGS -Os)
set_source_files_properties(BmpDecoder.c PROPERTIES COMPILE_FLAGS -Os)
set_source_files_properties(GPS.c PROPERTIES COMPILE_FLAGS -Os)
set_source_files_properties(I2C.c PROPERTIES COMPILE_FLAGS -Os)
set_source_files_properties(lfs.c PROPERTIES COMPILE_FLAGS -Os)
set_source_files_properties(picojpeg.c PROPERTIES COMPILE_FLAGS -Os)
set_source_files_properties(regex.c PROPERTIES COMPILE_FLAGS -Os)
set_source_files_properties(hxcmod.c PROPERTIES COMPILE_FLAGS -Os)
set_source_files_properties(MATHS.c PROPERTIES COMPILE_FLAGS -Os)
set_source_files_properties(Editor.c PROPERTIES COMPILE_FLAGS -Os)
set_source_files_properties(aes.c PROPERTIES COMPILE_FLAGS -Os)
pico_generate_pio_header(PicoMite ${CMAKE_CURRENT_LIST_DIR}/PicoMiteI2S.pio)

pico_enable_stdio_usb(PicoMite 1)
pico_enable_stdio_uart(PicoMite 0)
pico_set_printf_implementation(PicoMite compiler)

target_compile_options(PicoMite PRIVATE
  -DNDEBUG
  -DPICO_STDIO_USB_ENABLE_RESET_VIA_VENDOR_INTERFACE=0
  -DPICO_ADC_CLKDIV_ROUND_NEAREST
  -DPICO_XOSC_STARTUP_DELAY_MULTIPLIER=64
  -DPICO_CLOCK_AJDUST_PERI_CLOCK_WITH_SYS_CLOCK
  -DPICO_XOSC_STARTUP_DELAY_MULTIPLIER=64
  -DPICO_MALLOC_PANIC
  -O2
  -Wall
  -DPICOCALC
)

target_link_libraries(PicoMite
  pico_stdlib
  pico_unique_id
  hardware_flash
  hardware_irq
  hardware_adc
  hardware_pwm
  hardware_i2c
  hardware_spi
  hardware_dma
  hardware_exception
  hardware_pio
)

pico_add_extra_outputs(PicoMite)

target_link_options(PicoMite PRIVATE -Wl,--print-memory-usage)
