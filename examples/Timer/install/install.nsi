;NSIS Modern User Interface version 1.70
;Basic Example Script
;Written by Joost Verburg

;--------------------------------
;Include Modern UI

	!addincludedir			"..\..\nsis"
	!include				"MUI2.nsh"
	!include				"Library.nsh"
	!include				"tlbplugins.nsh"
	
	SetCompressor /SOLID lzma
	RequestExecutionLevel admin

;--------------------------------
;Component data
	!define INSTALLARTPATH	"..\..\nsis\img"
	!define OUTPATH			"..\..\nsis\bin"

 	!define GUID 			"{CC2EEFBE-792B-4917-9668-EA4C2FE1B132}"
	!define UDISPLAYNAME 	"TLB: Timer plugin"
	!define DISPLAYNAME		"Timer plugin"
	!define VERSION  		"1.0"
	!define FULLVERSION		"1.0.0.0"
	!define PLUGDIR 		"timer"
	!define PLUGFILE 		"timer"
	!define SRCFOLDER32		"..\Release"
	!define SRCFOLDER64		"..\x64\Release"
	!define DATAFOLDER 		"..\x64\Debug"
	!define LOCFILE 		"timer.xml"
	!define TLBREF32		"..\..\nsis\tlbref\32"
	!define TLBREF64		"..\..\nsis\tlbref\64"
	!define OUTFILE			"tlbtimer.exe"
	
	!define HAVESKINS
		!define SKINFILE 		"timer.ini"
		!define SKINSECTION 	"info"
		!define SKINNAME 		"name"
	

!insertmacro TLBPLUGINS_START
  
;--------------------------------
;Languages
 
  !insertmacro MUI_LANGUAGE "English"
  ;OutFile "${OUTPATH}\${OUTFILE}"

;--------------------------------
;Installer Sections

Section "Core Components" SecCore
	SectionIn RO

	!insertmacro TLBPLUGINS_WRITEDEFVALUES

	CreateDirectory "$INSTDIR\skins"
	SetOutPath "$INSTDIR\skins"
	
  	SetOutPath "$INSTDIR"
	File "${DATAFOLDER}\sound.mp3"
	
  	SetOutPath "$INSTDIR\skins"
		File /r /x *svn* "${DATAFOLDER}\skins\*.*"

	WriteUninstaller "$INSTDIR\Uninstall.exe"

	!insertmacro TLBPLUGINS_SETREBOOTEVENT
SectionEnd

;--------------------------------
;Descriptions

	;Language strings
	LangString DESC_SecCore ${LANG_ENGLISH} "${DISPLAYNAME} core files."

	;Assign language strings to sections
	!insertmacro MUI_FUNCTION_DESCRIPTION_BEGIN
    !insertmacro MUI_DESCRIPTION_TEXT ${SecCore} $(DESC_SecCore)
	!insertmacro MUI_FUNCTION_DESCRIPTION_END

;--------------------------------
;Uninstaller Section

Section "Uninstall"
	!insertmacro TLBPLUGINS_UWRITEDEFVALUES
	
	RMDir /r "$INSTDIR\skins"
	
	!insertmacro TLBPLUGINS_DEFUNINSTALL
SectionEnd
