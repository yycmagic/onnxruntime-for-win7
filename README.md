## ONNX runtime for win7 win8 & windows server 2012 R2

[中文](README.md)
[English](README_EN.md)


## 简述

ONNX Runtime: cross-platform, high performance ML inferencing and training accelerator.
<https://github.com/microsoft/onnxruntime>

最后一个支持 win7 & win8 的版本应该是 v1.11.1，后面的就不支持了。
这给 **最低支持 win7 & win8** 的客户端软件带来了不便。
魔改了一下， **最低支持 win7 & win8** ，商业使用请联系 <526926911@qq.com> 获取授权。

The last version that supports Windows 7 & Windows 8 is likely v1.11.1, as later versions no longer provide support.
This poses challenges for client software with a **minimum requirement of Windows 7 & Windows 8** .
A modified version has been created to ensure **minimum support for Windows 7 & Windows 8** .
For commercial use, please contact <526926911@qq.com> for authorization.

1. Windows 7 (2009)。
2. Windows 8 (2012)。
3. Windows 8.1 (2013)。
4. Windows 10 (2015)。
5. Windows 11 (2021)。
6. Windows Server 2008、2012、2016、2019、2022 等。


## ONNX Runtime v1.20.0

<https://github.com/microsoft/onnxruntime/releases/tag/v1.20.0>
* onnxruntime-win-x64-1.20.0.zip
* onnxruntime-win-x86-1.20.0.zip

根据这两个版本改了一下，包含完整的运行时环境。
Modified based on these two versions, including the complete runtime environment.


## api-ms-win-core-heap-l2-1-0.dll missing

**api-ms-win-core-heap-l2-1-0.dll missing on windows server 2012 R2 #15025**
<https://github.com/microsoft/onnxruntime/issues/15025>

> **"Windows builds are not compatible with Windows 8.x in this release. Please use v1.11 for now."**
> great news, I looked dependency walker with onnxruntime.dll `v1.11.1` , it seems it doesn't depends on `api-ms-win-core-heap-l2-1-0.dll` .

<https://github.com/microsoft/onnxruntime/pull/10796>
**You can implement it yourself to support Windows 7. The issue is resolved here:**
**可以自己实现，以支持 win7，这里解决了：<https://github.com/yycmagic/onnxruntime-for-win7>**

**Avoid using LocalFree on FormatMessageW buffer #10796**
<https://github.com/microsoft/onnxruntime/pull/10796>

其实要真正跑起来，问题不止这些，要调整的地方还挺多。
To actually get it running, there are more issues than just these; quite a few adjustments are needed.

这里包含一个样例。
This includes a sample.

[onnxruntime-inference-examples](https://github.com/microsoft/onnxruntime-inference-examples)
[fns_candy_style_transfer](https://github.com/microsoft/onnxruntime-inference-examples/tree/main/c_cxx/fns_candy_style_transfer)

```
fns_candy_style_transfer.exe <model_path> <input_image_path> <output_image_path> [cpu|cuda|dml]
```

以下 PE 文件不包含签名：
The following PE files do not contain signatures:

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


## demo 编译

下载 onnxruntime-win-x64-1.20.0.zip 和 onnxruntime-win-x86-1.20.0.zip 放到 git 根目录并解压到各自的文件夹。
目录结构：
```
* onnxruntime-win-1.20.0\demo\fns_candy_style_transfer.sln
* onnxruntime-win-x64-1.20.0\include
* onnxruntime-win-x86-1.20.0\include
* onnxruntime-win-x64-1.20.0.zip
* onnxruntime-win-x86-1.20.0.zip
```

打开 `fns_candy_style_transfer.sln` 并完成编译。

构建结果：
```
onnxruntime-win-1.20.0\demo\Debug\fns_candy_style_transfer.exe
onnxruntime-win-1.20.0\demo\Debug64\fns_candy_style_transfer.exe
onnxruntime-win-1.20.0\demo\Release\fns_candy_style_transfer.exe
onnxruntime-win-1.20.0\demo\Release64\fns_candy_style_transfer.exe
```

直接运行：
```
fns_candy_style_transfer.exe candy.onnx image.png result.png
```
