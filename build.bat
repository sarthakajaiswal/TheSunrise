@REM cl.exe /c /EHsc /Fo:output\ /I C:\glew-2.1.0\include src\*.cpp  src\scenes\*.cpp src\effects\*.cpp ImGui\*.cpp
@REM cl.exe /c /EHsc /Fo:output\ /I C:\glew-2.1.0\include src\*.cpp  src\scenes\*.cpp src\effects\*.cpp 
@REM cl.exe /c /EHsc /Fo:output\ /I C:\glew-2.1.0\include ImGui\*.cpp
@REM cl.exe /c /EHsc /Fo:output\ /I C:\glew-2.1.0\include src\OGL.cpp  src\scenes\introScene.cpp  src\imgui_manager.cpp
@REM cl.exe /c /EHsc /Fo:output\ /I C:\glew-2.1.0\include src\OGL.cpp  
@REM cl.exe /c /EHsc /Fo:output\ /I C:\glew-2.1.0\include src\imgui_manager.cpp
@REM cl.exe /c /EHsc /Fo:output\ /I C:\glew-2.1.0\include src\assimpModelLoader.cpp
@REM cl.exe /c /EHsc /Fo:output\ /I C:\glew-2.1.0\include src\objects.cpp
@REM cl.exe /c /EHsc /Fo:output\ /I C:\glew-2.1.0\include src\fbo.cpp
@REM cl.exe /c /EHsc /Fo:output\ /I C:\glew-2.1.0\include src\common.cpp
@REM cl.exe /c /EHsc /Fo:output\ /I C:\glew-2.1.0\include src\effects\guassianBlur.cpp
@REM cl.exe /c /EHsc /Fo:output\ /I C:\glew-2.1.0\include src\effects\exposure.cpp
@REM cl.exe /c /EHsc /Fo:output\ /I C:\glew-2.1.0\include src\effects\terrain.cpp
@REM cl.exe /c /EHsc /Fo:output\ /I C:\glew-2.1.0\include src\effects\textureBlending.cpp
@REM cl.exe /c /EHsc /Fo:output\ /I C:\glew-2.1.0\include src\effects\cubemap.cpp
@REM cl.exe /c /EHsc /Fo:output\ /I C:\glew-2.1.0\include src\effects\godrays.cpp
@REM cl.exe /c /EHsc /Fo:output\ /I C:\glew-2.1.0\include src\scenes\introScene.cpp
cl.exe /c /EHsc /Fo:output\ /I C:\glew-2.1.0\include src\effects\water.cpp
cl.exe /c /EHsc /Fo:output\ /I C:\glew-2.1.0\include src\scenes\scene2.cpp
@REM cl.exe /c /EHsc /Fo:output\ /I C:\glew-2.1.0\include src\scenes\scene1.cpp
@REM cl.exe /c /EHsc /Fo:output\ /I C:\glew-2.1.0\include src\scenes\testScene.cpp
@REM cl.exe /c /EHsc /Fo:output\ /I C:\glew-2.1.0\include src\imgui_manager.cpp
cl.exe /c /EHsc /Fo:output\ /I C:\glew-2.1.0\include src\camera.cpp
@REM cl.exe /c /EHsc /Fo:output\ /I C:\glew-2.1.0\include src\spline.cpp
@REM cl.exe /c /EHsc /Fo:output\ /I C:\glew-2.1.0\include src\OGL.cpp
@REM cl.exe /c /EHsc /Fo:output\ /I C:\glew-2.1.0\include src\effects\terrain.cpp

link.exe /OUT:OGL.exe output\*.obj output\OGL.res /LIBPATH:C:\glew-2.1.0\lib\Release\x64 glew32.lib libs\Sphere.lib assimp-vc143-mt.lib /SUBSYSTEM:WINDOWS 

@REM cl.exe /c /EHsc /Fo:output\ /I C:\glew-2.1.0\include src\scenes\testScene.cpp src\fileHandler.cpp src\objects.cpp src\camera.cpp 
@REM rc.exe /fo output\OGL.res src\OGL.rc 
@REM link.exe /OUT:OGL.exe output\*.obj output\OGL.res /LIBPATH:C:\glew-2.1.0\lib\Release\x64 glew32.lib /SUBSYSTEM:WINDOWS 
