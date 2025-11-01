; ScaleChord VST3 Plugin - NSIS Installer
; This file configures the Windows installer

!include "MUI2.nsh"
!include "x64.nsh"

; ==================================================
; Define product information
; ==================================================
!define PRODUCT_NAME "ScaleChord Plugin"
!define PRODUCT_VERSION "1.0.0"
!define PRODUCT_PUBLISHER "ScaleChord Development"
!define PRODUCT_WEB_SITE "https://github.com/your-repo/production-works"
!define PRODUCT_UNINST_KEY "Software\Microsoft\Windows\CurrentVersion\Uninstall\${PRODUCT_NAME}"

; ==================================================
; General Settings
; ==================================================
Name "${PRODUCT_NAME} ${PRODUCT_VERSION}"
OutFile "ScaleChord Setup ${PRODUCT_VERSION}.exe"
InstallDir "$PROGRAMFILES\${PRODUCT_NAME}"
ShowInstDetails show
ShowUnInstDetails show

; ==================================================
; MUI Settings
; ==================================================
!insertmacro MUI_PAGE_WELCOME
!insertmacro MUI_PAGE_DIRECTORY
!insertmacro MUI_PAGE_INSTFILES
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_WELCOME
!insertmacro MUI_UNPAGE_CONFIRM
!insertmacro MUI_UNPAGE_INSTFILES

!insertmacro MUI_LANGUAGE "English"

; ==================================================
; Installer Sections
; ==================================================
Section "Install"
  SetOutPath "$INSTDIR"
  
  ; Copy application files
  File "dist\win-unpacked\ScaleChord Plugin.exe"
  File "dist\win-unpacked\*.dll"
  File "dist\win-unpacked\*.node"
  
  ; Copy resources
  File /r "dist\win-unpacked\resources"
  
  ; Copy VST3 plugin
  SetOutPath "$INSTDIR\vst3_plugin"
  File /r "vst3_plugin\*.*"
  
  ; Create Start Menu shortcuts
  CreateDirectory "$SMPROGRAMS\${PRODUCT_NAME}"
  CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME}.lnk" "$INSTDIR\ScaleChord Plugin.exe"
  CreateShortCut "$SMPROGRAMS\${PRODUCT_NAME}\Uninstall.lnk" "$INSTDIR\uninst.exe"
  
  ; Create Desktop shortcut
  CreateShortCut "$DESKTOP\${PRODUCT_NAME}.lnk" "$INSTDIR\ScaleChord Plugin.exe"
  
  ; Register uninstaller
  WriteUninstaller "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_UNINST_KEY}" "DisplayName" "${PRODUCT_NAME} ${PRODUCT_VERSION}"
  WriteRegStr HKLM "${PRODUCT_UNINST_KEY}" "UninstallString" "$INSTDIR\uninst.exe"
  WriteRegStr HKLM "${PRODUCT_UNINST_KEY}" "DisplayVersion" "${PRODUCT_VERSION}"
  WriteRegStr HKLM "${PRODUCT_UNINST_KEY}" "DisplayIcon" "$INSTDIR\ScaleChord Plugin.exe"
  WriteRegStr HKLM "${PRODUCT_UNINST_KEY}" "URLInfoAbout" "${PRODUCT_WEB_SITE}"
  
  ; Install VST3 plugin to system location (requires admin)
  ${If} ${RunningX64}
    SetRegView 64
    CreateDirectory "$APPDATA\Local\Programs\Common Files\VST3"
    SetOverwrite ifnewer
    SetOutPath "$APPDATA\Local\Programs\Common Files\VST3"
    File /r "vst3_plugin\ScaleChord.vst3"
  ${Else}
    CreateDirectory "$APPDATA\Local\Programs\Common Files\VST3"
    SetOverwrite ifnewer
    SetOutPath "$APPDATA\Local\Programs\Common Files\VST3"
    File /r "vst3_plugin\ScaleChord.vst3"
  ${EndIf}
  
  MessageBox MB_ICONINFORMATION "Installation complete!$\n$\nScaleChord is now installed.$\n$\nThe VST3 plugin has been installed to your system."
SectionEnd

; ==================================================
; Uninstaller Section
; ==================================================
Section "Uninstall"
  ; Remove program files
  Delete "$INSTDIR\ScaleChord Plugin.exe"
  Delete "$INSTDIR\uninst.exe"
  RMDir /r "$INSTDIR"
  
  ; Remove Start Menu items
  Delete "$SMPROGRAMS\${PRODUCT_NAME}\${PRODUCT_NAME}.lnk"
  Delete "$SMPROGRAMS\${PRODUCT_NAME}\Uninstall.lnk"
  RMDir "$SMPROGRAMS\${PRODUCT_NAME}"
  
  ; Remove Desktop shortcut
  Delete "$DESKTOP\${PRODUCT_NAME}.lnk"
  
  ; Remove registry entries
  DeleteRegKey HKLM "${PRODUCT_UNINST_KEY}"
  
  ; Note: VST3 plugin left in system directory (user can manually remove if desired)
  MessageBox MB_ICONINFORMATION "Uninstallation complete!$\n$\nScaleChord has been removed.$\n$\nNote: The VST3 plugin remains installed. You can manually remove it from:$\n$APPDATA\Local\Programs\Common Files\VST3\ScaleChord.vst3"
SectionEnd

; ==================================================
; Helper functions
; ==================================================
Function .onInit
  ${If} ${RunningX64}
    SetRegView 64
  ${EndIf}
FunctionEnd
