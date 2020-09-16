$ENV:Path += ";C:\Users\tal\AppData\Local\bin\cmake-3.18.2-win64-x64\bin"
Set-Location work
cmake C:\Users\tal\.vst\VST3_SDK -G"Visual Studio 16 2019" `
		-DSMTG_MYPLUGINS_SRC_PATH=".." `
		-DSMTG_ADD_VSTGUI=TRUE `
		-DSMTG_ADD_VST3_HOSTING_SAMPLES=FALSE `
		-DSMTG_ADD_VST3_PLUGINS_SAMPLES=FALSE

cmake --build .

# cmake --build . --config Release
