!ifndef TLBPLUGINS_INCLUDED

!define TLBPLUGINS_INCLUDED

;---------------------------------
; Startup macro to init all
!macro TLBPLUGINS_START
		SilentUnInstall silent
	
		;Name and file
		Name "${DISPLAYNAME}"
		BrandingText "${DISPLAYNAME} v${VERSION}"
	
		;Default installation folder
		InstallDir "$PROGRAMFILES64\TrueLaunchBar\plugins\${PLUGDIR}"
		InstallDirRegKey HKLM "Software\True Software\TrueLaunchBar\Uninstall\${GUID}" "InstallPath"
	
		VIAddVersionKey "ProductName" 		"${DISPLAYNAME}"
		VIAddVersionKey "CompanyName" 		"Tordex"
		VIAddVersionKey "LegalCopyright" 	"Copyright (c) 2000-2009 Tordex"
		VIAddVersionKey "FileDescription" 	"${DISPLAYNAME} for True Launch Bar"
		VIAddVersionKey "FileVersion" 		"${FULLVERSION}"
		VIAddVersionKey "ProductVersion"	"${FULLVERSION}"
		
		VIProductVersion "${FULLVERSION}"

		;--------------------------------
		;Interface Settings
		
			!define MUI_ABORTWARNING
			!define MUI_WELCOMEFINISHPAGE_BITMAP 	"${INSTALLARTPATH}\tlb.bmp"
			!define MUI_HEADERIMAGE
			!define MUI_HEADERIMAGE_BITMAP 			"${INSTALLARTPATH}\header.bmp"
			!define MUI_ICON 						"${INSTALLARTPATH}\setup.ico"
			!define MUI_UNICON 						"${INSTALLARTPATH}\setup.ico"
			!define MUI_HEADERIMAGE_RIGHT
		
		;--------------------------------
		;Pages
		
			!insertmacro MUI_PAGE_WELCOME
			!insertmacro MUI_PAGE_COMPONENTS
			!insertmacro MUI_PAGE_DIRECTORY
			!insertmacro MUI_PAGE_INSTFILES
			!insertmacro MUI_PAGE_FINISH

		OutFile "${OUTPATH}\${OUTFILE}"

		Function .onInit
			Call GetPluginPath
			Pop $R0
			StrCmp $R0 "" noFound
			StrCpy $INSTDIR $R0
			Goto end
		noFound:
			Call GetTLBPath
			Pop $R0
			StrCmp $R0 "" end
			StrCpy $INSTDIR "$R0\plugins\${PLUGDIR}"
		end:
		FunctionEnd
		
		Function GetParentDir
		 
			 Exch $R0
			 Push $R1
			 Push $R2
			 Push $R3
			 
			 StrCpy $R1 0
			 StrLen $R2 $R0
			 
			 loop:
				 IntOp $R1 $R1 + 1
				 IntCmp $R1 $R2 get 0 get
				 StrCpy $R3 $R0 1 -$R1
				 StrCmp $R3 "\" get
				 Goto loop
			 
			 get:
				 StrCpy $R0 $R0 -$R1
				 
				 Pop $R3
				 Pop $R2
				 Pop $R1
				 Exch $R0
				 
		 FunctionEnd
		
		Function GetPluginPath
			ReadRegStr $R0 HKCR "CLSID\${GUID}\InprocServer32" ""
			Push $R0
			Call GetParentDir
		FunctionEnd
		
		Function GetTLBPath
			ReadRegStr $R0 HKCR "CLSID\{FC712CA0-A945-11d4-A594-956F6349FC18}\InprocServer32" ""
			Push $R0
			Call GetParentDir
		FunctionEnd
!macroend

;---------------------------------
; write std section values

