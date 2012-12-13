/* 
*  Copyright 2011 Thomas Fidler
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

#ifndef STROMX_CORE_FACTORY_H
#define STROMX_CORE_FACTORY_H

#include <string>
#include <vector>
#include "Registry.h"

namespace stromx
{
    namespace core
    {
        class Operator;
        
        /** \brief %Factory of operator and data objects. */
        class STROMX_CORE_API Factory : public Registry
        {
        public:
            virtual ~Factory();
            
            virtual void registerOperator(const OperatorKernel* const op);  
            
            virtual void registerData(const Data* data);   
            
            /** Allocates and returns a new operator. */
            virtual Operator* newOperator(const std::string & package, const std::string & type) const;      
            
            /** Allocates and returns a new data object. */
            virtual Data* newData(const std::string & package, const std::string & type) const;
            
            /** Returns a list of the operators registered with the factory. */
            virtual const std::vector<const OperatorKernel*> & availableOperators() const { return m_operators; }
            
        private:
            std::vector<const OperatorKernel*> m_operators;
            std::vector<const Data*> m_dataTypes;
        };
    }
}

#endif // STROMX_CORE_FACTORY_H
