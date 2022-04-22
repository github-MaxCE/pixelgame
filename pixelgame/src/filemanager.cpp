#include "filemanager.h"

#include <stdio.h>
#if defined(_MSC_VER) && !defined(_WIN32_WCE) && !defined(__S3E__)
#include <direct.h>
#endif
#ifdef _WIN32_WCE
#include <windows.h> // For GetModuleFileName()
#endif

#if defined(__S3E__) || defined(__APPLE__) || defined(__GNUC__)
#include <unistd.h> // For getcwd()
#endif

namespace max
{
	std::string apppath()
	{
		char buffer[1024];
#if defined(_MSC_VER) || defined(_WIN32)
#ifdef _WIN32_WCE
		static TCHAR apppath[MAX_PATH] = TEXT("");
		if (!apppath[0])
		{
			GetModuleFileName(NULL, apppath, MAX_PATH);

			int appLen = _tcslen(apppath);

			// Look for the last backslash in the path, which would be the end
			// of the path itself and the start of the filename.  We only want
			// the path part of the exe's full-path filename
			// Safety is that we make sure not to walk off the front of the
			// array (in case the path is nothing more than a filename)
			while (appLen > 1)
			{
				if (apppath[appLen - 1] == TEXT('\\'))
					break;
				appLen--;
			}

			// Terminate the string after the trailing backslash
			apppath[appLen] = TEXT('\0');
		}
#ifdef _UNICODE
		wcstombs(buffer, apppath, min(1024, wcslen(apppath) * sizeof(wchar_t)));
#else
		memcpy(buffer, apppath, min(1024, strlen(apppath)));
#endif

		return buffer;
#elif defined(__S3E__)
		// Marmalade uses its own portable C library
		return getcwd(buffer, (int)1024);
#elif _XBOX_VER >= 200
		// XBox 360 doesn't support the getcwd function, just use the root folder
		return "game:/";
#elif defined(_M_ARM)
		// TODO: How to determine current working dir on Windows Phone?
		return "";
#else
		return _getcwd(buffer, (int)1024);
#endif // _MSC_VER
#elif defined(__APPLE__) || defined(__linux__)
		return getcwd(buffer, 1024);
#else
		return "";
#endif
	}

	std::string mappath()
	{
		return apppath() + "/pxg/map/";
	}

	std::string matpath()
	{
		return apppath() + "/pxg/materials/";
	}

	std::string scriptpath()
	{
		return apppath() + "/pxg/scripts/";
	}
}