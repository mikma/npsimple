/* ***** BEGIN LICENSE BLOCK *****
 * (C)opyright 2008-2009 Aplix Corporation. anselm@aplixcorp.com
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 * ***** END LICENSE BLOCK ***** */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#if defined(XULRUNNER_SDK)
#include <npapi.h>
#include <npupp.h>
#include <npruntime.h>
#elif defined(ANDROID)

#undef HAVE_LONG_LONG
#include <jni.h>
#include <npapi.h>
#include <npfunctions.h>
#include <npruntime.h>
#define OSCALL
#define NPP_WRITE_TYPE (NPP_WriteProcPtr)
#define NPStringText UTF8Characters
#define NPStringLen  UTF8Length
extern JNIEnv *pluginJniEnv;

#undef STRINGN_TO_NPVARIANT
#define STRINGN_TO_NPVARIANT(_val, _len, _v)                                  \
NP_BEGIN_MACRO                                                                \
    (_v).type = NPVariantType_String;                                         \
    NPString str = { _val, (uint32_t)(_len) };                                \
    (_v).value.stringValue = str;                                             \
NP_END_MACRO

#elif defined(WEBKIT_DARWIN_SDK)

#include <Webkit/npapi.h>
#include <WebKit/npfunctions.h>
#include <WebKit/npruntime.h>
#define OSCALL

#elif defined(WEBKIT_WINMOBILE_SDK) /* WebKit SDK on Windows */

#ifndef PLATFORM
#define PLATFORM(x) defined(x)
#endif
#include <npfunctions.h>
#ifndef OSCALL
#define OSCALL WINAPI
#endif

#elif defined(NPAPI_SDK)

#include <npapi.h>
#include <npfunctions.h>
#include <npruntime.h>

#endif

#include "common.h"

static FILE *s_log;
static int s_loglevel = -1;
static NPObject        *so       = NULL;
static NPNetscapeFuncs *npnfuncs = NULL;
static NPP              inst     = NULL;

/* NPN */

static void logmsg(const char *format, ...) {
	if (s_loglevel == -1) {
		if (getenv("NPSIMPLE_LOG_LEVEL") != NULL) {
			s_loglevel = atoi(getenv("NPSIMPLE_LOG_LEVEL"));
		}
		else {
			s_loglevel = 0;
		}
	}
	if (s_loglevel != 0) {
		va_list args;

		if (s_log == NULL) {
			if (getenv("NPSIMPLE_LOG_FILE") != NULL) {
				s_log = fopen(getenv("NPSIMPLE_LOG_FILE"), "a");
			}
			if (s_log == NULL) {
				s_log = stderr;
			}
		}

		va_start(args, format);
		fprintf(s_log, "SPCHBHO-HELPER ");
		vfprintf(s_log, format, args);
		fputc('\n', s_log);
		fflush(s_log);
		va_end(args);
	}
}

static bool
hasMethod(NPObject* obj, NPIdentifier methodName) {
	logmsg("npsimple: hasMethod\n");
	return true;
}

static bool
invokeDefault(NPObject *obj, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	logmsg("npsimple: invokeDefault\n");
	result->type = NPVariantType_Int32;
	result->value.intValue = 42;
	return true;
}

static bool
invoke(NPObject* obj, NPIdentifier methodName, const NPVariant *args, uint32_t argCount, NPVariant *result) {
	char *name = npnfuncs->utf8fromidentifier(methodName);
	logmsg("npsimple: invoke\n");
	if(name) {
		if(!strcmp(name, "foo")) {
			logmsg("npsimple: invoke foo\n");
			return invokeDefault(obj, args, argCount, result);
		}
		else if(!strcmp(name, "callback")) {
			if(argCount == 1 && args[0].type == NPVariantType_Object) {
				static NPVariant v, r;
				const char kHello[] = "Hello";
				char *txt = (char *)npnfuncs->memalloc(strlen(kHello));

				logmsg("npsimple: invoke callback function\n");
				memcpy(txt, kHello, strlen(kHello));
				STRINGN_TO_NPVARIANT(txt, strlen(kHello), v);
				/* INT32_TO_NPVARIANT(42, v); */
				if(npnfuncs->invokeDefault(inst, NPVARIANT_TO_OBJECT(args[0]), &v, 1, &r))
					return invokeDefault(obj, args, argCount, result);
			}
		}
	}
	/* aim exception handling */
	npnfuncs->setexception(obj, "exception during invocation");
	return false;
}

static bool
hasProperty(NPObject *obj, NPIdentifier propertyName) {
	logmsg("npsimple: hasProperty\n");
	return false;
}

static bool
getProperty(NPObject *obj, NPIdentifier propertyName, NPVariant *result) {
	logmsg("npsimple: getProperty\n");
	return false;
}

