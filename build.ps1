. ".\config.ps1"

$ENV:Path += ";C:\Users\tal\AppData\Local\bin\cmake-3.18.2-win64-x64\bin"
$GENERATOR = '-G"Visual Studio 16 2019"'
$ARCH = "x86_64-win"

if (Test-Path ".\machine_config.ps1") {
	. ".\machine_config.ps1"
}


Set-Location work
cmake C:\Users\tal\.vst\VST3_SDK ${GENERATOR} `
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
bin\Debug\validator.exe VST3\Debug\${CONFIG_PLUGIN_NAME}.vst3\Contents\${ARCH}\${CONFIG_PLUGIN_NAME}.vst3

# cmake --build . --config Release
# bin\Release\validator.exe VST3\Release\${CONFIG_PLUGIN_NAME}.vst3\Contents\x86_64-win\${CONFIG_PLUGIN_NAME}.vst3
