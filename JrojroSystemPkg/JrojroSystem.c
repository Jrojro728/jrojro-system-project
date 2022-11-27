#include "JrojroSystem.h"

EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
)
{
  EFI_STATUS Status = EFI_SUCCESS;
  UINT8 Step = 1;
  BOOT_CONFIG BootConfig;
  
  VIDEO_CONFIG VideoConfig;
  Status = VideoInit(ImageHandle, &VideoConfig);
  DrawStep(Step++);
  DrawStep(Step++);

  Status = DrawLogo(ImageHandle);
  DrawStep(Step++);

  EFI_PHYSICAL_ADDRESS KernelEntryPoint;
  Status = Relocate(ImageHandle, L"\\Kernel.elf", &KernelEntryPoint);

  DrawStep(Step++);
    
  BootConfig.VideoConfig = VideoConfig;

  UINT64 (*KernelEntry)(BOOT_CONFIG *BootConfig);
  KernelEntry = (UINT64 (*)(BOOT_CONFIG *BootConfig))KernelEntryPoint;

  while (Step != 10)
  {
    DrawStep(Step);
    Step++;
  }
  
    
  UINT64 PassBack = KernelEntry(&BootConfig);
  Print(L"PassBack=%llX.\n", PassBack);

  return Status;
}