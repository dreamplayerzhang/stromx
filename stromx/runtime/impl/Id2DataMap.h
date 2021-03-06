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

#ifndef STROMX_RUNTIME_IMPL_ID2DATAMAP_H
#define STROMX_RUNTIME_IMPL_ID2DATAMAP_H

#include <map>
#include <set>
#include <vector>

namespace stromx
{
    namespace runtime
    {
        class DataContainer;
        class Input;
        class Output;
        class Parameter;
        
        namespace impl
        {
            class Id2DataMapObserver
            {
             public:
                virtual void observe(const unsigned int id, const DataContainer & oldData, const DataContainer & newData) const = 0;
            };
           
            class Id2DataMap
            {
            public:
                Id2DataMap();
                
                void initialize(const std::vector<const Input*> & descriptions,
                                const std::vector<const Parameter*> & parameters = std::vector<const Parameter*>());
                void initialize(const std::vector<const Output*> & descriptions,
                                const std::vector<const Parameter*> & parameters = std::vector<const Parameter*>());
                const DataContainer & get(const unsigned int id) const;
                void set(const unsigned int id, const DataContainer & data);
                void clear();
                bool empty() const;
                void setObserver(const Id2DataMapObserver* const observer);
                bool canBeSet(const unsigned int id) const;
                bool mustBeReset(const unsigned int id) const;
                
            private:
                std::map<unsigned int, DataContainer> m_map;
                std::set<unsigned int> m_persistentParameters;
                const Id2DataMapObserver* m_observer;
            };
        }
    }
}

#endif // STROMX_RUNTIME_IMPL_ID2DATAMAP_H
