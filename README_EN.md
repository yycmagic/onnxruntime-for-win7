## ONNX Runtime for Windows 7, Windows 8 & Windows Server 2012 R2

[中文](README.md)
[English](README_EN.md)

**Supporting ONNX Runtime on Windows 7**
The project involves AI-related components and relies on ONNX Runtime. After downloading a precompiled version from the official website, it ran successfully during testing.
However, during actual deployment, it was found that Windows 7 was not supported, with errors indicating missing `api-ms-core-...` DLLs. As a result, I had to modify ONNX Runtime manually.


## Overview

**ONNX Runtime** : A cross-platform, high-performance machine learning inferencing and training accelerator.
<https://github.com/microsoft/onnxruntime>

The last version supporting **Windows 7** and **Windows 8** is likely **v1.11.1** , as later versions do not provide support.
This creates challenges for client software requiring a **minimum of Windows 7 & Windows 8** .
A modified version has been created to ensure compatibility with these operating systems.
**For commercial use, please contact <526926911@qq.com> for authorization.**

1. **Windows 7** (2009)
2. **Windows 8** (2012)
3. **Windows 8.1** (2013)
4. **Windows 10** (2015)
5. **Windows 11** (2021)
6. **Windows Server versions** : 2008, 2012, 2016, 2019, 2022, etc.


## ONNX Runtime v1.20.0

<https://github.com/microsoft/onnxruntime/releases/tag/v1.20.0>
- **onnxruntime-win-x64-1.20.0.zip**
- **onnxruntime-win-x86-1.20.0.zip**

The runtime environment has been modified based on these two versions to include full support.


## `api-ms-win-core-heap-l2-1-0.dll` Missing Issue

**Issue: api-ms-win-core-heap-l2-1-0.dll missing on Windows Server 2012 R2 #15025**
<https://github.com/microsoft/onnxruntime/issues/15025>

> **"Windows builds are not compatible with Windows 8.x in this release. Please use v1.11 for now."**
> Great news! Dependency Walker analysis of `onnxruntime.dll v1.11.1` shows no dependency on `api-ms-win-core-heap-l2-1-0.dll` .

**Solution:**
You can implement the required support for **Windows 7** yourself. Detailed instructions are available here:
**<https://github.com/yycmagic/onnxruntime-for-win7>**

**Reference:**
- **Avoid using LocalFree on FormatMessageW buffer #10796**
<https://github.com/microsoft/onnxruntime/pull/10796>

To ensure full functionality, several additional adjustments are required.


## Sample Application

Sample code and examples can be found here:
- **[onnxruntime-inference-examples](https://github.com/microsoft/onnxruntime-inference-examples)**
- **[fns_candy_style_transfer](https://github.com/microsoft/onnxruntime-inference-examples/tree/main/c_cxx/fns_candy_style_transfer)**

Command:
```shell
fns_candy_style_transfer.exe <model_path> <input_image_path> <output_image_path> [cpu|cuda|dml]
```


## Modified PE Files and Dependencies

Unsigned PE Files,
The following files are unsigned:

* x64\\api-ms-win-core-heap-l2-1-0.dll
* x64\\api-ms-win-core-libraryloader-l1-2-0.dll
* x64\\api-ms-win-core-path-l1-1-0.dll
* x64\\api-ms-win-eventing-provider-l1-1-0.dll
* x64\\fastkn64.dll
* x64\\onnxrt64.dll
* x64\\onnxruntime.dll

* x86\\api-ms-win-core-heap-l2-1-0.dll
* x86\\api-ms-win-core-libraryloader-l1-2-0.dll
* x86\\api-ms-win-core-path-l1-1-0.dll
* x86\\api-ms-win-eventing-provider-l1-1-0.dll
* x86\\fastkn32.dll
* x86\\onnxrt32.dll
* x86\\onnxruntime.dll


## Demo Compilation Instructions

1. Download **onnxruntime-win-x64-1.20.0.zip** and **onnxruntime-win-x86-1.20.0.zip** .
2. Place the files in the project root and extract them.

Directory structure:
```
onnxruntime-win-1.20.0\demo\fns_candy_style_transfer.sln
onnxruntime-win-x64-1.20.0\include
onnxruntime-win-x86-1.20.0\include
onnxruntime-win-x64-1.20.0.zip
onnxruntime-win-x86-1.20.0.zip
```

3. Open `fns_candy_style_transfer.sln` and build the project.

Build output:
```
onnxruntime-win-1.20.0\demo\Debug\fns_candy_style_transfer.exe
onnxruntime-win-1.20.0\demo\Debug64\fns_candy_style_transfer.exe
onnxruntime-win-1.20.0\demo\Release\fns_candy_style_transfer.exe
onnxruntime-win-1.20.0\demo\Release64\fns_candy_style_transfer.exe
```

4. Run the executable:
```shell
fns_candy_style_transfer.exe candy.onnx image.png result.png
```
