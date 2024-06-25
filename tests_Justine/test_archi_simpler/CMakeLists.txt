# SPDX-License-Identifier: Apache-2.0

list(APPEND ZEPHYR_EXTRA_MODULES
  ${CMAKE_CURRENT_SOURCE_DIR}
  )

cmake_minimum_required(VERSION 3.20.0)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(test_archi_onoff)


target_sources(app PRIVATE src/main.c)
target_sources(app PRIVATE src/blinkLed.c)
target_sources(app PRIVATE src/blowCaptor.c)
target_sources(app PRIVATE src/buttonStatus.c)
target_sources(app PRIVATE src/combinationToMidi.c)
target_sources(app PRIVATE src/bluetoothThread.c)
target_sources(app PRIVATE src/playSingleNote.c)
target_sources(app PRIVATE src/mainCycle.c)
