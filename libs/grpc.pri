win32-msvc* {
    VCPKG_PATH = E:/Qt/vcpkg

    INCLUDEPATH += \
        $${VCPKG_PATH}/installed/x64-windows/include

    CONFIG (debug, debug|release) {
        VCPKG_LIBS = $${VCPKG_PATH}/installed/x64-windows/debug/lib
        VCPKG_DLLS = $${VCPKG_PATH}/installed/x64-windows/debug/bin
        DEBUG_SUFFIX = d
    } else {
        VCPKG_LIBS = $${VCPKG_PATH}/installed/x64-windows/lib
        VCPKG_DLLS = $${VCPKG_PATH}/installed/x64-windows/bin
        DEBUG_SUFFIX =
    }

    LIBS += \
        -L$${VCPKG_LIBS} \
        -labseil_dll \
        -labsl_flags_commandlineflag \
        -labsl_flags_commandlineflag_internal \
        -labsl_flags_config \
        -labsl_flags_internal \
        -labsl_flags_marshalling \
        -labsl_flags_private_handle_accessor \
        -labsl_flags_program_name \
        -labsl_flags_reflection \
        -laddress_sorting \
        -lcares \
        -lgpr \
        -lgrpc \
        -lgrpc++ \
        -lgrpc++_reflection \
        -llibcrypto \
        -llibprotobuf$${DEBUG_SUFFIX} \
        -llibssl \
        -lre2 \
        -lupb_base_lib \
        -lupb_json_lib \
        -lupb_mem_lib \
        -lupb_message_lib \
        -lupb_mini_descriptor_lib \
        -lupb_textformat_lib \
        -lupb_wire_lib \
        -lutf8_range \
        -lzlib$${DEBUG_SUFFIX}
} else {
    VCPKG_PATH = /home/gris/Qt/vcpkg

    INCLUDEPATH += \
        $${VCPKG_PATH}/installed/x64-linux/include

    CONFIG (debug, debug|release) {
        VCPKG_LIBS = $${VCPKG_PATH}/installed/x64-linux/debug/lib
        DEBUG_SUFFIX = d
    } else {
        VCPKG_LIBS = $${VCPKG_PATH}/installed/x64-linux/lib
        DEBUG_SUFFIX =
    }

    LIBS += \
        -L$${VCPKG_LIBS} \
        -lgrpc++_reflection \
        -lgrpc++ \
        -lprotobuf$${DEBUG_SUFFIX} \
        -labsl_die_if_null \
        -labsl_log_initialize \
        -lutf8_validity \
        -lgrpc \
        -labsl_statusor \
        -lupb_json_lib \
        -lupb_textformat_lib \
        -lupb_mini_descriptor_lib \
        -lupb_wire_lib \
        -lutf8_range \
        -lupb_message_lib \
        -lupb_base_lib \
        -lupb_mem_lib \
        -lre2 \
        -lz \
        -lcares \
        -lgpr \
        -labsl_flags_internal \
        -labsl_flags_reflection \
        -labsl_raw_hash_set \
        -labsl_hashtablez_sampler \
        -labsl_flags_config \
        -labsl_flags_program_name \
        -labsl_flags_private_handle_accessor \
        -labsl_flags_commandlineflag \
        -labsl_flags_commandlineflag_internal \
        -labsl_random_distributions \
        -labsl_random_seed_sequences \
        -labsl_random_internal_pool_urbg \
        -labsl_random_internal_randen \
        -labsl_random_internal_randen_hwaes \
        -labsl_random_internal_randen_hwaes_impl \
        -labsl_random_internal_randen_slow \
        -labsl_random_internal_platform \
        -labsl_random_internal_seed_material \
        -labsl_random_seed_gen_exception \
        -labsl_status \
        -labsl_cord \
        -labsl_cordz_info \
        -labsl_cord_internal \
        -labsl_cordz_functions \
        -labsl_exponential_biased \
        -labsl_cordz_handle \
        -labsl_crc_cord_state \
        -labsl_crc32c \
        -labsl_crc_internal \
        -labsl_crc_cpu_detect \
        -labsl_flags_marshalling \
        -labsl_log_internal_check_op \
        -labsl_leak_check \
        -labsl_log_internal_conditions \
        -labsl_log_internal_message \
        -labsl_strerror \
        -labsl_log_internal_nullguard \
        -labsl_examine_stack \
        -labsl_log_internal_format \
        -labsl_str_format_internal \
        -labsl_log_internal_structured_proto \
        -labsl_log_internal_proto \
        -labsl_log_internal_log_sink_set \
        -labsl_log_globals \
        -labsl_hash \
        -labsl_bad_variant_access \
        -labsl_city \
        -labsl_low_level_hash \
        -labsl_vlog_config_internal \
        -labsl_bad_optional_access \
        -labsl_log_internal_fnmatch \
        -labsl_synchronization \
        -labsl_stacktrace \
        -labsl_symbolize \
        -labsl_debugging_internal \
        -labsl_demangle_internal \
        -labsl_demangle_rust \
        -labsl_decode_rust_punycode \
        -labsl_utf8_for_code_point \
        -labsl_graphcycles_internal \
        -labsl_kernel_timeout_internal \
        -labsl_malloc_internal \
        -labsl_tracing_internal \
        -labsl_log_internal_globals \
        -labsl_log_sink \
        -labsl_log_entry \
        -labsl_time \
        -labsl_civil_time \
        -labsl_time_zone \
        -labsl_strings \
        -labsl_int128 \
        -labsl_strings_internal \
        -labsl_string_view \
        -labsl_base \
        -labsl_spinlock_wait \
        -labsl_throw_delegate \
        -labsl_raw_logging_internal \
        -labsl_log_severity \
        -lssl \
        -lcrypto \
        -laddress_sorting
}
