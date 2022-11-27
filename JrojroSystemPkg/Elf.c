#include "Elf.h"

EFI_STATUS Relocate(
    IN EFI_HANDLE ImageHandle,
    IN CHAR16 *FileName,
    OUT EFI_PHYSICAL_ADDRESS *KernelEntry
)
{
    EFI_STATUS Status = EFI_SUCCESS;
    EFI_FILE_PROTOCOL *Kernel;
    Status = GetFileHandle(ImageHandle, FileName, &Kernel);

    EFI_PHYSICAL_ADDRESS KernelBuffer;
    Status = ReadFile(Kernel, &KernelBuffer);

    Status = CheckELF(KernelBuffer);

    Status = LoadSegments(KernelBuffer, KernelEntry);

    return Status;
}

EFI_STATUS CheckELF(
    IN EFI_PHYSICAL_ADDRESS KernelBuffer
)
{
    EFI_STATUS Status = EFI_SUCCESS;

    ELF_HEADER_64 *KernelElfHeader = (ELF_HEADER_64 *)KernelBuffer;
    UINT32 Magic = KernelElfHeader->Magic;
    if(Magic != 0x464c457F)
    {
        Status = NOT_ELF;
        return Status;
    }

    UINT8 Format = KernelElfHeader->Format;
    if (Format != ELF_64)
    {
        Status = NOT_64_BIT;
        return NOT_64_BIT;
    }
    
    return Status;
}

EFI_STATUS LoadSegments(
    IN EFI_PHYSICAL_ADDRESS KernelBufferBase,
    OUT EFI_PHYSICAL_ADDRESS *KernelEntry
)
{
    EFI_STATUS Status = EFI_SUCCESS;
    ELF_HEADER_64 *ElfHeader = (ELF_HEADER_64 *)KernelBufferBase;
    PROGRAM_HEADER_64 *PHeader = (PROGRAM_HEADER_64 *)(KernelBufferBase + ElfHeader->Phoff); 

    EFI_PHYSICAL_ADDRESS LowAddr = 0xFFFFFFFFFFFFFFFF;
    EFI_PHYSICAL_ADDRESS HighAddr = 0;

    for (UINTN i = 0; i < ElfHeader->PHeadCount; i++)
    {
        if (PHeader[i].Type == PT_LOAD) {
            if (LowAddr > PHeader[i].PAddress) {
                LowAddr = PHeader[i].PAddress;
            }

            if (HighAddr < (PHeader[i].PAddress + PHeader[i].SizeInMemory)) {
                HighAddr = PHeader[i].PAddress + PHeader[i].SizeInMemory;
            }
        }
    }
      
    UINTN PageCount = (HighAddr - LowAddr + 4095) / 4096;
    EFI_PHYSICAL_ADDRESS KernelRelocateBase;
    Status = gBS->AllocatePages(
        AllocateAnyPages,
        EfiLoaderCode,
        PageCount,
        &KernelRelocateBase
    );
    if (EFI_ERROR(Status)) {
        return Status;
    }

    UINT64 RelocateOffset = KernelRelocateBase - LowAddr;
    UINT64 *ZeroStart = (UINT64 *)KernelRelocateBase;
    for (UINTN i = 0; i < (PageCount << 9); i++)
    {
        *ZeroStart = 0x000000000000;
        ZeroStart++;
    }

    for (UINTN i = 0; i < ElfHeader->PHeadCount; i++)
    {
        if (PHeader[i].Type == PT_LOAD)
        {
            UINT8 *SourceStart = (UINT8 *)KernelBufferBase + PHeader[i].Offset;
            UINT8 *DestStart = (UINT8 *)PHeader[i].VAddress + RelocateOffset;

            for(UINTN j = 0; j < PHeader[i].SizeInFile; j++)
            {
                *DestStart = *SourceStart;                            
                SourceStart++;
                DestStart++;               
            }
        }
    }
    *KernelEntry = ElfHeader->Entry + RelocateOffset;
    
    return Status;
}