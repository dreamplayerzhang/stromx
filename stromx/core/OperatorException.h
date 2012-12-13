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

#ifndef STROMX_CORE_OPERATOREXCEPTI0N_H
#define STROMX_CORE_OPERATOREXCEPTI0N_H

#include "Exception.h"

namespace stromx
{
    namespace core
    {
        class OperatorInfo;
        class Parameter;
        
        /** \brief Error in connection with a specified operator. */
        class OperatorError : public Exception
        {
        public:
            /** Constructs an operator exception, i.e. an exception related to an operator. */
            OperatorError(const OperatorInfo& op, const std::string & message)
              : Exception(message),
                m_operator(op)
            {}
            
            /** Constructs an operator exception, i.e. an exception related to an operator. */
            OperatorError(const OperatorInfo& op, const std::string & message, const std::string & name)
              : Exception(message),
                m_operator(op),
                m_name(name)
            {}
            
            virtual ~OperatorError() throw() {}
            
            /** Returns the operator this exception relates to. */
            const OperatorInfo& op() const { return m_operator; }
            
            /** Returns the name of the operator this exception relates to. */
            const std::string & name() const { return m_name; }
            
            /** Sets the name of the operator this exception relates to. */
            void setName(const std::string & name) { m_name = name; }
            
        private:
            const OperatorInfo& m_operator;
            std::string m_name;
        };
        
        /** \brief The current state of the operator does not allow a specific operation. */
        class WrongOperatorState : public OperatorError
        {
        public:
            WrongOperatorState(const OperatorInfo& op, const std::string & message = "WrongOperatorState")
              : OperatorError(op, message)
            {}
        };
        
        /** \brief A parameter with this ID does not exist. */
        class WrongParameterId : public OperatorError
        {
        public:
            WrongParameterId(const unsigned int id, const OperatorInfo& op, const std::string & message = "WrongParameterId")
              : OperatorError(op, message),
                m_id(id)
            {}
            
        private:
            unsigned int m_id;
        };
        
        /** \brief General error in connection with a specified parameter. */
        class ParameterError : public OperatorError
        {
        public:
            ParameterError(const Parameter& param, const OperatorInfo& op, const std::string & message = "ParameterError")
              : OperatorError(op, message),
                m_parameter(param)
            {}
            
            const Parameter & parameter() const { return m_parameter; }
            
        private:
            const Parameter& m_parameter;
        };
        
        /** \brief Tried to set a parameter to data of the wrong type. */
        class WrongParameterType : public ParameterError
        {
        public:
            WrongParameterType(const Parameter& param, const OperatorInfo& op, const std::string & message = "WrongParameterType")
              : ParameterError(param, op, message)
            {}
        };
        
        /** \brief Tried to set a parameter to a wrong value. */
        class WrongParameterValue : public ParameterError
        {
        public:
            WrongParameterValue(const Parameter& param, const OperatorInfo& op, const std::string & message = "WrongParameterValue")
              : ParameterError(param, op, message)
            {}
        };
        
        /** \brief Tried to violate the parameter access mode. */
        class ParameterAccessViolation : public ParameterError
        {
        public:
            ParameterAccessViolation(const Parameter& param, const OperatorInfo& op, const std::string & message = "ParameterAccessViolation")
              : ParameterError(param, op, message)
            {}
        };
        
        
        /** \brief General error in connection with an operator output. */
        class OutputError : public OperatorError
        {
        public:
            OutputError(const unsigned int outputId, const OperatorInfo& op, const std::string & message = "InputError")
              : OperatorError(op, message),
                m_outputId(outputId)
            {}
            
            unsigned int outputId() const { return m_outputId; }
        private:
            const unsigned int m_outputId;
        };
        
        /** \brief General error in connection with an operator input. */
        class InputError : public OperatorError
        {
        public:
            InputError(const unsigned int inputId, const OperatorInfo& op, const std::string & message = "InputError")
              : OperatorError(op, message),
                m_inputId(inputId)
            {}
            
            unsigned int inputId() const { return m_inputId; }
        private:
            const unsigned int m_inputId;
        };
        
        /** \brief Tried to set an input to data of the wrong type. */
        class WrongInputType : public InputError
        {
        public:
            WrongInputType(const unsigned int inputId, const OperatorInfo& op, const std::string & message = "WrongInputType")
              : InputError(inputId, op, message)
            {}
        };
        
        /** \brief Tried to set an output to data of the wrong type. */
        class WrongOutputType : public OutputError
        {
        public:
            WrongOutputType(const unsigned int outputId, const OperatorInfo& op, const std::string & message = "WrongInputType")
              : OutputError(outputId, op, message)
            {}
        };
    }
}

#endif // STROMX_CORE_OPERATOREXCEPTI0N_H
