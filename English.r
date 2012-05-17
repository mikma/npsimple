#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <CoreServices/CoreServices.r>
#include "common.h"

// Plugin info
resource 'STR#' (126) { {
    "Tests npsimple()",
    "npsimple Test Plug-In"
} };

// MIME Type descriptions
resource 'STR#' (127) { {
    "npsimple test"
} };

// MIME Types
resource 'STR#' (128) { {
    FOO_MIMETYPE
} };
