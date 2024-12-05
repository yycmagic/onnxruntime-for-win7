@echo off
setlocal enabledelayedexpansion

set FILE=onnxruntime.dll

cd x64
copy /Y "api-ms-win-core-heap-l2-1-0.dll" "fastkn64.dll"
copy /Y "api-ms-win-core-heap-l2-1-0.dll" "api-ms-win-core-path-l1-1-0.dll"
copy /Y "api-ms-win-core-heap-l2-1-0.dll" "api-ms-win-eventing-provider-l1-1-0.dll"
for %%A in (%FILE%) do set FILESIZE=%%~zA
if %FILESIZE% GTR 1048576 (
    echo 文件 %FILE% 大于 1MB，执行复制操作...
    copy /Y "onnxruntime.dll" "onnxrt64.dll"
    chgimport.exe onnxrt64.dll
) else (
    echo 文件 %FILE% 小于或等于 1MB，跳过复制。
)
copy /Y "fastonnx.dll" "onnxruntime.dll"
cd ..

cd x86
copy /Y "api-ms-win-core-heap-l2-1-0.dll" "fastkn32.dll"
copy /Y "api-ms-win-core-heap-l2-1-0.dll" "api-ms-win-core-path-l1-1-0.dll"
copy /Y "api-ms-win-core-heap-l2-1-0.dll" "api-ms-win-eventing-provider-l1-1-0.dll"
for %%A in (%FILE%) do set FILESIZE=%%~zA
if %FILESIZE% GTR 1048576 (
    echo 文件 %FILE% 大于 1MB，执行复制操作...
    copy /Y "onnxruntime.dll" "onnxrt32.dll"
    chgimport.exe onnxrt32.dll
) else (
    echo 文件 %FILE% 小于或等于 1MB，跳过复制。
)
copy /Y "fastonnx.dll" "onnxruntime.dll"
cd ..

endlocal
@pause

void EtwRegistrationManager::LazyInitialize() {
  if (initialization_status_ == InitializationStatus::NotInitialized) {
    std::lock_guard<std::mutex> lock(init_mutex_);
    if (initialization_status_ == InitializationStatus::NotInitialized) {  // Double-check locking pattern
      initialization_status_ = InitializationStatus::Initializing;
      etw_status_ = ::TraceLoggingRegisterEx(etw_provider_handle, ORT_TL_EtwEnableCallback, nullptr);
      if (FAILED(etw_status_)) {
        // Registration can fail when running under Low Integrity process, and should be non-fatal
        initialization_status_ = InitializationStatus::Failed;
        // Injection of ETW logger can happen very early if ETW provider was already listening.
        // Don't use LOGS_DEFAULT here or can get "Attempt to use DefaultLogger but none has been registered"
        std::cerr << "Error in ETW registration: " << std::to_string(etw_status_) << std::endl;
      }
      initialization_status_ = InitializationStatus::Initialized;
    }
  }
}

修改函数 OverrideLevelWithEtw 直接返回。
procdump -ma -e 1 -x .. "doclayout.exe layout32.onnx"

