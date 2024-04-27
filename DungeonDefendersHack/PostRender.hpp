#include "stdafx.h"
#include "utils.hpp"

namespace Render {

    
    static inline UTexture2D* woodenFrameTexture = nullptr;
    static bool texturesLoaded = false;

    void LoadTextures() {
        static bool allTexturesLoaded = true; 

        if (globals::OriginalStaticLoadObject != nullptr && globals::Engine != nullptr)
        {
            if (woodenFrameTexture == nullptr) {
                woodenFrameTexture = (UTexture2D*)globals::OriginalStaticLoadObject(UTexture2D::StaticClass(), nullptr, L"DunDefUI.Textures.GenericPanelBackground_Wooden", nullptr, 0, 0, true);
                if (woodenFrameTexture == nullptr) {
                    allTexturesLoaded = false; 
                }
            }
        }
        else {
            allTexturesLoaded = false; 
        }

        texturesLoaded = allTexturesLoaded; 
    }

    inline void PostRender(UCanvas* canvas) {
        if (!canvas) return;

        if (!texturesLoaded) {
            LoadTextures();
            return;
        }

        canvas->SetPos(0.0f, 0.0f);
        //canvas->DrawTexture(woodenFrameTexture, 1.0f);
    }
}