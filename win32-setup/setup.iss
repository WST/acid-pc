[Setup]
AppName=ACId Messenger
AppVersion=0.1-devel-preview
AppPublisher=SmartCommunity
AppPublisherURL="http://jsmart.web.id"
DefaultDirName={pf}\ACId Messenger
DefaultGroupName=SmartCommunity
UninstallDisplayIcon={app}\acid-pc.exe
Compression=lzma2/ultra
SolidCompression=yes
OutputDir="..\bin"
WizardImageFile="setupbg.bmp"
LicenseFile="licence.rtf"
DirExistsWarning=yes
ShowLanguageDialog=yes
VersionInfoProductVersion=0.1
VersionInfoCompany=SmartCommunity
VersionInfoCopyright="(c) SmartCommunity"
VersionInfoProductName="ACId Messenger"
VersionInfoDescription="Easy to use Jabber/XMPP powered IM application"
VersionInfoVersion=0.1

[Files]
Source: "..\bin\acid-pc.exe"; DestDir: "{app}"
Source: "..\bin\libgcc_s_dw2-1.dll"; DestDir: "{app}"
Source: "..\bin\mingwm10.dll"; DestDir: "{app}"
Source: "..\bin\qxmpp0.dll"; DestDir: "{app}"
Source: "..\bin\QtCore4.dll"; DestDir: "{app}"
Source: "..\bin\QtGui4.dll"; DestDir: "{app}"
Source: "..\bin\QtNetwork4.dll"; DestDir: "{app}"
Source: "..\bin\QtXml4.dll"; DestDir: "{app}"
Source: "..\bin\QtMultimedia4.dll"; DestDir: "{app}"
Source: "..\i18n\acid-pc_ru.qm"; DestDir: "{app}"
Source: "..\i18n\acid-pc_id.qm"; DestDir: "{app}"
Source: "..\i18n\acid-pc_jawa.qm"; DestDir: "{app}"
Source: "..\i18n\acid-pc_sunda.qm"; DestDir: "{app}"
Source: "..\shared\sounds\default\*"; DestDir: "{app}\shared\sounds\default"
Source: "..\shared\emoticons\default\*"; DestDir: "{app}\shared\emoticons\default"

[Icons]
Name: "{group}\ACId Messenger"; Filename: "{app}\acid-pc.exe"
Name: "{group}\SmartCommunity website"; Filename: "http://jsmart.web.id"
Name: "{group}\Uninstall ACId Messenger"; Filename: "{uninstallexe}"
