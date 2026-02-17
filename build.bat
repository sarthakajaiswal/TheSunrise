@REM cl.exe /c /EHsc /Fo:output\ /I C:\glew-2.1.0\include src\OGL.cpp  src\scenes\introScene.cpp  src\imgui_manager.cpp
@REM cl.exe /c /EHsc /Fo:output\ /I C:\glew-2.1.0\include src\eventManager.cpp
cl.exe /c /EHsc /Fo:output\ /I C:\glew-2.1.0\include src\scenes\introScene.cpp
@REM cl.exe /c /EHsc /Fo:output\ /I C:\glew-2.1.0\include src\imgui_manager.cpp
@REM cl.exe /c /EHsc /Fo:output\ /I C:\glew-2.1.0\include src\OGL.cpp
@REM cl.exe /c /EHsc /Fo:output\ /I C:\glew-2.1.0\include src\alphabets.cpp
@REM cl.exe /c /EHsc /Fo:output\ /I C:\glew-2.1.0\include src\fbo.cpp
link.exe /OUT:OGL.exe output\*.obj output\OGL.res /LIBPATH:C:\glew-2.1.0\lib\Release\x64 glew32.lib /SUBSYSTEM:WINDOWS 

@REM cl.exe /c /EHsc /Fo:output\ /I C:\glew-2.1.0\include src\scenes\testScene.cpp src\fileHandler.cpp src\objects.cpp src\camera.cpp 
@REM rc.exe /fo output\OGL.res src\OGL.rc 
@REM link.exe /OUT:OGL.exe output\*.obj output\OGL.res /LIBPATH:C:\glew-2.1.0\lib\Release\x64 glew32.lib /SUBSYSTEM:WINDOWS 
