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

#include <boost/assert.hpp>
#include "stromx/runtime/Try.h"

namespace stromx
{
    namespace runtime
    {
        bool Try::trySet(const runtime::impl::Id2DataMap&) const
        {
            return true;
        }

        bool Try::tryGet(const runtime::impl::Id2DataMap&) const
        {
            return true;
        }

        void Try::set(impl::Id2DataMap& id2DataMap) const
        {
            if(m_mapper.trySet(id2DataMap))
                m_mapper.set(id2DataMap);
        }

        void Try::get(impl::Id2DataMap& id2DataMap) const
        {
            if(m_mapper.tryGet(id2DataMap))
                m_mapper.get(id2DataMap);
        }
    }
}
