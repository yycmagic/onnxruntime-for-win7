#define _WIN32_WINNT 0x0601 // Windows 7 µÄ×îµÍ°æ±¾

#include <shcore.h>
#include <wincodec.h>
#include <wincodecsdk.h>

#include <filesystem>
#include <vector>

#include "image_file.h"

/**
 *  Read the file from `input_file` and auto-scale it to 720x720
 * \param out should be freed by caller after use
 * \param output_count Array length of the `out` param
 */
int read_image_file(_In_z_ const ORTCHAR_T* input_file, _Out_ size_t* height, _Out_ size_t* width, _Outptr_ float** out,
    _Out_ size_t* output_count) {
    IWICImagingFactory* piFactory = nullptr;
    if (FAILED(CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&piFactory)))) {
        return -1; // Failed to create WIC factory
    }

    IWICBitmapDecoder* decoder = nullptr;
    if (FAILED(piFactory->CreateDecoderFromFilename(input_file, NULL, GENERIC_READ,
        WICDecodeMetadataCacheOnDemand,  // defer parsing non-critical metadata
        &decoder))) {
        piFactory->Release();
        return -2; // Failed to create decoder
    }

    UINT count = 0;
    if (FAILED(decoder->GetFrameCount(&count)) || count != 1) {
        printf("The image has multiple frames, I don't know which to choose.\n");
        decoder->Release();
        piFactory->Release();
        return -3; // Invalid frame count
    }

    IWICBitmapFrameDecode* piFrameDecode = nullptr;
    if (FAILED(decoder->GetFrame(0, &piFrameDecode))) {
        decoder->Release();
        piFactory->Release();
        return -4; // Failed to get frame
    }

    UINT image_width, image_height;
    if (FAILED(piFrameDecode->GetSize(&image_width, &image_height))) {
        piFrameDecode->Release();
        decoder->Release();
        piFactory->Release();
        return -5; // Failed to get size
    }

    IWICBitmapScaler* scaler = nullptr;
    IWICBitmapSource* source_to_copy = piFrameDecode;
    if (image_width != 720 || image_height != 720) {
        if (FAILED(piFactory->CreateBitmapScaler(&scaler))) {
            piFrameDecode->Release();
            decoder->Release();
            piFactory->Release();
            return -6; // Failed to create scaler
        }
        if (FAILED(scaler->Initialize(source_to_copy, 720, 720, WICBitmapInterpolationModeFant))) {
            scaler->Release();
            piFrameDecode->Release();
            decoder->Release();
            piFactory->Release();
            return -7; // Failed to initialize scaler
        }
        source_to_copy = scaler;
        image_width = 720;
        image_height = 720;
    }

    IWICFormatConverter* ppIFormatConverter = nullptr;
    if (FAILED(piFactory->CreateFormatConverter(&ppIFormatConverter))) {
        if (scaler) scaler->Release();
        piFrameDecode->Release();
        decoder->Release();
        piFactory->Release();
        return -8; // Failed to create format converter
    }
    if (FAILED(ppIFormatConverter->Initialize(source_to_copy, GUID_WICPixelFormat24bppRGB,
        WICBitmapDitherTypeNone, NULL, 0.f, WICBitmapPaletteTypeCustom))) {
        ppIFormatConverter->Release();
        if (scaler) scaler->Release();
        piFrameDecode->Release();
        decoder->Release();
        piFactory->Release();
        return -9; // Failed to initialize format converter
    }

    // output format is 24bpp, which means 24 bits per pixel
    constexpr UINT bytes_per_pixel = 24 / 8;
    UINT stride = image_width * bytes_per_pixel;
    std::vector<uint8_t> data(image_width * image_height * bytes_per_pixel);
    if (FAILED(ppIFormatConverter->CopyPixels(nullptr, stride, static_cast<UINT>(data.size()), data.data()))) {
        ppIFormatConverter->Release();
        if (scaler) scaler->Release();
        piFrameDecode->Release();
        decoder->Release();
        piFactory->Release();
        return -10; // Failed to copy pixels
    }

    hwc_to_chw(data.data(), image_height, image_width, out, output_count);
    *height = image_height;
    *width = image_width;

    ppIFormatConverter->Release();
    if (scaler) scaler->Release();
    piFrameDecode->Release();
    decoder->Release();
    piFactory->Release();
    return 0;
}

