#include <Library/BaseMemoryLib.h>
#include <Protocol/GraphicsOutput.h>

#include "File.h"

typedef struct
{
    EFI_PHYSICAL_ADDRESS       FrameBufferBase;
    UINTN                      FrameBufferSize;
    UINT32                     HorizontalResolution;
    UINT32                     VerticalResolution;
    UINT32                     PixelsPerScanLine;
} VIDEO_CONFIG;

typedef struct
{
    UINTN Size;
    UINTN PageSize;
    UINTN Width;
    UINTN Height;
    UINTN Offset;
    UINT64 PixelStart;
} BMP_CONFIG;

#pragma pack(1)
struct bmp_header {
    UINT8 sig[2];
    UINT32 file_size;
    UINT16 reserved1;
    UINT16 reserved2;
    UINT32 header_size;
    UINT32 info_header_size;
    UINT32 width;
    UINT32 height;
    UINT16 plane_num;
    UINT16 color_bit;
    UINT32 compression_type;
    UINT32 compression_size;
    UINT32 horizontal_pixel;
    UINT32 vertical_pixel;
    UINT32 color_num;
    UINT32 essentail_num;
};
#pragma pack()

EFI_STATUS GetGopHandle(
    IN EFI_HANDLE ImageHandle,
    OUT EFI_GRAPHICS_OUTPUT_PROTOCOL **Gop
);

EFI_STATUS SetVideoMode(
    IN EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop
);

EFI_STATUS BmpTransform(
    IN EFI_PHYSICAL_ADDRESS BmpBase,
    OUT BMP_CONFIG *BmpConfig
);

EFI_STATUS DrawBmp(
    IN EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop,
    IN BMP_CONFIG BmpConfig,
    IN UINTN X,
    IN UINTN Y
);