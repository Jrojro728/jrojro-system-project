#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

EFI_STATUS GetGopHandle(
    IN EFI_HANDLE ImageHandle,
    OUT EFI_GRAPHICS_OUTPUT_PROTOCOL *Gop
)
{
    EFI_STATUS Status = EFI_SUCCESS;
    UINTN NoHandle = 0;
    EFI_HANDLE *HandleBuffer;
    Status = gBS->LocateHandleBuffer(
        ByProtocol,
        &gEfiGraphicsOutputProtocolGuid,
        NULL,
        &NoHandle,
        &HandleBuffer
    );

    #ifdef DEBUG
	Print(L"Status = %d.\n", Status);

	if (EFI_ERROR(Status))
	{
		Print(L"Failed to LocateHandleBuffer.\n");
		return Status;
	}

	Print(L"Get %d handle of EFI_GRAPHICS_OUTPUT_PROTOCOL.\n", NoHandle);
	#endif

    Status = gBS->OpenProtocol(
        HandleBuffer[0],
        &gEfiGraphicsOutputProtocolGuid,
        (void **) Gop,
        ImageHandle,
        NULL,
        EFI_OPEN_PROTOCOL_GET_PROTOCOL
    );

    #ifdef DEBUG
	Print(L"Status = %d.\n", Status);

	if (EFI_ERROR(Status))
	{
		Print(L"Failed to OpenProtocol.\n");
		return Status;
	}

    Print(L"EFI_GRAPHICS_OUTPUT_PROTOCOL is open.\n")
	#endif

    return Status;
}