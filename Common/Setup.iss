;; xsubst utility.
;; Copyright (c) 2004-2011 by Elijah Zarezky,
;; All rights reserved.

;; Licensed under the Apache License, Version 2.0 (the "License");
;; you may not use this file except in compliance with the License.
;; You may obtain a copy of the License at
;;
;; http://www.apache.org/licenses/LICENSE-2.0
;;
;; Unless required by applicable law or agreed to in writing, software
;; distributed under the License is distributed on an "AS IS" BASIS,
;; WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
;; See the License for the specific language governing permissions and
;; limitations under the License.

;; Setup.iss - setup script for Inno Setup compiler
;; NOTE that this file is generated automatically by NAnt.exe

[Setup]
AppName=xsubst
AppVerName=xsubst 1.1.5842
AppID={{5F7BE167-B54A-408C-9AE0-66F20E2BEFFC}
AppPublisher=Elijah Zarezky
AppPublisherURL=http://zarezky.spb.ru/
AppSupportURL=http://zarezky.spb.ru/projects/xsubst.html
AppUpdatesURL=http://zarezky.spb.ru/projects/xsubst.html
AppVersion=1.1.5842
AppCopyright=Copyright � 2004�2011 by Elijah Zarezky
DefaultDirName={pf}\Elijah Zarezky\xsubst
DefaultGroupName=Elijah Zarezky\xsubst
AllowNoIcons=true
Compression=lzma
SolidCompression=true
OutputDir=..\Setup
OutputBaseFilename=xsubst-1.1.5842-setup
VersionInfoVersion=1.1.5842.92
VersionInfoProductName=Power Gadgets
VersionInfoProductVersion=1.1
MinVersion=0,5.0.2195
PrivilegesRequired=admin
WizardImageFile=compiler:WizModernImage-IS.bmp
WizardSmallImageFile=compiler:WizModernSmallImage-IS.bmp
LicenseFile=ApacheLicense.rtf
AppMutex=WinSubst.Instance.326F8F0D-E321-4832-B29F-08542F06BCB9

[LangOptions]
DialogFontName=MS Shell Dlg 2
DialogFontSize=8

[Code]
//  Code Page default values
const
	CP_ACP = 0;
	CP_OEMCP = 1;
	CP_MACCP = 2;
	CP_THREAD_ACP = 3;
	CP_SYMBOL = 42;
	CP_UTF7 = 65000;
	CP_UTF8 = 65001;

// MBCS and Unicode translation flags
const
	MB_PRECOMPOSED = $0001;
	MB_COMPOSITE = $0002;
	MB_USEGLYPHCHARS = $0004;
	MB_ERR_INVALID_CHARS = $0008;

	WC_COMPOSITECHECK = $0200;
	WC_DISCARDNS = $0010;
	WC_SEPCHARS = $0020;
	WC_DEFAULTCHAR = $0040;
	WC_ERR_INVALID_CHARS = $0080;
	WC_NO_BEST_FIT_CHARS = $0400;

	DefFlagsMB = MB_PRECOMPOSED;
	DefFlagsWC = WC_COMPOSITECHECK or WC_DISCARDNS or WC_SEPCHARS or WC_DEFAULTCHAR;

function MultiByteToWideChar(CodePage, Flags: Integer; SrcStr: AnsiString; SrcLen: Integer; DestStr: String; DestLen: Integer): Integer;
external 'MultiByteToWideChar@kernel32.dll stdcall';
function WideCharToMultiByte(CodePage, Flags: Integer; SrcStr: String; SrcLen: Integer; DestStr: AnsiString; DestLen: Integer; DefChar: Integer; DefCharUsed: Integer): Integer;
external 'WideCharToMultiByte@kernel32.dll stdcall';

function AnsiStringToString(const SrcStr: AnsiString; CodePage: Integer): String;
var
	WideLen: Integer;
begin
	if (SrcStr <> '') then
	begin
		WideLen := MultiByteToWideChar(CodePage, DefFlagsMB, SrcStr, -1, Result, 0);
		SetLength(Result, WideLen - 1);
		MultiByteToWideChar(CodePage, DefFlagsMB, SrcStr, -1, Result, WideLen - 1);
	end
	else begin
		Result := '';
	end;
end;

function StringToAnsiString(const SrcStr: String; CodePage: Integer): AnsiString;
var
	AnsiLen: Integer;
begin
	if (SrcStr <> '') then
	begin
		AnsiLen := WideCharToMultiByte(CodePage, DefFlagsWC, SrcStr, -1, Result, 0, 0, 0);
		SetLength(Result, AnsiLen - 1);
		WideCharToMultiByte(CodePage, DefFlagsWC, SrcStr, -1, Result, AnsiLen - 1, 0, 0);
	end
	else begin
		Result := '';
	end;
end;

procedure CleanupHKCU();
begin
   RegDeleteKeyIncludingSubkeys(HKCU, 'Software\Elijah Zarezky\xsubst');
   RegDeleteKeyIfEmpty(HKCU, 'Software\Elijah Zarezky');
end;

function StopService(): Boolean;
var
	ResCode: Integer;
begin
	Exec(GetSystemDir() + '\net.exe', 'stop "Substituted Drives Manager"', GetSystemDir(), SW_HIDE, ewWaitUntilTerminated, ResCode);
	Result := True;
end;

function IsWinSxS(): Boolean;
var
	osVersion: TWindowsVersion;
begin
	GetWindowsVersionEx(osVersion);
	Result := ((osVersion.Major = 5) and (osVersion.Minor >= 1)) or (osVersion.Major > 5);
end;

const
	MagicStr = '{6DF4C042-C237-46b2-A205-C41DAAA4A0F5}';

procedure AdjustAfxConfig();
var
	AfxConfigPath: String;
	AfxConfigXML: AnsiString;
	WideXML: String;
begin
	if (IsWinSxS()) then
	begin
		AfxConfigPath := ExpandConstant(CurrentFileName());
		if (LoadStringFromFile(AfxConfigPath, AfxConfigXML)) then
		begin
			WideXML := AnsiStringToString(AfxConfigXML, CP_ACP);
			StringChangeEx(WideXML, MagicStr, ExpandConstant('{app}'), True);
			SaveStringToFile(AfxConfigPath, StringToAnsiString(WideXML, CP_ACP), False);
		end;
	end;
end;

[InstallDelete]
;; from 1.0 release
Type: files; Name: "{app}\msvcr71.dll"
Type: files; Name: "{app}\mfc71u.dll"
Type: files; Name: "{app}\SubstSvc.log"
;; from 1.1 pre-release
Type: filesandordirs; Name: "{app}\Microsoft.VC90.CRT"
Type: filesandordirs; Name: "{app}\Microsoft.VC90.MFC"
Type: files; Name: "{app}\Microsoft.VC90.MFC.manifest"
Type: files; Name: "{group}\xsubst on the Web.lnk"
Type: files; Name: "{app}\xsubst.url"
Type: files; Name: "{group}\View Substituted Drives Manager log.lnk"

[Files]
;; core application files
Source: "..\WinSubst\Output.2008\x86\Release\Unicode\WinSubst.exe"; DestDir: "{app}"
Source: "..\SubstSvc\Output.2008\x86\Release\Unicode\SubstSvc.exe"; DestDir: "{app}"; Check: StopService
Source: "..\WinSubst\Detours\Bin\detoured.dll"; DestDir: "{app}"
Source: ".\ApacheLicense.rtf"; DestDir: "{app}"; Flags: ignoreversion

;; CRT redistributables
Source: "..\Redist\Microsoft.VC90.CRT\msvcr90.dll"; DestDir: "{app}"
Source: "..\Redist\Microsoft.VC90.CRT\msvcp90.dll"; DestDir: "{app}"
Source: "..\Redist\Microsoft.VC90.CRT\msvcm90.dll"; DestDir: "{app}"
Source: "..\Redist\Microsoft.VC90.CRT\Microsoft.VC90.PrivateCRT.manifest"; DestDir: "{app}"; MinVersion: 0,5.01.2600
Source: "..\Redist\Microsoft.VC90.CRT\Microsoft.VC90.CRT.manifest"; DestDir: "{app}"; MinVersion: 0,5.01.2600

;; MFC library redistributables
Source: "..\Redist\Microsoft.VC90.MFC\mfc90u.dll"; DestDir: "{app}"
Source: "..\Redist\Microsoft.VC90.MFC\mfc90u.dll.1000.config"; DestDir: "{app}"; MinVersion: 0,5.01.2600; AfterInstall: AdjustAfxConfig
Source: "..\Redist\Microsoft.VC90.MFC\mfcm90u.dll"; DestDir: "{app}"
Source: "..\Redist\Microsoft.VC90.MFC\Microsoft.VC90.PrivateMFC.manifest"; DestDir: "{app}"; MinVersion: 0,5.01.2600

;; MFC library localizations
Source: "..\Redist\Microsoft.VC90.MFCLOC\mfc90enu.dll"; DestDir: "{app}"
Source: "..\Redist\Microsoft.VC90.MFCLOC\mfc90rus.dll"; DestDir: "{app}"
Source: "..\Redist\Microsoft.VC90.MFCLOC\Microsoft.VC90.MFCLOC.manifest"; DestDir: "{app}"; MinVersion: 0,5.01.2600

[Dirs]
Name: "{commonappdata}\Elijah Zarezky\WinSubst"; Flags: uninsalwaysuninstall

[Icons]
Name: "{group}\WinSubst"; Filename: "{app}\WinSubst.exe"; Comment: "Launch WinSubst to manage substituted drives"
Name: "{group}\License Agreement"; Filename: "{app}\ApacheLicense.rtf"; Comment: "View xsubst license agreement"
Name: "{group}\Uninstall xsubst"; Filename: "{uninstallexe}"; Comment: "Remove xsubst from your computer"
Name: "{group}\View Log"; FileName: "{commonappdata}\Elijah Zarezky\WinSubst\SubstSvc.log"; Comment: "View Substituted Drives Manager log"
Name: "{userdesktop}\WinSubst"; Filename: "{app}\WinSubst.exe"; Tasks: desktopicon; Comment: "Launch WinSubst to manage substituted drives"
Name: "{userappdata}\Microsoft\Internet Explorer\Quick Launch\WinSubst"; Filename: "{app}\WinSubst.exe"; Tasks: quicklaunchicon; Comment: "Launch WinSubst to manage substituted drives"

[INI]
Filename: "{group}\xsubst on the Web.url"; Section: "InternetShortcut"; Key: "URL"; String: "http://zarezky.spb.ru/projects/xsubst.html"
Filename: "{group}\xsubst on the Web.url"; Section: "InternetShortcut"; Key: "IconFile"; String: ""
Filename: "{group}\xsubst on the Web.url"; Section: "InternetShortcut"; Key: "IconIndex"; String: "0"
Filename: "{group}\xsubst on the Web.url"; Section: "InternetShortcut"; Key: "Hotkey"; String: "0"
Filename: "{group}\xsubst on the Web.url"; Section: "InternetShortcut"; Key: "WorkingDirectory"; String: ""
Filename: "{group}\xsubst on the Web.url"; Section: "InternetShortcut"; Key: "ShowCommand"; String: "1"

[Registry]
Root: HKU; Subkey: ".DEFAULT\Software\Elijah Zarezky"; Flags: uninsdeletekeyifempty
Root: HKU; Subkey: ".DEFAULT\Software\Elijah Zarezky\xsubst"; Flags: uninsdeletekey
Root: HKU; Subkey: ".DEFAULT\Software\Elijah Zarezky\xsubst\SubstSvc"; Flags: uninsdeletekey
Root: HKU; Subkey: ".DEFAULT\Software\Elijah Zarezky\xsubst\SubstSvc\Drives"; Flags: uninsdeletekey
Root: HKU; Subkey: ".DEFAULT\Software\Elijah Zarezky\xsubst\SubstSvc\Logging"; Flags: uninsdeletekey
Root: HKU; Subkey: ".DEFAULT\Software\Elijah Zarezky\xsubst\SubstSvc\Logging"; ValueType: string; ValueName: "TargetPath"; ValueData: "{commonappdata}\Elijah Zarezky\WinSubst\SubstSvc.log"; Flags: createvalueifdoesntexist
Root: HKU; Subkey: ".DEFAULT\Software\Elijah Zarezky\xsubst\SubstSvc\Logging"; ValueType: dword; ValueName: "Continuous"; ValueData: "1"; Flags: createvalueifdoesntexist
Root: HKU; Subkey: ".DEFAULT\Software\Elijah Zarezky\xsubst\WinSubst"; Flags: uninsdeletekey
Root: HKU; Subkey: ".DEFAULT\Software\Elijah Zarezky\xsubst\WinSubst\Catchpit"; Flags: uninsdeletekey
Root: HKU; Subkey: ".DEFAULT\Software\Elijah Zarezky\xsubst\WinSubst\Catchpit"; ValueType: dword; ValueName: "TortoiseShell.dll"; ValueData: "1"; Flags: createvalueifdoesntexist
Root: HKU; Subkey: ".DEFAULT\Software\Elijah Zarezky\xsubst\WinSubst\Catchpit"; ValueType: dword; ValueName: "TortoiseOverlays.dll"; ValueData: "1"; Flags: createvalueifdoesntexist
Root: HKU; Subkey: ".DEFAULT\Software\Elijah Zarezky\xsubst\WinSubst\Catchpit"; ValueType: dword; ValueName: "TortoiseSVN.dll"; ValueData: "1"; Flags: createvalueifdoesntexist
Root: HKU; Subkey: ".DEFAULT\Software\Elijah Zarezky\xsubst\WinSubst\Catchpit"; ValueType: dword; ValueName: "TortoiseStub.dll"; ValueData: "1"; Flags: createvalueifdoesntexist
Root: HKU; Subkey: ".DEFAULT\Software\Elijah Zarezky\xsubst\WinSubst\Catchpit"; ValueType: dword; ValueName: "PhoneBrowser.dll"; ValueData: "1"; Flags: createvalueifdoesntexist

;; compatibility mode for Windows Vista and Windows Server 2008
Root: HKLM; Subkey: "SOFTWARE\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers"; ValueType: string; ValueName: "{app}\WinSubst.exe"; ValueData: "WINXPSP2 RUNASADMIN"; Flags: uninsdeletevalue; MinVersion: 0,6.0; OnlyBelowVersion: 0,6.1
Root: HKLM; Subkey: "SOFTWARE\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers"; ValueType: string; ValueName: "{app}\SubstSvc.exe"; ValueData: "WINXPSP2 RUNASADMIN"; Flags: uninsdeletevalue; MinVersion: 0,6.0; OnlyBelowVersion: 0,6.1
;; compatibility mode for Windows 7 and Windows Server 2008 R2
Root: HKLM; Subkey: "SOFTWARE\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers"; ValueType: string; ValueName: "{app}\WinSubst.exe"; ValueData: "WINXPSP3 RUNASADMIN"; Flags: uninsdeletevalue; MinVersion: 0,6.1
Root: HKLM; Subkey: "SOFTWARE\Microsoft\Windows NT\CurrentVersion\AppCompatFlags\Layers"; ValueType: string; ValueName: "{app}\SubstSvc.exe"; ValueData: "WINXPSP3 RUNASADMIN"; Flags: uninsdeletevalue; MinVersion: 0,6.1

[Tasks]
Name: desktopicon; Description: "Create a &desktop icon"; GroupDescription: "Additional icons:"; Flags: unchecked
Name: quicklaunchicon; Description: "Create a &Quick Launch icon"; GroupDescription: "Additional icons:"; Flags: unchecked

[Run]
Filename: "{app}\SubstSvc.exe"; Parameters: "/Register"; StatusMsg: "Registering Substituted Drives Manager service..."; Flags: runhidden; AfterInstall: CleanupHKCU
Filename: "{sys}\net.exe"; Parameters: "start ""Substituted Drives Manager"""; StatusMsg: "Starting Substituted Drives Manager service..."; Flags: runhidden
Filename: "{app}\WinSubst.exe"; Description: "Launch WinSubst"; Flags: nowait postinstall skipifsilent runascurrentuser

[UninstallRun]
Filename: "{sys}\net.exe"; Parameters: "stop ""Substituted Drives Manager"""; StatusMsg: "Stopping Substituted Drives Manager service..."; Flags: runhidden
Filename: "{app}\SubstSvc.exe"; Parameters: "/UnRegister"; StatusMsg: "Unregistering Substituted Drives Manager service..."; Flags: runhidden; AfterInstall: CleanupHKCU

[UninstallDelete]
Type: files; Name: "{commonappdata}\Elijah Zarezky\WinSubst\SubstSvc.log"
Type: files; Name: "{group}\xsubst on the Web.url"

;; end of file
