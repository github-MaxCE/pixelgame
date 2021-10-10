#pragma once
#include "olcPixelGameEngine.h"
#include "worldmanager.h"
#include "rapidxml.h"
#include "rapidxml_utils.h"
#include "rapidxml_print.h"
#include "olcPGEX_FrostUI.h"
extern "C"
{
	#include "stb_image_write.h"
}
#include "filemanager.h"
#include <vector>
#include <algorithm>

constexpr unsigned int str2int(const char* data, size_t const size) noexcept
{
    int hash = 5381;

    for (const char* c = data; c < data + size; ++c)
        hash = ((hash << 5) + hash) + (unsigned char)*c;

    return hash;
}