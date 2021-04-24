@echo off

for %%f in (../shaders/*.vert) do (
	glslc.exe -fshader-stage="vertex" ../shaders/%%f -o ../spirv/%%~nf.vspv
)

for %%f in (../shaders/*.tesc) do (
	glslc.exe -fshader-stage="tesscontrol" ../shaders/%%f -o ../spirv/%%~nf.tcspv
)

for %%f in (../shaders/*.tese) do (
	glslc.exe -fshader-stage="tesseval" ../shaders/%%f -o ../spirv/%%~nf.tespv
)

for %%f in (../shaders/*.geom) do (
	glslc.exe -fshader-stage="geometry" ../shaders/%%f -o ../spirv/%%~nf.gspv
)

for %%f in (../shaders/*.frag) do (
	glslc.exe -fshader-stage="fragment" ../shaders/%%f -o ../spirv/%%~nf.fspv
)

for %%f in (../shaders/*.comp) do (
	glslc.exe -fshader-stage="compute" ../shaders/%%f -o ../spirv/%%~nf.cspv
)
