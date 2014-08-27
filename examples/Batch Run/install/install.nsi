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

 	!define GUID 			"{A0CD9E4B-EB20-40ab-B10A-D81F90C1D11F}"
	!define UDISPLAYNAME 	"TLB: Batch Run plugin"
	!define DISPLAYNAME		"Batch Run plugin"
	!define VERSION  		"2.0"
	!define FULLVERSION		"2.0.0.0"
	!define PLUGDIR 		"batchrun"
	!define PLUGFILE 		"batchrun"
	!define SRCFOLDER32		"..\Release"
	!define SRCFOLDER64		"..\x64\Release"
	!define DATAFOLDER 		"..\Release"
	!define LOCFILE 		"batchrun.xml"
	!define TLBREF32		"..\..\nsis\tlbref\32"
	!define TLBREF64		"..\..\nsis\tlbref\64"
	!define OUTFILE			"tlbbatchrun.exe"

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
	
	!insertmacro TLBPLUGINS_DEFUNINSTALL
SectionEnd
