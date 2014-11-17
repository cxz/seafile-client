CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

PROJECT(breakpad CXX)

INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})

IF(WIN32)
    SET(breakpad_sources
        client/windows/handler/exception_handler.cc
        common/windows/string_utils.cc
        common/windows/guid_string.cc
        client/windows/crash_generation/crash_generation_client.cc
    )
ELSEIF (${CMAKE_SYSTEM_NAME} MATCHES "Linux" OR ${CMAKE_SYSTEM_NAME} MATCHES "BSD")
    SET(breakpad_sources
        client/linux/crash_generation/crash_generation_client.cc
        client/linux/handler/exception_handler.cc
        client/linux/handler/minidump_descriptor.cc
        client/linux/minidump_writer/minidump_writer.cc
        client/linux/minidump_writer/linux_dumper.cc
        client/linux/minidump_writer/linux_ptrace_dumper.cc
        client/linux/microdump_writer/microdump_writer.cc
        client/linux/dump_writer_common/ucontext_reader.cc
        client/linux/dump_writer_common/thread_info.cc
        client/linux/dump_writer_common/seccomp_unwinder.cc
        client/linux/log/log.cc
        client/minidump_file_writer.cc
        common/linux/linux_libc_support.cc
        common/linux/file_id.cc
        common/linux/memory_mapped_file.cc
        common/linux/safe_readlink.cc
        common/linux/guid_creator.cc
        common/linux/elfutils.cc
        common/string_conversion.cc
        common/convert_UTF.c
    )
ELSEIF (APPLE)
    INCLUDE_DIRECTORIES(common/mac)

    SET(breakpad_sources
        client/mac/crash_generation/crash_generation_client.cc
        client/mac/handler/breakpad_nlist_64.cc
        client/mac/handler/dynamic_images.cc
        client/mac/handler/exception_handler.cc
        client/mac/handler/minidump_generator.cc
        client/minidump_file_writer.cc
        common/mac/bootstrap_compat.cc
        common/mac/file_id.cc
        common/mac/launch_reporter.cc
        common/mac/macho_id.cc
        common/mac/macho_utilities.cc
        common/mac/macho_walker.cc
        common/mac/string_utilities.cc
        common/mac/MachIPC.mm
        common/mac/GTMLogger.m
        common/md5.cc
        common/string_conversion.cc
        common/convert_UTF.c
    )
ENDIF()

ADD_LIBRARY(breakpad_s STATIC ${breakpad_sources})
TARGET_LINK_LIBRARIES(breakpad_s -lpthread)
