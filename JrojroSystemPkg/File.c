#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

EFI_STATUS GetFileHandle(
    IN EFI_HANDLE ImageHandle,
    IN CHAR16 *FileName,
    OUT EFI_FILE_PROTOCOL **FileHandle
)
{
    EFI_STATUS Status = EFI_SUCCESS;
    UINTN NoHandle = 0;
    EFI_HANDLE *HandleBuffer;
    Status = gBS->LocateHandleBuffer(
        ByProtocol,
        &gEfiSimpleFileSystemProtocolGuid,
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

    Print(L"Get %d handle of EFI_SIMPLE_FILE_SYSTEM_PROTOCOL.\n", NoHandle);
	#endif

    EFI_SIMPLE_FILE_SYSTEM_PROTOCOL *FileSystem;
    Status = gBS->OpenProtocol(
        HandleBuffer[0],
        &gEfiSimpleFileSystemProtocolGuid,
        (void **)&FileSystem,
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

    Print(L"EFI_SIMPLE_FILE_SYSTEM_PROTOCOL is opened.\n");
	#endif

    EFI_FILE_PROTOCOL *Root;
    Status = FileSystem->OpenVolume(
        FileSystem,
        &Root
    );
    #ifdef DEBUG
	Print(L"Status = %d.\n", Status);

	if (EFI_ERROR(Status))
	{
		Print(L"Failed to OpenVolume.\n");
		return Status;
	}

    Print(L"Volume is opened.\n");
	#endif

    Status = Root->Open(
        Root,
        FileHandle,
        FileName,
        EFI_FILE_MODE_WRITE | EFI_FILE_MODE_READ,
        EFI_OPEN_PROTOCOL_GET_PROTOCOL
    );

    #ifdef DEBUG
	Print(L"Status = %d.\n", Status);

	if (EFI_ERROR(Status))
	{
		Print(L"Failed to Open.\n");
		return Status;
	}

    Print(L"File is opened.\n");
	#endif

    return Status;
}