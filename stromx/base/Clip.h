/* 
*  Copyright 2011 Matthias Fuchs
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

#ifndef STROMX_BASE_CLIP_H
#define STROMX_BASE_CLIP_H

#include "Config.h"
#include <stromx/core/OperatorKernel.h>
#include <stromx/core/Primitive.h>
#include <stromx/core/RecycleAccess.h>

namespace stromx
{
    namespace base
    {
        /** \brief Clips an image to a rectangular region. */
        class STROMX_BASE_API Clip : public core::OperatorKernel
        {
        public:
            enum InputId
            {
                INPUT
            };
            
            enum OutputId
            {
                OUTPUT
            };
            
            enum ParameterId
            {
                TOP,
                LEFT,
                WIDTH,
                HEIGHT,
                NUM_PARAMS
            };
            
            Clip();
            
            virtual OperatorKernel* clone() const { return new Clip; }
            virtual void setParameter(const unsigned int id, const core::Data& value);
            virtual const core::DataRef getParameter(const unsigned int id) const;
            virtual void execute(core::DataProvider& provider);
            
        private:
            static const std::vector<const core::Description*> setupInputs();
            static const std::vector<const core::Description*> setupOutputs();
            static const std::vector<const core::Parameter*> setupParameters();
            
            static const std::string TYPE;
            static const std::string PACKAGE;
            static const core::Version VERSION;
            
            void adjustClipRegion(const unsigned int destWidth, const unsigned int destHeight,
                                unsigned int & left, unsigned int & top,
                                unsigned int & width, unsigned int & height);                           
            
            core::UInt32 m_top;
            core::UInt32 m_left;
            core::UInt32 m_width;
            core::UInt32 m_height;
        };
    }
}

#endif // STROMX_BASE_CLIP_H