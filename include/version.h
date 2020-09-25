#pragma once

#include "pluginterfaces/base/fplatform.h"

#define MAJOR_VERSION_INT 1
#define SUB_VERSION_INT 0
#define RELEASE_NUMBER_INT 0
#define BUILD_NUMBER_INT 2

// HERE you have to define your plug-in, company name, email and web
#ifdef PLUGIN_NAME
#define stringPluginName PLUGIN_NAME
#else
#define stringPluginName "helloworld"
#endif

#define stringOriginalFilename PLUGIN_NAME ".vst3"

#ifdef VENDOR
#define stringVendorName VENDOR "\0"
#else
#define stringVendorName "Steinberg Media Technologies\0"
#endif

#ifdef URL
#define stringVendorWeb URL
#else
#define stringVendorWeb "http://www.steinberg.net"
#endif

#ifdef MAIL
#define stringVendorEmail MAIL
#else
#define stringVendorEmail "mailto:info@steinberg.de"
#endif

#ifdef COPYRIGHT
#define stringLegalCopyright COPYRIGHT
#else
#define stringLegalCopyright "© 2020 " stringVendorName
#endif

#ifdef TRADEMARK
#define stringLegalTrademarks TRADEMARK
#else
#define stringLegalTrademarks "VST is a trademark of Steinberg Media Technologies GmbH"
#endif

// -----------------------------------------------------------------
#if SMTG_PLATFORM_64
#define stringFileDescription stringPluginName " VST3-SDK (64Bit)"
#else
#define stringFileDescription stringPluginName " VST3-SDK"
#endif

#define __NUM_TO_STR(x) #x
#define ___NUM_TO_STR(x) __NUM_TO_STR(x)

#define MAJOR_VERSION_STR ___NUM_TO_STR(MAJOR_VERSION_INT)
#define SUB_VERSION_STR ___NUM_TO_STR(SUB_VERSION_INT)
#define RELEASE_NUMBER_STR ___NUM_TO_STR(RELEASE_NUMBER_INT)
#define BUILD_NUMBER_STR ___NUM_TO_STR(BUILD_NUMBER_INT)

// Version with build number (example "1.0.3.342")
#define FULL_VERSION_STR MAJOR_VERSION_STR "." SUB_VERSION_STR "." RELEASE_NUMBER_STR "." BUILD_NUMBER_STR

// Version without build number (example "1.0.3")
#define VERSION_STR MAJOR_VERSION_STR "." SUB_VERSION_STR "." RELEASE_NUMBER_STR
