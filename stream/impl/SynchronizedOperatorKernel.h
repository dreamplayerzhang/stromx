#ifndef STREAM_SYNCHRONIZEDOPERATORKERNEL_H
#define STREAM_SYNCHRONIZEDOPERATORKERNEL_H

#include "../DataProvider.h"
#include "../Parameter.h"
#include "../OperatorKernel.h"

#include "Id2DataMap.h"

#include <boost/thread/mutex.hpp>
#include <boost/thread/condition_variable.hpp>

namespace stream
{
    class OperatorKernel;
    class Data;
    
    class SynchronizedOperatorKernel : public DataProvider
    {
    public:    
        enum Status
        {
            NONE,
            INITIALIZED,
            ACTIVE,
            EXECUTING
        };
        
        SynchronizedOperatorKernel(OperatorKernel* const op);
        virtual ~SynchronizedOperatorKernel();
        
        const OperatorInfo* const info() const { return m_op; }
        
        void initialize();
        void activate();
        void deactivate();
        
        // DataProvider implementation
        void receiveInputData(const Id2DataMapper& mapper);
        void sendOutputData(const Id2DataMapper& mapper);
        void testForInterrupt();
        void sleep(const unsigned int microseconds);
        
        const Status status() { return m_status; }
        
        void setParameter(unsigned int id, const Data& value);
        const Data& getParameter(unsigned int id);
        
        DataContainer getOutputData(const unsigned int id);
        void setInputData(const unsigned int id, DataContainer data);
        void clearOutputData(unsigned int id);
        
    private:
        typedef boost::lock_guard<boost::mutex> lock_t;
        typedef boost::unique_lock<boost::mutex> unique_lock_t;
        
        void execute();
        void waitForSignal(unique_lock_t& lock);
        void validateParameterId(const unsigned int id);
        void validateWriteAccess(const unsigned int id);
        void validateReadAccess(const unsigned int id);
        void validateParameterType(const unsigned int id, const DataType& type);
        
        OperatorKernel* m_op;
        Status m_status;
        boost::condition_variable_any m_cond;
        boost::mutex m_mutex;
        boost::mutex m_executeMutex;
        impl::Id2DataMap m_inputMap;
        impl::Id2DataMap m_outputMap;
    };
}

#endif // STREAM_SYNCHRONIZEDOPERATORKERNEL_H