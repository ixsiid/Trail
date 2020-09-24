. ".\config.ps1"

$ENV:Path += ";C:\Users\tal\AppData\Local\bin\cmake-3.18.3-win64-x64\bin"
Set-Location work
cmake C:\Users\tal\.vst\VST3_SDK -G"Visual Studio 15 2017" `
		-DPLUGIN_NAME="${CONFIG_PLUGIN_NAME}" `
		-DVENDOR="${CONFIG_VENDOR}" `
		-DURL="${CONFIG_URL}" `
		-DMAIL="${CONFIG_MAIL}" `
		-DCOPYRIGHT="${CONFIG_COPYRIGHT}" `
		-DTRADEMARK="${CONFIG_TRADEMARK}" `
		-DSMTG_MYPLUGINS_SRC_PATH=".." `
		-DSMTG_ADD_VSTGUI=TRUE `
		-DSMTG_ADD_VST3_HOSTING_SAMPLES=FALSE `
		-DSMTG_ADD_VST3_PLUGINS_SAMPLES=FALSE

cmake --build .

bin\Debug\validator.exe VST3\Debug\${CONFIG_PLUGIN_NAME}.vst3\Contents\x86-win\${CONFIG_PLUGIN_NAME}.vst3

# cmake --build . --config Release
