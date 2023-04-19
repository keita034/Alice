cd %~dp0\Alice\lib\vcpkg

vcpkg.exe remove imgui[core,dx12-binding,win32-binding]:x64-windows-static 
vcpkg.exe remove directxtex:x64-windows-static
vcpkg.exe remove assimp:x64-windows-static 

pause