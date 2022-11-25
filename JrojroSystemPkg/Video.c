#include "Video.h"

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

EFI_STATUS GetGopHandle(
    IN EFI_HANDLE ImageHandle,
    OUT EFI_GRAPHICS_OUTPUT_PROTOCOL **Gop
)
{
    EFI_STATUS Status = EFI_SUCCESS;
    UINTN NoHandle;
    EFI_HANDLE *HandleBuffer;
    Status = gBS->LocateHandleBuffer(
        ByProtocol,
        &gEfiGraphicsOutputProtocolGuid,
        NULL,
        &NoHandle,
        &HandleBuffer
    );

    Status = gBS->OpenProtocol(
        HandleBuffer[0],
        &gEfiGraphicsOutputProtocolGuid,
        (void **) Gop,
        ImageHandle,
        NULL,
        EFI_OPEN_PROTOCOL_GET_PROTOCOL
    );

    return Status;
}

EFI_STATUS SetVideoMode(
    IN EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop
)
{
    EFI_STATUS Status = EFI_SUCCESS;
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION *ModeInfo;
    UINTN ModeInfoSize = sizeof(EFI_GRAPHICS_OUTPUT_MODE_INFORMATION);
    UINTN H = 0;
    UINTN V = 0;
    UINTN ModeIndex = 0;

    for(UINTN i = 0; i < Gop->Mode->MaxMode; i++) {
        Status = Gop->QueryMode(Gop, i, &ModeInfoSize, &ModeInfo);
        H = ModeInfo->HorizontalResolution;
        V = ModeInfo->VerticalResolution;
        if(((H == 1920) && (V == 1080))){
            ModeIndex = i;
        }
    }

    Status = Gop->SetMode(Gop, ModeIndex);

    return Status;
}

EFI_STATUS BmpTransform(
    IN EFI_PHYSICAL_ADDRESS BmpBase,
    OUT BMP_CONFIG *BmpConfig
)
{
    EFI_STATUS Status = EFI_SUCCESS;
    struct bmp_header *bmp_header = (struct bmp_header *)BmpBase;
    Print(L"File size = %08lx.\n", bmp_header->file_size);

    BmpConfig->Size = bmp_header->file_size;
    BmpConfig->PageSize = (BmpConfig->Size >> 12) + 1;
    BmpConfig->Offset = bmp_header->header_size;
    BmpConfig->Width = bmp_header->width;
    BmpConfig->Height = bmp_header->height;
    BmpConfig->PixelStart = BmpBase + BmpConfig->Offset;
    
    EFI_PHYSICAL_ADDRESS PixelStart;
    Status = gBS->AllocatePages(
        AllocateAnyPages,
        EfiLoaderData,
        BmpConfig->PageSize,
        &PixelStart
    );

    EFI_GRAPHICS_OUTPUT_BLT_PIXEL *PixelFromFile = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)(BmpBase 
                                                    + BmpConfig->Offset 
                                                    + BmpConfig->Width * BmpConfig->Height * 4);
    EFI_GRAPHICS_OUTPUT_BLT_PIXEL *PixelToBuffer = (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)PixelStart;
    for(UINTN i = 0; i < BmpConfig->Height; i++)
    {
        PixelFromFile -= BmpConfig->Width;
        for(UINTN j = 0; j < BmpConfig->Width; j++)
        {
            *PixelToBuffer = *PixelFromFile;
            PixelToBuffer++;
            PixelFromFile++;
        }
        PixelFromFile -= BmpConfig->Width;
    }

    BmpConfig->PixelStart = PixelStart;

    return Status;
}

EFI_STATUS DrawBmp(
    IN EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop,
    IN BMP_CONFIG BmpConfig,
    IN UINTN X,
    IN UINTN Y
)
{
    EFI_STATUS Status = EFI_SUCCESS;

    Status = Gop->Blt(
        Gop,
        (EFI_GRAPHICS_OUTPUT_BLT_PIXEL *)BmpConfig.PixelStart,
        EfiBltBufferToVideo,
        0,0,
        X,Y,
        BmpConfig.Width, BmpConfig.Height,
        0
    );

    return Status;
}