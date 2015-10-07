############################################################################################
#      NSIS Installation Script created by NSIS Quick Setup Script Generator v1.09.18
#               Entirely Edited with NullSoft Scriptable Installation System                
#              by Vlasis K. Barkas aka Red Wine red_wine@freemail.gr Sep 2006               
############################################################################################

!define APP_NAME "CAN2"
!define COMP_NAME "CAN Developers"
!define VERSION "00.00.00.01"
!define COPYRIGHT "Author � 2015"
!define DESCRIPTION "Application"
!define INSTALLER_NAME "setup.exe"
!define MAIN_APP_EXE "CAN2.exe"
!define INSTALL_TYPE "SetShellVarContext current"
!define REG_ROOT "HKCU"
!define REG_APP_PATH "Software\Microsoft\Windows\CurrentVersion\App Paths\${MAIN_APP_EXE}"
!define UNINSTALL_PATH "Software\Microsoft\Windows\CurrentVersion\Uninstall\${APP_NAME}"

!define REG_START_MENU "Start Menu Folder"

var SM_Folder

######################################################################

VIProductVersion  "${VERSION}"
VIAddVersionKey "ProductName"  "${APP_NAME}"
VIAddVersionKey "CompanyName"  "${COMP_NAME}"
VIAddVersionKey "LegalCopyright"  "${COPYRIGHT}"
VIAddVersionKey "FileDescription"  "${DESCRIPTION}"
VIAddVersionKey "FileVersion"  "${VERSION}"

######################################################################

SetCompressor ZLIB
Name "${APP_NAME}"
Caption "${APP_NAME}"
OutFile "${INSTALLER_NAME}"
BrandingText "${APP_NAME}"
XPStyle on
InstallDirRegKey "${REG_ROOT}" "${REG_APP_PATH}" ""
InstallDir "$PROGRAMFILES\CAN2"

######################################################################

!include "MUI.nsh"

!define MUI_ABORTWARNING
!define MUI_UNABORTWARNING

!define MUI_LANGDLL_REGISTRY_ROOT "${REG_ROOT}"
!define MUI_LANGDLL_REGISTRY_KEY "${UNINSTALL_PATH}"
!define MUI_LANGDLL_REGISTRY_VALUENAME "Installer Language"

!insertmacro MUI_PAGE_WELCOME

!ifdef LICENSE_TXT
!insertmacro MUI_PAGE_LICENSE "${LICENSE_TXT}"
!endif

!insertmacro MUI_PAGE_DIRECTORY

!ifdef REG_START_MENU
!define MUI_STARTMENUPAGE_DEFAULTFOLDER "CAN2"
!define MUI_STARTMENUPAGE_REGISTRY_ROOT "${REG_ROOT}"
!define MUI_STARTMENUPAGE_REGISTRY_KEY "${UNINSTALL_PATH}"
!define MUI_STARTMENUPAGE_REGISTRY_VALUENAME "${REG_START_MENU}"
!insertmacro MUI_PAGE_STARTMENU Application $SM_Folder
!endif

!insertmacro MUI_PAGE_INSTFILES

!define MUI_FINISHPAGE_RUN "$INSTDIR\${MAIN_APP_EXE}"
!insertmacro MUI_PAGE_FINISH

!insertmacro MUI_UNPAGE_CONFIRM

!insertmacro MUI_UNPAGE_INSTFILES

!insertmacro MUI_UNPAGE_FINISH

!insertmacro MUI_LANGUAGE "English"
!insertmacro MUI_LANGUAGE "German"

!insertmacro MUI_RESERVEFILE_LANGDLL

######################################################################

Function .onInit
!insertmacro MUI_LANGDLL_DISPLAY
FunctionEnd

######################################################################

RequestExecutionLevel admin

