#include "JrojroSystem.h"

EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
)
{
    EFI_STATUS Status = EFI_SUCCESS;
  
    VIDEO_CONFIG VideoConfig;
    Status = VideoInit(ImageHandle, &VideoConfig);

    Status = DrawLogo(ImageHandle);

    for(UINTN i = 0; i < 10; i++)
    {
        Status = DrawStep(i);
    }

    return Status;
}