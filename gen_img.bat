set index=0
set folder=images

:addIndex

set fileName=%folder%/image%index%.ppm
if exist %fileName% (
    set /a index+=1
    goto addIndex
)

C:\Users\Eric\UNITEMP\RayTracing\out\build\x64-release\RayTracing.exe > %fileName%