Section -MainProgram
${INSTALL_TYPE}
SetOverwrite ifnewer
SetOutPath "$INSTDIR"
File "..\Deploy\CAN2\avcodec-55.dll"
File "..\Deploy\CAN2\avformat-55.dll"
File "..\Deploy\CAN2\avutil-53.dll"
File "..\Deploy\CAN2\CAN2.exe"
File "..\Deploy\CAN2\freetype6.dll"
File "..\Deploy\CAN2\icudt53.dll"
File "..\Deploy\CAN2\icuin53.dll"
File "..\Deploy\CAN2\icuuc53.dll"
File "..\Deploy\CAN2\libgcc_s_dw2-1.dll"
File "..\Deploy\CAN2\libgcc_s_sjlj-1.dll"
File "..\Deploy\CAN2\libgit2.dll"
File "..\Deploy\CAN2\libjpeg-8.dll"
File "..\Deploy\CAN2\libmp3lame-0.dll"
File "..\Deploy\CAN2\libogg-0.dll"
File "..\Deploy\CAN2\libopenjpeg.dll"
File "..\Deploy\CAN2\libopus-0.dll"
File "..\Deploy\CAN2\libpng12.dll"
File "..\Deploy\CAN2\libpoppler-qt5.dll"
File "..\Deploy\CAN2\libpoppler.dll"
File "..\Deploy\CAN2\libstdc++-6.dll"
File "..\Deploy\CAN2\libvo-aacenc-0.dll"
File "..\Deploy\CAN2\libvorbis-0.dll"
File "..\Deploy\CAN2\libvorbisenc-2.dll"
File "..\Deploy\CAN2\libwinpthread-1.dll"
File "..\Deploy\CAN2\libx264-142.dll"
File "..\Deploy\CAN2\Qt5Core.dll"
File "..\Deploy\CAN2\Qt5Gui.dll"
File "..\Deploy\CAN2\Qt5Multimedia.dll"
File "..\Deploy\CAN2\Qt5Network.dll"
File "..\Deploy\CAN2\Qt5Widgets.dll"
File "..\Deploy\CAN2\Qt5Xml.dll"
File "..\Deploy\CAN2\soundtouch.dll"
SetOutPath "$INSTDIR\platforms"
File "..\Deploy\CAN2\platforms\qwindows.dll"
SectionEnd

######################################################################

Section -Icons_Reg
SetOutPath "$INSTDIR"
WriteUninstaller "$INSTDIR\uninstall.exe"

!ifdef REG_START_MENU
!insertmacro MUI_STARTMENU_WRITE_BEGIN Application
CreateDirectory "$SMPROGRAMS\$SM_Folder"
CreateShortCut "$SMPROGRAMS\$SM_Folder\${APP_NAME}.lnk" "$INSTDIR\${MAIN_APP_EXE}"
CreateShortCut "$DESKTOP\${APP_NAME}.lnk" "$INSTDIR\${MAIN_APP_EXE}"
CreateShortCut "$SMPROGRAMS\$SM_Folder\Uninstall ${APP_NAME}.lnk" "$INSTDIR\uninstall.exe"

!ifdef WEB_SITE
WriteIniStr "$INSTDIR\${APP_NAME} website.url" "InternetShortcut" "URL" "${WEB_SITE}"
CreateShortCut "$SMPROGRAMS\$SM_Folder\${APP_NAME} Website.lnk" "$INSTDIR\${APP_NAME} website.url"
!endif
!insertmacro MUI_STARTMENU_WRITE_END
!endif

!ifndef REG_START_MENU
CreateDirectory "$SMPROGRAMS\CAN2"
CreateShortCut "$SMPROGRAMS\CAN2\${APP_NAME}.lnk" "$INSTDIR\${MAIN_APP_EXE}"
CreateShortCut "$DESKTOP\${APP_NAME}.lnk" "$INSTDIR\${MAIN_APP_EXE}"
CreateShortCut "$SMPROGRAMS\CAN2\Uninstall ${APP_NAME}.lnk" "$INSTDIR\uninstall.exe"

!ifdef WEB_SITE
WriteIniStr "$INSTDIR\${APP_NAME} website.url" "InternetShortcut" "URL" "${WEB_SITE}"
CreateShortCut "$SMPROGRAMS\CAN2\${APP_NAME} Website.lnk" "$INSTDIR\${APP_NAME} website.url"
!endif
!endif

WriteRegStr ${REG_ROOT} "${REG_APP_PATH}" "" "$INSTDIR\${MAIN_APP_EXE}"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "DisplayName" "${APP_NAME}"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "UninstallString" "$INSTDIR\uninstall.exe"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "DisplayIcon" "$INSTDIR\${MAIN_APP_EXE}"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "DisplayVersion" "${VERSION}"
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "Publisher" "${COMP_NAME}"

