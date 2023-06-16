set CurrentPath=%~dp0


set env=%CurrentPath%..\..\tools\
set ProjectName=VolumeRender


set "Path=%env%mingw1120_64\bin;%env%ninja;%env%cmake-3.24.0-rc1-windows-x86_64\bin;"


set CMAKE=%env%cmake-3.24.0-rc1-windows-x86_64\bin\cmake.exe
set PrjDIR=%env%..\lib\%ProjectName%\prj
set SrcDIR=%env%..\lib\%ProjectName%\src

::cmake


cmake -S %SrcDIR% -B %PrjDIR% -G "MinGW Makefiles"


::build
cmake  --build %PrjDIR%   --config Release --parallel 1


