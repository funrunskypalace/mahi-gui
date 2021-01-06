#pragma once

#include <memory>

#include "framework/common/data_struct.h"
#include "framework/common/data_type.h"
#include "framework/common/errno.h"

#ifndef PURE
#define PURE = 0
#endif

#ifdef __linux__
#define GWSDK_API __attribute__((visibility("default")))
#else
#ifdef GWSDK_STATIC
#define GWSDK_API
#else
#if defined(skypalace_framework_EXPORTS)
#define GWSDK_API __declspec(dllexport)
#else
#define GWSDK_API __declspec(dllimport)
#endif
#endif
#endif

#define GWSDK_COMDAT __declspec(selectany)

extern "C" GWSDK_API void greatwall_init();

#include "framework/common/logger.h"
