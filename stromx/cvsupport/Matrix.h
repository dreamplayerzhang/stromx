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

#ifndef STROMX_CVSUPPORT_MATRIX_H
#define STROMX_CVSUPPORT_MATRIX_H

#include <string>
#include <vector>
#include "stromx/cvsupport/Config.h"
#include <stromx/runtime/MatrixWrapper.h>

namespace cv
{
    class Mat;
}

namespace stromx
{
    namespace cvsupport
    {
        /** \brief %Matrix implementation based on OpenCV matrices. */
        class STROMX_CVSUPPORT_API Matrix : public runtime::MatrixWrapper
        {
            friend STROMX_CVSUPPORT_API cv::Mat getOpenCvMat(const runtime::Matrix& matrix);
        
        public:
            
            /** 
             * Constructs an empty matrix with zero rows and columns and value type Matrix::NONE.
             * No data is associated with the matrix, i.e. its data and buffer pointers are 0.
             */
            Matrix();
            
            /** Constructs a matrix for the given parameters. */
            explicit Matrix(const unsigned int rows, const unsigned int cols, const ValueType valueType);
            
            /** 
             * Constructs a matrix from \c cvMatrix. The data of the input matrix is not copied
             * only a lightweight wrapper is allocated which references \c cvMatrix.
             */
            explicit Matrix(cv::Mat& cvMatrix);
            
            /** Copy constructs a matrix from \c matrix. */
            explicit Matrix(const stromx::runtime::Matrix& matrix);
            
            /** Copy constructs a matrix from \c matrix. */
            explicit Matrix(const stromx::cvsupport::Matrix& matrix);
            
            /** 
             * Allocates a matrix with a buffer of a given size in bytes. The matrix has 1 row and
             * \c size columns. Its value type is Image::NONE. The buffer size of the image is 
             * guaranteed to be at least \c size.
             */
            explicit Matrix(const unsigned int size);
            
            /** Allocates a matrix and reads its content from the file \c filename. */
            explicit Matrix(const std::string & filename);
            
            virtual ~Matrix();
            
            virtual const runtime::Version & version() const { return VERSION; }
            virtual const std::string & type() const { return TYPE; }
            virtual const std::string & package() const { return PACKAGE; }
            
            virtual Data* clone() const;
            
        private:
            static const std::string TYPE;
            static const std::string PACKAGE;
            static const runtime::Version VERSION;
            
            static int cvTypeFromValueType(const runtime::Matrix::ValueType valueType);
            static runtime::Matrix::ValueType valueTypeFromCvType(const int cvType);
            
            void copy(const stromx::runtime::Matrix & matrix);
            void allocate(const unsigned int rows, const unsigned int cols, const runtime::Matrix::ValueType valueType);
            void getDataFromCvMatrix(const ValueType valueType);
            
                
            cv::Mat* m_matrix;
        };
    }
}

#endif // STROMX_CVSUPPORT_MATRIX_H