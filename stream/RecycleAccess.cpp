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

#include "RecycleAccess.h"

namespace stream
{
    RecycleAccess::RecycleAccess(DataContainer data)
      : m_impl(new impl::RecycleAccessImpl(data))
    {
    }

    Data*const RecycleAccess::operator()()
    {
        if(! m_impl.get())
            return 0;
        
        return (*m_impl)();
    }

    Data*const RecycleAccess::operator()(const unsigned int timeout)
    {
        if(! m_impl.get())
            return 0;
        
        return (*m_impl)(timeout);
    }
    
    void RecycleAccess::add(DataContainer data)
    {
        if(! m_impl.get())
            m_impl = boost::shared_ptr<impl::RecycleAccessImpl>(new impl::RecycleAccessImpl(data));
        else
            m_impl->add(data);
    }
} 
