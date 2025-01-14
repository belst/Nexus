#ifndef QUICKACCESS_FUNCDEFS_H
#define QUICKACCESS_FUNCDEFS_H

#include "FuncDefs.h"

typedef void (*QUICKACCESS_ADDSHORTCUT)				(const char* aIdentifier, const char* aTextureIdentifier, const char* aTextureHoverIdentifier, const char* aKeybindIdentifier, const char* aTooltipText);
typedef void (*QUICKACCESS_ADDSIMPLE)				(const char* aIdentifier, GUI_RENDER aShortcutRenderCallback);
typedef void (*QUICKACCESS_GENERIC)					(const char* aIdentifier);

#endif