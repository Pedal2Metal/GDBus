CMAKE_MINIMUM_REQUIRED (VERSION 2.8.10)

EXECUTE_PROCESS(COMMAND gdbus-codegen --generate-c-code ${PROJECT_SOURCE_DIR}/gdbus/gdbusdemo_gen ${PROJECT_SOURCE_DIR}/gdbus/Interface.xml)  