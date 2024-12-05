# onnxruntime-for-win7

## ONNX runtime for win7 win8 & windows server 2012 R2

[中文]()
[English]()


## 简述

ONNX Runtime: cross-platform, high performance ML inferencing and training accelerator.
<https://github.com/microsoft/onnxruntime>

最后一个支持 win7 & win8 的版本应该是 v1.11.1，后面的就不支持了。
这给 **最低支持 win7 & win8** 的客户端软件带来了不便。
魔改了一下， **最低支持 win7 & win8** ，商业使用请联系 <526926911@qq.com> 获取授权。
非商业使用，无任何限制。

1. Windows 7 (2009)。
2. Windows 8 (2012)。
3. Windows 10 (2015)。
4. Windows 11 (2021)。
5. Windows Server 2008、2012、2016、2019、2022 等。


## ONNX Runtime v1.20.0

<https://github.com/microsoft/onnxruntime/releases/tag/v1.20.0>
onnxruntime-win-x64-1.20.0.zip
onnxruntime-win-x86-1.20.0.zip

根据这两个版本改了一下，包含完整的运行时环境。

包含一个完整的实例，[版面识别 {% include relref_github.html %}](https://github.com/opendatalab/DocLayout-YOLO)，假如本地路径是 `D:\work` 。
```
D:
cd D:\work\x86
doclayout.exe layout32.onnx
cd D:\work\x64
doclayout.exe layout32.onnx
```

输出文件是：`doclayout.out.json` 。


## api-ms-win-core-heap-l2-1-0.dll missing

**api-ms-win-core-heap-l2-1-0.dll missing on windows server 2012 R2 #15025**
<https://github.com/microsoft/onnxruntime/issues/15025>

{% include image.html url="/invisible/images/infai-onnx/224756907-e47a299c-eb12-48ad-ae88-6e3aaf919192.png" %}

> **"Windows builds are not compatible with Windows 8.x in this release. Please use v1.11 for now."**
> great news, I looked dependency walker with onnxruntime.dll `v1.11.1` , it seems it doesn't depends on `api-ms-win-core-heap-l2-1-0.dll` .

**avoid using LocalFree on FormatMessageW buffer #10796**
<https://github.com/microsoft/onnxruntime/pull/10796>

其实要真正跑起来，问题不止这些，要调整的地方还挺多。

onnxruntime-inference-examples
fns_candy_style_transfer

```
fns_candy_style_transfer.exe <model_path> <input_image_path> <output_image_path> [cpu|cuda|dml]
```

* x64\\api-ms-win-core-heap-l2-1-0.dll
* x64\\api-ms-win-core-path-l1-1-0.dll
* x64\\api-ms-win-eventing-provider-l1-1-0.dll
* x64\\fastkn64.dll
* x64\\onnxrt64.dll
* x64\\onnxruntime.dll

* x86\\api-ms-win-core-heap-l2-1-0.dll
* x86\\api-ms-win-core-path-l1-1-0.dll
* x86\\api-ms-win-eventing-provider-l1-1-0.dll
* x86\\fastkn32.dll
* x86\\onnxrt32.dll
* x86\\onnxruntime.dll

以下 PE 文件不包含签名：

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
