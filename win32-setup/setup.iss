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

[Files]
Source: "..\bin\acid-pc.exe"; DestDir: "{app}"
Source: "..\bin\libgcc_s_dw2-1.dll"; DestDir: "{app}"
Source: "..\bin\mingwm10.dll"; DestDir: "{app}"
Source: "..\bin\QtCore4.dll"; DestDir: "{app}"
Source: "..\bin\QtGui4.dll"; DestDir: "{app}"
Source: "..\bin\QtNetwork4.dll"; DestDir: "{app}"
Source: "..\bin\QtXml4.dll"; DestDir: "{app}"
Source: "..\bin\QtMultimedia4.dll"; DestDir: "{app}"

[Icons]
Name: "{group}\ACId Messenger"; Filename: "{app}\acid-pc.exe"
Name: "{group}\SmartCommunity website"; Filename: "http://jsmart.web.id"
Name: "{group}\Uninstall ACId Messenger"; Filename: "{uninstallexe}"
