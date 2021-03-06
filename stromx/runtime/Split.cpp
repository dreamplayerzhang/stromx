/* 
 *  Copyright 2015 Matthias Fuchs
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

#include "stromx/runtime/Split.h"

#include <boost/assert.hpp>

#include "stromx/runtime/DataProvider.h"
#include "stromx/runtime/Id2DataComposite.h"
#include "stromx/runtime/Id2DataPair.h"
#include "stromx/runtime/List.h"
#include "stromx/runtime/Locale.h"
#include "stromx/runtime/NumericParameter.h"
#include "stromx/runtime/OperatorException.h"
#include "stromx/runtime/RecycleAccess.h"
#include "stromx/runtime/Variant.h"

namespace 
{
    const static unsigned int ITEM_THREAD = 0;
    const static unsigned int LIST_THREAD = 1;
}

namespace stromx
{
    namespace runtime
    {
        const std::string Split::TYPE("Split");
        const std::string Split::PACKAGE(STROMX_RUNTIME_PACKAGE_NAME);
        const Version Split::VERSION(STROMX_RUNTIME_VERSION_MAJOR, STROMX_RUNTIME_VERSION_MINOR, STROMX_RUNTIME_VERSION_PATCH);
        
        Split::Split()
          : OperatorKernel(TYPE, PACKAGE, VERSION, setupInputs(), setupOutputs())
        {
        }
        
        void Split::setParameter(unsigned int id, const runtime::Data& /*value*/)
        {
            throw WrongParameterId(id, *this);
        }

        const DataRef Split::getParameter(const unsigned int id) const
        {
            throw WrongParameterId(id, *this);
        }
        
        void Split::activate()
        {
            BOOST_ASSERT(m_storedItems.size() == 0);
        }
        
        void Split::deactivate()
        {
            m_storedItems.clear();
        }
        
        void Split::execute(DataProvider& provider)
        {
            if (m_storedItems.size() == 0)
            {
                RecycleAccess recycle;
                {
                    Id2DataPair input(INPUT);
                    provider.receiveInputData(input);
                    recycle.add(input.data());
                }
                Data* data = recycle.get();
                List* list = data_cast<List>(data);
                
                if (list == 0)
                    throw InputError(INPUT, *this, "Input data must be a 'List' object.");
                
                for (std::vector<Data*>::iterator iter = list->content().begin();
                     iter != list->content().end(); ++iter)
                {
                    m_storedItems.push_back(DataContainer(*iter));
                }
                
                uint64_t size = list->content().size();
                list->content().clear();
                delete list;
                
                DataContainer outNumItems(new UInt64(size));
                Id2DataPair dataMapper(OUTPUT_NUM_ITEMS, outNumItems);
                
                provider.sendOutputData(dataMapper);
            }
            
            if (m_storedItems.size() != 0)
            {
                DataContainer outData = m_storedItems.front();
                Id2DataPair dataMapper(OUTPUT_DATA, outData);
                m_storedItems.pop_front();
                
                provider.sendOutputData(dataMapper);
            }
        }
        
        const std::vector<const Input*> Split::setupInputs()
        {
            std::vector<const Input*> inputs;
            Input* input = new Input(INPUT, Variant::LIST);
            input->setTitle(L_("List"));
            input->setOperatorThread(LIST_THREAD);
            inputs.push_back(input);
            
            return inputs;
        }
        
        const std::vector<const Output*> Split::setupOutputs() const
        {
            std::vector<const Output*> outputs;
            
            Output* data = new Output(OUTPUT_DATA, Variant::DATA);
            data->setTitle(L_("List item"));
            data->setOperatorThread(ITEM_THREAD);
            outputs.push_back(data);
            
            Output* index = new Output(OUTPUT_NUM_ITEMS, Variant::UINT_64);
            index->setTitle(L_("Number of list items"));
            index->setOperatorThread(LIST_THREAD);
            outputs.push_back(index);
            
            return outputs;
        }
    }
}