!ifdef WEB_SITE
WriteRegStr ${REG_ROOT} "${UNINSTALL_PATH}"  "URLInfoAbout" "${WEB_SITE}"
!endif

${registerExtension} "c:\myplayer.exe" ".mkv" "MKV_File"

SectionEnd

######################################################################


Section Uninstall
${INSTALL_TYPE}
Delete "$INSTDIR\avcodec-55.dll"
Delete "$INSTDIR\avformat-55.dll"
Delete "$INSTDIR\avutil-53.dll"
Delete "$INSTDIR\CAN2.exe"
Delete "$INSTDIR\freetype6.dll"
Delete "$INSTDIR\icudt53.dll"
Delete "$INSTDIR\icuin53.dll"
Delete "$INSTDIR\icuuc53.dll"
Delete "$INSTDIR\libgcc_s_dw2-1.dll"
Delete "$INSTDIR\libgcc_s_sjlj-1.dll"
Delete "$INSTDIR\libgit2.dll"
Delete "$INSTDIR\libjpeg-8.dll"
Delete "$INSTDIR\libmp3lame-0.dll"
Delete "$INSTDIR\libogg-0.dll"
Delete "$INSTDIR\libopenjpeg.dll"
Delete "$INSTDIR\libopus-0.dll"
Delete "$INSTDIR\libpng12.dll"
Delete "$INSTDIR\libpoppler-qt5.dll"
Delete "$INSTDIR\libpoppler.dll"
Delete "$INSTDIR\libstdc++-6.dll"
Delete "$INSTDIR\libvo-aacenc-0.dll"
Delete "$INSTDIR\libvorbis-0.dll"
Delete "$INSTDIR\libvorbisenc-2.dll"
Delete "$INSTDIR\libwinpthread-1.dll"
Delete "$INSTDIR\libx264-142.dll"
Delete "$INSTDIR\Qt5Core.dll"
Delete "$INSTDIR\Qt5Gui.dll"
Delete "$INSTDIR\Qt5Multimedia.dll"
Delete "$INSTDIR\Qt5Network.dll"
Delete "$INSTDIR\Qt5Widgets.dll"
Delete "$INSTDIR\Qt5Xml.dll"
Delete "$INSTDIR\soundtouch.dll"
Delete "$INSTDIR\platforms\qwindows.dll"
 
RmDir "$INSTDIR\platforms"
 
Delete "$INSTDIR\uninstall.exe"
!ifdef WEB_SITE
Delete "$INSTDIR\${APP_NAME} website.url"
!endif

RmDir "$INSTDIR"

!ifdef REG_START_MENU
!insertmacro MUI_STARTMENU_GETFOLDER "Application" $SM_Folder
Delete "$SMPROGRAMS\$SM_Folder\${APP_NAME}.lnk"
Delete "$SMPROGRAMS\$SM_Folder\Uninstall ${APP_NAME}.lnk"
!ifdef WEB_SITE
Delete "$SMPROGRAMS\$SM_Folder\${APP_NAME} Website.lnk"
!endif
Delete "$DESKTOP\${APP_NAME}.lnk"

RmDir "$SMPROGRAMS\$SM_Folder"
!endif

!ifndef REG_START_MENU
Delete "$SMPROGRAMS\CAN2\${APP_NAME}.lnk"
Delete "$SMPROGRAMS\CAN2\Uninstall ${APP_NAME}.lnk"
!ifdef WEB_SITE
Delete "$SMPROGRAMS\CAN2\${APP_NAME} Website.lnk"
!endif
Delete "$DESKTOP\${APP_NAME}.lnk"

RmDir "$SMPROGRAMS\CAN2"
!endif

DeleteRegKey ${REG_ROOT} "${REG_APP_PATH}"
DeleteRegKey ${REG_ROOT} "${UNINSTALL_PATH}"

${unregisterExtension} ".can" "CAN_File"

SectionEnd

######################################################################

Function un.onInit
!insertmacro MUI_UNGETLANGUAGE
FunctionEnd

######################################################################

