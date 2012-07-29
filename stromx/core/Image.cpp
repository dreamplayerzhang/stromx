/* 
 *  Copyright 2012 Matthias Fuchs
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */

#include "Image.h"

namespace stromx
{
    namespace core
    {
        const int Image::numChannels(const PixelType pixelType)
        {
            switch(pixelType)
            {
            case core::Image::NONE:
            case core::Image::MONO_8:
            case core::Image::MONO_16:
            case core::Image::BAYERBG_8:
            case core::Image::BAYERGB_8:
            case core::Image::BAYERBG_16:
            case core::Image::BAYERGB_16:
                return 1;
            case core::Image::RGB_24:
            case core::Image::BGR_24:
            case core::Image::RGB_48:
            case core::Image::BGR_48:
                return 3;
            default:
                throw core::WrongArgument("Unknown pixel type.");    
            }
        }
        
        const int Image::depth(const PixelType pixelType)
        {
            switch(pixelType)
            {
            case core::Image::NONE:
            case core::Image::MONO_8:
            case core::Image::RGB_24:
            case core::Image::BGR_24:
            case core::Image::BAYERBG_8:
            case core::Image::BAYERGB_8:
                return 1;
            case core::Image::MONO_16:
            case core::Image::RGB_48:
            case core::Image::BGR_48:
            case core::Image::BAYERBG_16:
            case core::Image::BAYERGB_16:
                return 2;
            default:
                throw core::WrongArgument("Unknown pixel type.");    
            }
        }
        
        const core::DataVariant Image::dataVariantFromPixelType(const core::Image::PixelType pixelType)
        {
            switch(pixelType)
            {
            case core::Image::NONE:
                return core::DataVariant::IMAGE;
            case core::Image::MONO_8:
                return core::DataVariant::MONO_8_IMAGE;
            case core::Image::BAYERBG_8:
                return core::DataVariant::BAYERBG_8_IMAGE;
            case core::Image::BAYERGB_8:
                return core::DataVariant::BAYERGB_8_IMAGE;
            case core::Image::RGB_24:
                return core::DataVariant::RGB_24_IMAGE;
            case core::Image::BGR_24:
                return core::DataVariant::BGR_24_IMAGE;
            default:
                throw core::WrongArgument("Unknown pixel type.");  
            }
        }
    }
}
