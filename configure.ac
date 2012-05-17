AC_PREREQ(2.60)

AC_INIT([npsimple], [000])
AC_DEFINE([PACKAGE_VERSION_RESOURCES], [0,0,0,0], [resource version for windows])

AC_CONFIG_AUX_DIR([.])
AM_CONFIG_HEADER([config.h])
AC_CONFIG_SRCDIR([npsimple.c])
AC_CONFIG_MACRO_DIR([m4])
AM_INIT_AUTOMAKE
AC_CANONICAL_HOST
AC_USE_SYSTEM_EXTENSIONS

AC_ARG_ENABLE(
	[so-suffix],
	[AS_HELP_STRING([--enable-so-suffix], [add host so suffix @<:@default=no@:>@])],
	,
	[enable_so_suffix="no"]
)

AC_ARG_ENABLE(
	[pedantic],
	[AS_HELP_STRING([--enable-pedantic], [enable pedantic compiler warnings, will not generate a working executable (debugging option) @<:@default=no@:>@])],
	,
	[enable_pedantic="no"]
)

AC_ARG_WITH(
	[sdk],
	[AS_HELP_STRING([--with-sdk], [sdk type: npapi-sdk, xulrunner, webkit-darwin, webkit-winmobile, android @<:@default=npapi-sdk@:>@])],
	,
	[with_sdk="npapi-sdk"]
)

AC_ARG_WITH(
	[pluginsdir],
	[AS_HELP_STRING([--with-plugindirs], [plugins directory @<:@default=LIBDIR/nsbrowser/plugins@:>@])],
	[pluginsdir="${with_pluginsdir}"],
	[
		case "$host" in
			*-darwin-*) pluginsdir="\$(libdir)/Internet\\ Plug-Ins" ;;
			*) pluginsdir="\${libdir}/nsbrowser/plugins" ;;
		esac
	]
)

case "$host" in
	*-darwin-*)
		DARWIN=yes
		;;
	*-mingw*)
		CPPFLAGS="${CPPFLAGS} -DWIN32_LEAN_AND_MEAN"
		WIN32=yes
		;;
esac

PKG_PROG_PKG_CONFIG
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_SED
AC_PROG_MAKE_SET

#
# Libtool
#
ifdef(
	[LT_INIT],
	[
		LT_INIT([win32-dll])
		LT_LANG([Windows Resource])
	],
	[
		AC_LIBTOOL_WIN32_DLL
		AC_LIBTOOL_RC
		AC_PROG_LIBTOOL
	]
)

AC_ARG_VAR([RES], [path to Res utility])
AC_CHECK_PROGS([RES], [Res])

AC_CHECK_HEADERS([ \
	stdbool.h \
])

PKG_CHECK_MODULES(
	[NPAPI_SDK],
	[npapi-sdk],
	,
	[AC_MSG_ERROR([npapi-sdk not found])]
)

case ${with_sdk} in
	npapi-sdk)
		AC_DEFINE_UNQUOTED([USE_SDK_NPAPI_SDK], [1], [use the npapi sdk])
		;;
	xulrunner)
		AC_DEFINE_UNQUOTED([USE_SDK_XULRUNNER], [1], [use the xulrunner sdk])
		;;
	webkit-darwin)
		AC_DEFINE_UNQUOTED([USE_SDK_WEBKIT_DARWIN], [1], [use the webkit darwin sdk])
		;;
	webkit-winmobile)
		AC_DEFINE_UNQUOTED([USE_SDK_WEBKIT_WINMOBILE], [1], [use the webkit windows mobile sdk])
		;;
	android)
		AC_DEFINE_UNQUOTED([USE_SDK_ANDROID], [1], [use the android sdk])
		;;
	*)
		AC_MSG_ERROR([invalid sdk specified])
		;;
esac

if test "${enable_so_suffix}" = "yes"; then
	so_suffix="-${host}"
fi

if test "${enable_pedantic}" = "yes"; then
	CFLAGS="${CFLAGS} -pedantic -Wall -Wextra"
	test "${WIN32}" != "yes" && CFLAGS="${CFLAGS} -ansi"
fi

AC_SUBST([so_suffix])
AC_SUBST([pluginsdir])

AM_CONDITIONAL([WIN32], [test "${WIN32}" = "yes"])
AM_CONDITIONAL([DARWIN], [test "${DARWIN}" = "yes"])

AC_CONFIG_FILES([
	Makefile
])
AC_OUTPUT
