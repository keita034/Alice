chcp 65001
@echo off

echo バッチ処理を開始します

echo フォルダ生成開始
cd %~dp0Engine\Alice\lib\
md lib
md include
cd %~dp0Engine\Alice\lib\lib
md debug
echo フォルダ生成終了

echo libファイル生成するためのvcpkg生成開始
call %~dp0Engine\Alice\lib\vcpkg\bootstrap-vcpkg.bat
echo libファイル生成するためのvcpkg生成終了

echo libファイル生成開始
cd %~dp0Engine\Alice\lib\vcpkg

START /wait vcpkg.exe install imgui[core,dx12-binding,win32-binding]:x64-windows-static 
echo imgui終了

START /wait vcpkg.exe install directxtex:x64-windows-static
echo directxTex終了

START /wait vcpkg.exe install assimp:x64-windows-static 
echo assimp終了

START /wait vcpkg.exe install nlohmann-json:x64-windows-static
echo nlohmann-json終了

START /wait vcpkg.exe install physx:x64-windows-static
echo PhysX終了

START /wait vcpkg.exe install joltphysics:x64-windows-static
echo JoltPhysics終了

echo libファイル生成終了

echo 生成ファイル移動開始
cd %~dp0Engine\Alice\lib\vcpkg\installed\x64-windows-static\lib
copy *.lib %~dp0Engine\Alice\lib\lib

cd %~dp0Engine\Alice\lib\vcpkg\installed\x64-windows-static\debug\lib
copy *.lib %~dp0Engine\Alice\lib\lib\debug

cd %~dp0Engine\Alice\lib\vcpkg\installed\x64-windows-static
xcopy /e include %~dp0Engine\Alice\lib\include\
echo 生成ファイル移動終了

echo バッチ処理が終了しました

pause