// ----------------------------------------------------------------------------
//	We are using Semantic Versioning 2.0.0
//
//	That means in our case a version number MAJOR.MINOR.PATCH.BUILD, increment the:
//	1. MAJOR version when we make incompatible API/Protokoll changes
//	2. MINOR version when we add functionality in a mostly backwards compatible manner
//	3. PATCH version when we make backwards compatible bug fixes
//	4. BUILD version when ever a new build has been made
//
//	In addition to the above:
//	1. Odd MINOR versions represent an development version
//	2. Even MINOR versions represent an release/stable version
// ----------------------------------------------------------------------------


// defines global VERSION_MAJOR and VERSION_MINOR
#include "Common/version.h"

#define VERSION_PATCH 0
#define VERSION_BUILD 104 // will automatically incremented by build_inc
