#pragma once
#include "logging.h"

namespace playeveryware::eos::temp
{
    typedef EOS_EResult(EOS_CALL* EOS_Initialize_t)(const EOS_InitializeOptions* Options);
    typedef EOS_EResult(EOS_CALL* EOS_Shutdown_t)();
    typedef EOS_HPlatform(EOS_CALL* EOS_Platform_Create_t)(const EOS_Platform_Options* Options);
    typedef void (EOS_CALL* EOS_Platform_Release_t)(EOS_HPlatform Handle);
    typedef EOS_EResult(EOS_CALL* EOS_Logging_SetLogLevel_t)(EOS_ELogCategory LogCategory, EOS_ELogLevel LogLevel);
    typedef EOS_EResult(EOS_CALL* EOS_Logging_SetCallback_t)(EOS_LogMessageFunc Callback);

    typedef EOS_EResult(*EOS_IntegratedPlatformOptionsContainer_Add_t)(EOS_HIntegratedPlatformOptionsContainer Handle, const EOS_IntegratedPlatformOptionsContainer_AddOptions* InOptions);
    typedef EOS_EResult(*EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainer_t)(const EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainerOptions* Options, EOS_HIntegratedPlatformOptionsContainer* OutIntegratedPlatformOptionsContainerHandle);
    typedef void (*EOS_IntegratedPlatformOptionsContainer_Release_t)(EOS_HIntegratedPlatformOptionsContainer IntegratedPlatformOptionsContainerHandle);


    static EOS_Initialize_t EOS_Initialize_ptr;
    static EOS_Shutdown_t EOS_Shutdown_ptr;
    static EOS_Platform_Create_t EOS_Platform_Create_ptr;
    static EOS_Platform_Release_t EOS_Platform_Release_ptr;
    static EOS_Logging_SetLogLevel_t EOS_Logging_SetLogLevel_ptr;
    static EOS_Logging_SetCallback_t EOS_Logging_SetCallback_ptr;

    static EOS_IntegratedPlatformOptionsContainer_Add_t EOS_IntegratedPlatformOptionsContainer_Add_ptr;
    static EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainer_t EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainer_ptr;
    static EOS_IntegratedPlatformOptionsContainer_Release_t EOS_IntegratedPlatformOptionsContainer_Release_ptr;

    static void* s_eos_sdk_overlay_lib_handle;
    static void* s_eos_sdk_lib_handle;
    static EOS_HPlatform eos_platform_handle;

    //-------------------------------------------------------------------------
    inline static void* load_library_at_path(const std::filesystem::path& library_path)
    {
        void* to_return = nullptr;

#if PLATFORM_WINDOWS
        logging::log_inform(("Loading path at " + string_helpers::to_utf8_str(library_path)).c_str());
        HMODULE handle = LoadLibrary(library_path.c_str());
        to_return = (void*)handle;
#endif

        return to_return;
    }

    //-------------------------------------------------------------------------
    inline static void* load_function_with_name(void* library_handle, const char* function)
    {
        void* to_return = nullptr;
#if PLATFORM_WINDOWS
        HMODULE handle = (HMODULE)library_handle;
        to_return = (void*)GetProcAddress(handle, function);
#endif
        return to_return;
    }

    //-------------------------------------------------------------------------
    template<typename T>
    inline static T load_function_with_name(void* library_handle, const char* function)
    {
        return reinterpret_cast<T>(load_function_with_name(library_handle, function));
    }

    inline static const char* pick_if_32bit_else(const char* choice_if_32bit, const char* choice_if_else)
    {
#if PLATFORM_32BITS
        return choice_if_32bit;
#else
        return choice_if_else;
#endif
    }

    //-------------------------------------------------------------------------
    inline static void FetchEOSFunctionPointers()
    {
        // The '@' in the function names is apart of how names are mangled on windows. The value after the '@' is the size of the params on the stack
        EOS_Initialize_ptr = load_function_with_name<EOS_Initialize_t>(s_eos_sdk_lib_handle, pick_if_32bit_else("_EOS_Initialize@4", "EOS_Initialize"));
        EOS_Shutdown_ptr = load_function_with_name<EOS_Shutdown_t>(s_eos_sdk_lib_handle, pick_if_32bit_else("_EOS_Shutdown@0", "EOS_Shutdown"));
        EOS_Platform_Create_ptr = load_function_with_name<EOS_Platform_Create_t>(s_eos_sdk_lib_handle, pick_if_32bit_else("_EOS_Platform_Create@4", "EOS_Platform_Create"));
        EOS_Platform_Release_ptr = load_function_with_name<EOS_Platform_Release_t>(s_eos_sdk_lib_handle, pick_if_32bit_else("_EOS_Platform_Release@4", "EOS_Platform_Release"));
        EOS_Logging_SetLogLevel_ptr = load_function_with_name<EOS_Logging_SetLogLevel_t>(s_eos_sdk_lib_handle, pick_if_32bit_else("_EOS_Logging_SetLogLevel@8", "EOS_Logging_SetLogLevel"));
        EOS_Logging_SetCallback_ptr = load_function_with_name<EOS_Logging_SetCallback_t>(s_eos_sdk_lib_handle, pick_if_32bit_else("EOS_Logging_SetCallback@4", "EOS_Logging_SetCallback"));

        EOS_IntegratedPlatformOptionsContainer_Add_ptr = load_function_with_name<EOS_IntegratedPlatformOptionsContainer_Add_t>(s_eos_sdk_lib_handle, pick_if_32bit_else("_EOS_IntegratedPlatformOptionsContainer_Add@8", "EOS_IntegratedPlatformOptionsContainer_Add"));
        EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainer_ptr = load_function_with_name<EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainer_t>(s_eos_sdk_lib_handle, pick_if_32bit_else("_EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainer@8", "EOS_IntegratedPlatform_CreateIntegratedPlatformOptionsContainer"));
        EOS_IntegratedPlatformOptionsContainer_Release_ptr = load_function_with_name<EOS_IntegratedPlatformOptionsContainer_Release_t>(s_eos_sdk_lib_handle, pick_if_32bit_else("_EOS_IntegratedPlatformOptionsContainer_Release@4", "EOS_IntegratedPlatformOptionsContainer_Release"));
    }

}