int write_image_file(_In_ uint8_t* model_output_bytes, unsigned int height, unsigned int width,
    _In_z_ const ORTCHAR_T* output_file) {
    std::filesystem::path file_path(output_file);
    if (!file_path.has_extension()) {
        printf("Unrecognized file type!\n");
        return -1;
    }
    auto ext = file_path.extension().wstring();
    GUID container_format = GUID_ContainerFormatJpeg;
    if (_wcsicmp(ext.c_str(), L".jpg") == 0) {
        container_format = GUID_ContainerFormatJpeg;
    }
    else if (_wcsicmp(ext.c_str(), L".png") == 0) {
        container_format = GUID_ContainerFormatPng;
    }
    else if (_wcsicmp(ext.c_str(), L".bmp") == 0) {
        container_format = GUID_ContainerFormatBmp;
    }
    else {
        wprintf(L"Unrecognized file type:%s!\n", ext.c_str());
        return -1;
    }

    IWICImagingFactory* piFactory = nullptr;
    if (FAILED(CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&piFactory)))) {
        return -2; // Failed to create WIC factory
    }

    IWICStream* output_stream = nullptr;
    if (FAILED(piFactory->CreateStream(&output_stream))) {
        piFactory->Release();
        return -3; // Failed to create stream
    }
    if (FAILED(output_stream->InitializeFromFilename(output_file, GENERIC_WRITE))) {
        output_stream->Release();
        piFactory->Release();
        return -4; // Failed to initialize stream
    }

    IWICBitmapEncoder* encoder = nullptr;
    if (FAILED(piFactory->CreateEncoder(container_format, nullptr, &encoder))) {
        output_stream->Release();
        piFactory->Release();
        return -5; // Failed to create encoder
    }
    if (FAILED(encoder->Initialize(output_stream, WICBitmapEncoderNoCache))) {
        encoder->Release();
        output_stream->Release();
        piFactory->Release();
        return -6; // Failed to initialize encoder
    }

    IWICBitmapFrameEncode* frame = nullptr;
    IPropertyBag2* bag = nullptr;
    if (FAILED(encoder->CreateNewFrame(&frame, &bag))) {
        encoder->Release();
        output_stream->Release();
        piFactory->Release();
        return -7; // Failed to create frame
    }
    if (FAILED(frame->Initialize(bag))) {
        if (bag) bag->Release();
        frame->Release();
        encoder->Release();
        output_stream->Release();
        piFactory->Release();
        return -8; // Failed to initialize frame
    }
    frame->SetSize(width, height);
    WICPixelFormatGUID targetFormat = GUID_WICPixelFormat24bppRGB;
    if (FAILED(frame->SetPixelFormat(&targetFormat))) {
        if (bag) bag->Release();
        frame->Release();
        encoder->Release();
        output_stream->Release();
        piFactory->Release();
        return -9; // Failed to set pixel format
    }

    constexpr UINT bytes_per_pixel = 24 / 8;
    size_t stride = width * bytes_per_pixel;
    if (FAILED(frame->WritePixels(height, static_cast<UINT>(stride),
        static_cast<UINT>(height * width * bytes_per_pixel), model_output_bytes))) {
        if (bag) bag->Release();
        frame->Release();
        encoder->Release();
        output_stream->Release();
        piFactory->Release();
        return -10; // Failed to write pixels
    }
    if (FAILED(frame->Commit())) {
        if (bag) bag->Release();
        frame->Release();
        encoder->Release();
        output_stream->Release();
        piFactory->Release();
        return -11; // Failed to commit frame
    }
    if (FAILED(encoder->Commit())) {
        if (bag) bag->Release();
        frame->Release();
        encoder->Release();
        output_stream->Release();
        piFactory->Release();
        return -12; // Failed to commit encoder
    }

    if (bag) bag->Release();
    frame->Release();
    encoder->Release();
    output_stream->Release();
    piFactory->Release();
    return 0;
}
