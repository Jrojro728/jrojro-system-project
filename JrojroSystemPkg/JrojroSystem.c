#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

EFI_STATUS
EFIAPI
UefiMain (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
)
{
	EFI_STATUS Status = EFI_SUCCESS;
	UINTN NoHandles = 0;
	EFI_HANDLE *Buffer = NULL;
	Status = gBS->LocateHandleBuffer (
		ByProtocol,
		&gEfiSimpleFileSystemProtocolGuid,
		NULL,
		&NoHandles,
		&Buffer
	);
	
	return Status;
}