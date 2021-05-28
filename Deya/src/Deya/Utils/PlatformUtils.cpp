#include "dypch.h"
#include "Deya/Utils/PlatformUtils.h"

#include <nfd.h>

#include <GLFW/glfw3.h> // ? needed

namespace Deya
{
    std::string FileDialogs::OpenFile(const char* filter)
    {
        nfdchar_t *outPath = NULL;
        nfdresult_t result = NFD_OpenDialog( filter, NULL, &outPath );
            
        if ( result == NFD_OKAY ) 
        {
            DY_CORE_TRACE("Opened path: {0}", outPath);
            return outPath;
        }
        else if ( result == NFD_CANCEL ) 
        {
            DY_CORE_TRACE("User pressed cancel.");
        }
        else 
        {
            DY_CORE_ERROR("File open error: {0}", NFD_GetError());
        }

        return std::string();
    }

    std::string FileDialogs::SaveFile(const char* filter)
    {
        nfdchar_t *outPath = NULL;
        nfdresult_t result = NFD_SaveDialog( filter, NULL, &outPath );
            
        if ( result == NFD_OKAY ) 
        {
            DY_CORE_TRACE("Saved to path: {0}", outPath);
            return outPath;
        }
        else if ( result == NFD_CANCEL ) 
        {
            DY_CORE_TRACE("User pressed cancel.");
        }
        else 
        {
            DY_CORE_ERROR("File open error: {0}", NFD_GetError());
        }

        return std::string();
    }
}