static NPClass npcRefObject = {
	NP_CLASS_STRUCT_VERSION,
	NULL,
	NULL,
	NULL,
	hasMethod,
	invoke,
	invokeDefault,
	hasProperty,
	getProperty,
	NULL,
	NULL,
};

/* NPP */

static NPError
nevv(NPMIMEType pluginType, NPP instance, uint16_t mode, int16_t argc, char *argn[], char *argv[], NPSavedData *saved) {
	inst = instance;
	logmsg("npsimple: new\n");
	return NPERR_NO_ERROR;
}

static NPError
destroy(NPP instance, NPSavedData **save) {
	if(so)
		npnfuncs->releaseobject(so);
	so = NULL;
	logmsg("npsimple: destroy\n");
	return NPERR_NO_ERROR;
}

static NPError
getValue(NPP instance, NPPVariable variable, void *value) {
	inst = instance;
	switch(variable) {
	default:
		logmsg("npsimple: getvalue - default\n");
		return NPERR_GENERIC_ERROR;
	case NPPVpluginNameString:
		logmsg("npsimple: getvalue - name string\n");
		*((char **)value) = "AplixFooPlugin";
		break;
	case NPPVpluginDescriptionString:
		logmsg("npsimple: getvalue - description string\n");
		*((char **)value) = "<a href=\"http://www.aplix.co.jp/\">AplixFooPlugin</a> plugin.";
		break;
	case NPPVpluginScriptableNPObject:
		logmsg("npsimple: getvalue - scriptable object\n");
		if(!so)
			so = npnfuncs->createobject(instance, &npcRefObject);
		npnfuncs->retainobject(so);
		*(NPObject **)value = so;
		break;
#if defined(XULRUNNER_SDK) || defined(NPAPI_SDK)
	case NPPVpluginNeedsXEmbed:
		logmsg("npsimple: getvalue - xembed\n");
		*((NPBool *)value) = true;
		break;
#endif
	}
	return NPERR_NO_ERROR;
}

static NPError /* expected by Safari on Darwin */
handleEvent(NPP instance, void *ev) {
	inst = instance;
	logmsg("npsimple: handleEvent\n");
	return NPERR_NO_ERROR;
}

static NPError /* expected by Opera */
setWindow(NPP instance, NPWindow* pNPWindow) {
	inst = instance;
	logmsg("npsimple: setWindow\n");
	return NPERR_NO_ERROR;
}

/* EXPORT */
#ifdef __cplusplus
extern "C" {
#endif

NPError OSCALL
NP_GetEntryPoints(NPPluginFuncs *nppfuncs) {
	logmsg("npsimple: NP_GetEntryPoints\n");
	nppfuncs->version       = (NP_VERSION_MAJOR << 8) | NP_VERSION_MINOR;
	nppfuncs->newp          = nevv;
	nppfuncs->destroy       = destroy;
	nppfuncs->getvalue      = getValue;
	nppfuncs->event         = handleEvent;
	nppfuncs->setwindow     = setWindow;

	return NPERR_NO_ERROR;
}

#ifndef HIBYTE
#define HIBYTE(x) ((((uint32_t)(x)) & 0xff00) >> 8)
#endif

NPError OSCALL
NP_Initialize(NPNetscapeFuncs *npnf
#if defined(ANDROID)
			, NPPluginFuncs *nppfuncs, JNIEnv *env, jobject plugin_object
#elif !defined(_WINDOWS) && !defined(WEBKIT_DARWIN_SDK)
			, NPPluginFuncs *nppfuncs
#endif
			)
{
	logmsg("npsimple: NP_Initialize\n");
	if(npnf == NULL)
		return NPERR_INVALID_FUNCTABLE_ERROR;

	if(HIBYTE(npnf->version) > NP_VERSION_MAJOR)
		return NPERR_INCOMPATIBLE_VERSION_ERROR;

	npnfuncs = npnf;
#if !defined(_WINDOWS) && !defined(WEBKIT_DARWIN_SDK)
	NP_GetEntryPoints(nppfuncs);
#endif
	return NPERR_NO_ERROR;
}

NPError
OSCALL NP_Shutdown() {
	logmsg("npsimple: NP_Shutdown\n");
	return NPERR_NO_ERROR;
}

char *
NP_GetMIMEDescription(void) {
	logmsg("npsimple: NP_GetMIMEDescription\n");
	return FOO_MIMETYPE ":.foo:anselm@aplix.co.jp";
}

NPError OSCALL /* needs to be present for WebKit based browsers */
NP_GetValue(void *npp, NPPVariable variable, void *value) {
	inst = (NPP)npp;
	return getValue((NPP)npp, variable, value);
}

#ifdef __cplusplus
}
#endif
