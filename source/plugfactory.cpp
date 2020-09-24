#include "public.sdk/source/main/pluginfactory.h"

#include "../include/plugcontroller.h"	// for createInstance
#include "../include/plugprocessor.h"	// for createInstance
#include "../include/plugids.h"			// for uids
#include "../include/version.h"			// for version and naming

#include "../include/log.h"

#define stringSubCategory	"Fx" // "Fx|Analyzer"	// Subcategory for this plug-in (to be changed if needed, see PlugType in ivstaudioprocessor.h)

BEGIN_FACTORY_DEF (stringVendorName, stringVendorWeb, stringVendorEmail)

	DEF_CLASS2 (INLINE_UID_FROM_FUID(Steinberg::HelloWorld::MyProcessorUID),
				PClassInfo::kManyInstances,	// cardinality  
				kVstAudioEffectClass,	// the component category (do not changed this)
				stringPluginName,		// here the plug-in name (to be changed)
				Vst::kDistributable,	// means that component and controller could be distributed on different computers
				stringSubCategory,		// Subcategory for this plug-in (to be changed)
				FULL_VERSION_STR,		// Plug-in version (to be changed)
				kVstVersionString,		// the VST 3 SDK version (do not changed this, use always this define)
				Steinberg::HelloWorld::PlugProcessor::createInstance)	// function pointer called when this component should be instantiated

	DEF_CLASS2 (INLINE_UID_FROM_FUID(Steinberg::HelloWorld::MyControllerUID),
				PClassInfo::kManyInstances,  // cardinality   
				kVstComponentControllerClass,// the Controller category (do not changed this)
				stringPluginName "Controller",	// controller name (could be the same than component name)
				0,						// not used here
				"",						// not used here
				FULL_VERSION_STR,		// Plug-in version (to be changed)
				kVstVersionString,		// the VST 3 SDK version (do not changed this, use always this define)
				Steinberg::HelloWorld::PlugController::createInstance)// function pointer called when this component should be instantiated

END_FACTORY

//------------------------------------------------------------------------
//  Module init/exit
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// called after library was loaded
bool InitModule ()
{
	new Steinberg::HelloWorld::Log();
	return true;
}

//------------------------------------------------------------------------
// called after library is unloaded
bool DeinitModule ()
{
	delete Steinberg::HelloWorld::Log::io;
	return true;
}
