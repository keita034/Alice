call %~dp0\Alice\lib\vcpkg\bootstrap-vcpkg.bat

cd %~dp0\Alice\lib\vcpkg

vcpkg.exe install imgui[core,dx12-binding,win32-binding]:x64-windows 
vcpkg.exe install imgui[core,dx12-binding,win32-binding]:x64-windows-static 
vcpkg.exe install directxtk:x64-windows
vcpkg.exe install directxtk:x64-windows-static
vcpkg.exe install assimp:x64-windows
vcpkg.exe install assimp:x64-windows-static 

pause

del %0
