# A CMake script to find SetEnv.cmd.

find_program(
  LONGLP_WINSDK_SETENV
  NAMES SetEnv.cmd
  PATHS
    "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Microsoft SDKs\\Windows;CurrentInstallFolder]/bin"
)
if(LONGLP_WINSDK_SETENV AND PRINT_PATH)
  execute_process(COMMAND ${CMAKE_COMMAND} -E echo "${LONGLP_WINSDK_SETENV}")
endif()