!macro TLBPLUGINS_WRITEDEFVALUES
	${If} ${RunningX64}
		SetRegView 64
	${EndIf}
	; write uninstall strings
	WriteRegStr HKLM "Software\True Software\TrueLaunchBar\Uninstall\${GUID}" "DisplayName" "${DISPLAYNAME}"
	WriteRegStr HKLM "Software\True Software\TrueLaunchBar\Uninstall\${GUID}" "UninstallString" '"$INSTDIR\Uninstall.exe"'
	WriteRegStr HKLM "Software\True Software\TrueLaunchBar\Uninstall\${GUID}" "Icon" "$INSTDIR\${PLUGFILE}.png"
	WriteRegStr HKLM "Software\True Software\TrueLaunchBar\Uninstall\${GUID}" "Version" "${FULLVERSION}"
	WriteRegStr HKLM "Software\True Software\TrueLaunchBar\Uninstall\${GUID}" "InstallPath" "$INSTDIR"
	WriteRegStr HKLM "SOFTWARE\True Software\TrueLaunchBar\plugins\${GUID}" "locfile" 				"${LOCFILE}"
	!ifdef HAVESKINS
		WriteRegStr HKLM "SOFTWARE\True Software\TrueLaunchBar\plugins\${GUID}" "skinFile" 				"${SKINFILE}"
		WriteRegStr HKLM "SOFTWARE\True Software\TrueLaunchBar\plugins\${GUID}" "skinNameSection" "${SKINSECTION}"
		WriteRegStr HKLM "SOFTWARE\True Software\TrueLaunchBar\plugins\${GUID}" "skinNameValue" 	"${SKINNAME}"
	!endif

	SetOutPath "$INSTDIR"
	File ${DATAFOLDER}\${PLUGFILE}.png

	${If} ${RunningX64}
		!define LIBRARY_X64
		!insertmacro InstallLib REGDLL NOTSHARED REBOOT_NOTPROTECTED "${SRCFOLDER64}\${PLUGFILE}.dll" "$INSTDIR\${PLUGFILE}.dll" 	"$INSTDIR"
		!undef LIBRARY_X64
		File "${TLBREF64}\tlbref.exe"
	${Else}
		!insertmacro InstallLib REGDLL NOTSHARED REBOOT_NOTPROTECTED "${SRCFOLDER32}\${PLUGFILE}.dll" "$INSTDIR\${PLUGFILE}.dll" 	"$INSTDIR"
		File "${TLBREF32}\tlbref.exe"
	${EndIf}
	ExecWait '"$INSTDIR\tlbref.exe" /P'
	Delete "$INSTDIR\tlbref.exe"
!macroend

!macro TLBPLUGINS_SETREBOOTEVENT
			IfSilent slLabel endS
		slLabel:
			IfRebootFlag reboot endS
		reboot:
			StrCpy $0 "2"
			StrCpy $1 "0"
			StrCpy $2 "tlbInstallEvent"
			System::Call 'Kernel32::OpenEventA(i, i, t)i (r0, r1, r2) .r3'
			System::Call 'Kernel32::SetEvent(i)i (r3) .r0'
			System::Call 'Kernel32::CloseHandle(i)i (r3) .r0'
		endS:
!macroend

!macro TLBPLUGINS_UWRITEDEFVALUES
	${If} ${RunningX64}
		SetRegView 64
	${EndIf}
	DeleteRegKey HKLM "Software\True Software\TrueLaunchBar\Uninstall\${GUID}"
	DeleteRegKey HKLM "SOFTWARE\True Software\TrueLaunchBar\plugins\${GUID}"
	${If} ${RunningX64}
		!define LIBRARY_X64
		!insertmacro UnInstallLib REGDLL NOTSHARED REBOOT_NOTPROTECTED "$INSTDIR\${PLUGFILE}.dll"
		!undef LIBRARY_X64
	${Else}
		!insertmacro UnInstallLib REGDLL NOTSHARED REBOOT_NOTPROTECTED "$INSTDIR\${PLUGFILE}.dll"
	${EndIf}
	Delete /REBOOTOK "$INSTDIR\${PLUGFILE}.png"
!macroend

!macro TLBPLUGINS_DEFUNINSTALL
  Delete "$INSTDIR\Uninstall.exe"
	IfRebootFlag uiReboot uiNoReboot
uiReboot:
  RMDir /REBOOTOK "$INSTDIR"
	Goto uiEnd
uiNoReboot:
  RMDir "$INSTDIR"
uiEnd:
	!insertmacro TLBPLUGINS_SETREBOOTEVENT
!macroend

!endif