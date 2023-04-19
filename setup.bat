call %~dp0\Alice\lib\vcpkg\bootstrap-vcpkg.bat

cd %~dp0\Alice\lib\vcpkg

vcpkg.exe install imgui[core,dx12-binding,win32-binding]:x64-windows-static 
vcpkg.exe install directxtex:x64-windows-static
vcpkg.exe install assimp:x64-windows-static 
vcpkg.exe install nlohmann-json:x64-windows-static

pause