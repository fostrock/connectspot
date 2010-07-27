xcopy .\3rdlib\lightopc\lib\*.dll .\bin /s /y /i
xcopy .\3rdlib\lightopc\lib\*.lib .\bin /s /y /i
xcopy .\3rdlib\unilog\*.dll .\bin /s /y /i
xcopy .\3rdlib\boost\v_1_41\lib\*.dll .\bin /s /y /i
xcopy .\3rdlib\libxml++\v_2_26\lib\*.dll .\bin /s /y /i

xcopy .\srcs\opcspot\zsdriver.xml .\bin\config /s /y /i

rem VC9 CRT
xcopy "F:\Microsoft Visual Studio 9.0\VC\redist\x86\Microsoft.VC90.CRT\*.*" .\bin /s /y /i
xcopy "F:\Microsoft Visual Studio 9.0\VC\redist\Debug_NonRedist\x86\Microsoft.VC90.DebugCRT\*.*" .\bin /s /y /i
