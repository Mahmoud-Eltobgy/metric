//=================================================================================================
/*!
//  \file blaze/math/LowerMatrix.h
//  \brief Header file for the complete LowerMatrix implementation
//
//  Copyright (C) 2012-2019 Klaus Iglberger - All Rights Reserved
//
//  This file is part of the Blaze library. You can redistribute it and/or modify it under
//  the terms of the New (Revised) BSD License. Redistribution and use in source and binary
//  forms, with or without modification, are permitted provided that the following conditions
//  are met:
//
//  1. Redistributions of source code must retain the above copyright notice, this list of
//     conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright notice, this list
//     of conditions and the following disclaimer in the documentation and/or other materials
//     provided with the distribution.
//  3. Neither the names of the Blaze development group nor the names of its contributors
//     may be used to endorse or promote products derived from this software without specific
//     prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY
//  EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
//  OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
//  SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED
//  TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
//  BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
//  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
//  DAMAGE.
*/
//=================================================================================================

#ifndef _BLAZE_MATH_LOWERMATRIX_H_
#define _BLAZE_MATH_LOWERMATRIX_H_


//*************************************************************************************************
// Includes
//*************************************************************************************************

#include <cmath>
#include <vector>
#include "../math/Aliases.h"
#include "../math/adaptors/DiagonalMatrix.h"
#include "../math/adaptors/LowerMatrix.h"
#include "../math/constraints/DenseMatrix.h"
#include "../math/constraints/Resizable.h"
#include "../math/constraints/SparseMatrix.h"
#include "../math/DenseMatrix.h"
#include "../math/Exception.h"
#include "../math/SparseMatrix.h"
#include "../math/typetraits/IsDenseMatrix.h"
#include "../math/typetraits/UnderlyingBuiltin.h"
#include "../math/UpperMatrix.h"
#include "../util/Indices.h"
#include "../util/IntegralConstant.h"
#include "../util/Random.h"
#include "../util/Types.h"


namespace blaze {

//=================================================================================================
//
//  RAND SPECIALIZATION
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Specialization of the Rand class template for LowerMatrix.
// \ingroup random
//
// This specialization of the Rand class creates random instances of LowerMatrix.
*/
template< typename MT  // Type of the adapted matrix
        , bool SO      // Storage order of the adapted matrix
        , bool DF >    // Numeric flag
class Rand< LowerMatrix<MT,SO,DF> >
{
 public:
   //**Generate functions**************************************************************************
   /*!\name Generate functions */
   //@{
   inline const LowerMatrix<MT,SO,DF> generate() const;
   inline const LowerMatrix<MT,SO,DF> generate( size_t n ) const;
   inline const LowerMatrix<MT,SO,DF> generate( size_t n, size_t nonzeros ) const;

   template< typename Arg >
   inline const LowerMatrix<MT,SO,DF> generate( const Arg& min, const Arg& max ) const;

   template< typename Arg >
   inline const LowerMatrix<MT,SO,DF> generate( size_t n, const Arg& min, const Arg& max ) const;

   template< typename Arg >
   inline const LowerMatrix<MT,SO,DF> generate( size_t n, size_t nonzeros,
                                                const Arg& min, const Arg& max ) const;
   //@}
   //**********************************************************************************************

   //**Randomize functions*************************************************************************
   /*!\name Randomize functions */
   //@{
   inline void randomize( LowerMatrix<MT,SO,DF>& matrix ) const;
   inline void randomize( LowerMatrix<MT,false,DF>& matrix, size_t nonzeros ) const;
   inline void randomize( LowerMatrix<MT,true,DF>& matrix, size_t nonzeros ) const;

   template< typename Arg >
   inline void randomize( LowerMatrix<MT,SO,DF>& matrix, const Arg& min, const Arg& max ) const;

   template< typename Arg >
   inline void randomize( LowerMatrix<MT,false,DF>& matrix, size_t nonzeros,
                          const Arg& min, const Arg& max ) const;

   template< typename Arg >
   inline void randomize( LowerMatrix<MT,true,DF>& matrix, size_t nonzeros,
                          const Arg& min, const Arg& max ) const;
   //@}
   //**********************************************************************************************

 private:
   //**Randomize functions*************************************************************************
   /*!\name Randomize functions */
   //@{
   inline void randomize( LowerMatrix<MT,SO,DF>& matrix, TrueType  ) const;
   inline void randomize( LowerMatrix<MT,SO,DF>& matrix, FalseType ) const;

   template< typename Arg >
   inline void randomize( LowerMatrix<MT,SO,DF>& matrix, const Arg& min, const Arg& max, TrueType ) const;

   template< typename Arg >
   inline void randomize( LowerMatrix<MT,SO,DF>& matrix, const Arg& min, const Arg& max, FalseType ) const;
   //@}
   //**********************************************************************************************
};
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Generation of a random LowerMatrix.
//
// \return The generated random matrix.
*/
template< typename MT  // Type of the adapted matrix
        , bool SO      // Storage order of the adapted matrix
        , bool DF >    // Numeric flag
inline const LowerMatrix<MT,SO,DF> Rand< LowerMatrix<MT,SO,DF> >::generate() const
{
   BLAZE_CONSTRAINT_MUST_NOT_BE_RESIZABLE_TYPE( MT );

   LowerMatrix<MT,SO,DF> matrix;
   randomize( matrix );
   return matrix;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Generation of a random LowerMatrix.
//
// \param n The number of rows and columns of the random matrix.
// \return The generated random matrix.
*/
template< typename MT  // Type of the adapted matrix
        , bool SO      // Storage order of the adapted matrix
        , bool DF >    // Numeric flag
inline const LowerMatrix<MT,SO,DF>
   Rand< LowerMatrix<MT,SO,DF> >::generate( size_t n ) const
{
   BLAZE_CONSTRAINT_MUST_BE_RESIZABLE_TYPE( MT );

   LowerMatrix<MT,SO,DF> matrix( n );
   randomize( matrix );
   return matrix;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Generation of a random LowerMatrix.
//
// \param n The number of rows and columns of the random matrix.
// \param nonzeros The number of non-zero elements of the random matrix.
// \return The generated random matrix.
// \exception std::invalid_argument Invalid number of non-zero elements.
*/
template< typename MT  // Type of the adapted matrix
        , bool SO      // Storage order of the adapted matrix
        , bool DF >    // Numeric flag
inline const LowerMatrix<MT,SO,DF>
   Rand< LowerMatrix<MT,SO,DF> >::generate( size_t n, size_t nonzeros ) const
{
   BLAZE_CONSTRAINT_MUST_BE_RESIZABLE_TYPE    ( MT );
   BLAZE_CONSTRAINT_MUST_BE_SPARSE_MATRIX_TYPE( MT );

   if( nonzeros > LowerMatrix<MT,SO,DF>::maxNonZeros( n ) ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Invalid number of non-zero elements" );
   }

   LowerMatrix<MT,SO,DF> matrix( n );
   randomize( matrix, nonzeros );

   return matrix;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Generation of a random LowerMatrix.
//
// \param min The smallest possible value for a matrix element.
// \param max The largest possible value for a matrix element.
// \return The generated random matrix.
*/
template< typename MT     // Type of the adapted matrix
        , bool SO         // Storage order of the adapted matrix
        , bool DF >       // Numeric flag
template< typename Arg >  // Min/max argument type
inline const LowerMatrix<MT,SO,DF>
   Rand< LowerMatrix<MT,SO,DF> >::generate( const Arg& min, const Arg& max ) const
{
   BLAZE_CONSTRAINT_MUST_NOT_BE_RESIZABLE_TYPE( MT );

   LowerMatrix<MT,SO,DF> matrix;
   randomize( matrix, min, max );
   return matrix;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Generation of a random LowerMatrix.
//
// \param n The number of rows and columns of the random matrix.
// \param min The smallest possible value for a matrix element.
// \param max The largest possible value for a matrix element.
// \return The generated random matrix.
*/
template< typename MT     // Type of the adapted matrix
        , bool SO         // Storage order of the adapted matrix
        , bool DF >       // Numeric flag
template< typename Arg >  // Min/max argument type
inline const LowerMatrix<MT,SO,DF>
   Rand< LowerMatrix<MT,SO,DF> >::generate( size_t n, const Arg& min, const Arg& max ) const
{
   BLAZE_CONSTRAINT_MUST_BE_RESIZABLE_TYPE( MT );

   LowerMatrix<MT,SO,DF> matrix( n );
   randomize( matrix, min, max );
   return matrix;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Generation of a random LowerMatrix.
//
// \param n The number of rows and columns of the random matrix.
// \param nonzeros The number of non-zero elements of the random matrix.
// \param min The smallest possible value for a matrix element.
// \param max The largest possible value for a matrix element.
// \return The generated random matrix.
// \exception std::invalid_argument Invalid number of non-zero elements.
*/
template< typename MT     // Type of the adapted matrix
        , bool SO         // Storage order of the adapted matrix
        , bool DF >       // Numeric flag
template< typename Arg >  // Min/max argument type
inline const LowerMatrix<MT,SO,DF>
   Rand< LowerMatrix<MT,SO,DF> >::generate( size_t n, size_t nonzeros,
                                            const Arg& min, const Arg& max ) const
{
   BLAZE_CONSTRAINT_MUST_BE_RESIZABLE_TYPE    ( MT );
   BLAZE_CONSTRAINT_MUST_BE_SPARSE_MATRIX_TYPE( MT );

   if( nonzeros > LowerMatrix<MT,SO,DF>::maxNonZeros( n ) ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Invalid number of non-zero elements" );
   }

   LowerMatrix<MT,SO,DF> matrix( n );
   randomize( matrix, nonzeros, min, max );

   return matrix;
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Randomization of a LowerMatrix.
//
// \param matrix The matrix to be randomized.
// \return void
*/
template< typename MT  // Type of the adapted matrix
        , bool SO      // Storage order of the adapted matrix
        , bool DF >    // Numeric flag
inline void Rand< LowerMatrix<MT,SO,DF> >::randomize( LowerMatrix<MT,SO,DF>& matrix ) const
{
   randomize( matrix, typename IsDenseMatrix<MT>::Type() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Randomization of a dense LowerMatrix.
//
// \param matrix The matrix to be randomized.
// \return void
*/
template< typename MT  // Type of the adapted matrix
        , bool SO      // Storage order of the adapted matrix
        , bool DF >    // Numeric flag
inline void Rand< LowerMatrix<MT,SO,DF> >::randomize( LowerMatrix<MT,SO,DF>& matrix, TrueType ) const
{
   BLAZE_CONSTRAINT_MUST_BE_DENSE_MATRIX_TYPE( MT );

   using ET = ElementType_t<MT>;

   const size_t n( matrix.rows() );

   for( size_t i=0UL; i<n; ++i ) {
      for( size_t j=0UL; j<=i; ++j ) {
         matrix(i,j) = rand<ET>();
      }
   }
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Randomization of a sparse LowerMatrix.
//
// \param matrix The matrix to be randomized.
// \return void
*/
template< typename MT  // Type of the adapted matrix
        , bool SO      // Storage order of the adapted matrix
        , bool DF >    // Numeric flag
inline void Rand< LowerMatrix<MT,SO,DF> >::randomize( LowerMatrix<MT,SO,DF>& matrix, FalseType ) const
{
   BLAZE_CONSTRAINT_MUST_BE_SPARSE_MATRIX_TYPE( MT );

   const size_t n( matrix.rows() );

   if( n == 0UL ) return;

   const size_t nonzeros( rand<size_t>( 1UL, std::ceil( 0.3*n*n ) ) );

   randomize( matrix, nonzeros );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Randomization of a row-major sparse LowerMatrix.
//
// \param matrix The matrix to be randomized.
// \param nonzeros The number of non-zero elements of the random matrix.
// \return void
// \exception std::invalid_argument Invalid number of non-zero elements.
*/
template< typename MT  // Type of the adapted matrix
        , bool SO      // Storage order of the adapted matrix
        , bool DF >    // Numeric flag
inline void Rand< LowerMatrix<MT,SO,DF> >::randomize( LowerMatrix<MT,false,DF>& matrix, size_t nonzeros ) const
{
   BLAZE_CONSTRAINT_MUST_BE_SPARSE_MATRIX_TYPE( MT );

   using ET = ElementType_t<MT>;

   const size_t n( matrix.rows() );

   if( nonzeros > LowerMatrix<MT,SO,DF>::maxNonZeros( n ) ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Invalid number of non-zero elements" );
   }

   if( n == 0UL ) return;

   matrix.reset();
   matrix.reserve( nonzeros );

   std::vector<size_t> dist( n );

   for( size_t nz=0UL; nz<nonzeros; ) {
      const size_t index = rand<size_t>( 0UL, n-1UL );
      if( dist[index] == index+1UL ) continue;
      ++dist[index];
      ++nz;
   }

   for( size_t i=0UL; i<n; ++i ) {
      const Indices indices( 0UL, i, dist[i] );
      for( size_t j : indices ) {
         matrix.append( i, j, rand<ET>() );
      }
      matrix.finalize( i );
   }
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Randomization of a column-major sparse LowerMatrix.
//
// \param matrix The matrix to be randomized.
// \param nonzeros The number of non-zero elements of the random matrix.
// \return void
// \exception std::invalid_argument Invalid number of non-zero elements.
*/
template< typename MT  // Type of the adapted matrix
        , bool SO      // Storage order of the adapted matrix
        , bool DF >    // Numeric flag
inline void Rand< LowerMatrix<MT,SO,DF> >::randomize( LowerMatrix<MT,true,DF>& matrix, size_t nonzeros ) const
{
   BLAZE_CONSTRAINT_MUST_BE_SPARSE_MATRIX_TYPE( MT );

   using ET = ElementType_t<MT>;

   const size_t n( matrix.rows() );

   if( nonzeros > LowerMatrix<MT,SO,DF>::maxNonZeros( n ) ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Invalid number of non-zero elements" );
   }

   if( n == 0UL ) return;

   matrix.reset();
   matrix.reserve( nonzeros );

   std::vector<size_t> dist( n );

   for( size_t nz=0UL; nz<nonzeros; ) {
      const size_t index = rand<size_t>( 0UL, n-1UL );
      if( dist[index] == n - index ) continue;
      ++dist[index];
      ++nz;
   }

   for( size_t j=0UL; j<n; ++j ) {
      const Indices indices( j, n-1UL, dist[j] );
      for( size_t i : indices ) {
         matrix.append( i, j, rand<ET>() );
      }
      matrix.finalize( j );
   }
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Randomization of a LowerMatrix.
//
// \param matrix The matrix to be randomized.
// \param min The smallest possible value for a matrix element.
// \param max The largest possible value for a matrix element.
// \return void
*/
template< typename MT     // Type of the adapted matrix
        , bool SO         // Storage order of the adapted matrix
        , bool DF >       // Numeric flag
template< typename Arg >  // Min/max argument type
inline void Rand< LowerMatrix<MT,SO,DF> >::randomize( LowerMatrix<MT,SO,DF>& matrix,
                                                      const Arg& min, const Arg& max ) const
{
   randomize( matrix, min, max, typename IsDenseMatrix<MT>::Type() );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Randomization of a dense LowerMatrix.
//
// \param matrix The matrix to be randomized.
// \param min The smallest possible value for a matrix element.
// \param max The largest possible value for a matrix element.
// \return void
*/
template< typename MT     // Type of the adapted matrix
        , bool SO         // Storage order of the adapted matrix
        , bool DF >       // Numeric flag
template< typename Arg >  // Min/max argument type
inline void Rand< LowerMatrix<MT,SO,DF> >::randomize( LowerMatrix<MT,SO,DF>& matrix,
                                                      const Arg& min, const Arg& max, TrueType ) const
{
   BLAZE_CONSTRAINT_MUST_BE_DENSE_MATRIX_TYPE( MT );

   using ET = ElementType_t<MT>;

   const size_t n( matrix.rows() );

   for( size_t i=0UL; i<n; ++i ) {
      for( size_t j=0UL; j<=i; ++j ) {
         matrix(i,j) = rand<ET>( min, max );
      }
   }
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Randomization of a sparse LowerMatrix.
//
// \param matrix The matrix to be randomized.
// \param min The smallest possible value for a matrix element.
// \param max The largest possible value for a matrix element.
// \return void
*/
template< typename MT     // Type of the adapted matrix
        , bool SO         // Storage order of the adapted matrix
        , bool DF >       // Numeric flag
template< typename Arg >  // Min/max argument type
inline void Rand< LowerMatrix<MT,SO,DF> >::randomize( LowerMatrix<MT,SO,DF>& matrix,
                                                      const Arg& min, const Arg& max, FalseType ) const
{
   BLAZE_CONSTRAINT_MUST_BE_SPARSE_MATRIX_TYPE( MT );

   const size_t n( matrix.rows() );

   if( n == 0UL ) return;

   const size_t nonzeros( rand<size_t>( 1UL, std::ceil( 0.3*n*n ) ) );

   randomize( matrix, nonzeros, min, max );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Randomization of a row-major sparse LowerMatrix.
//
// \param matrix The matrix to be randomized.
// \param nonzeros The number of non-zero elements of the random matrix.
// \param min The smallest possible value for a matrix element.
// \param max The largest possible value for a matrix element.
// \return void
// \exception std::invalid_argument Invalid number of non-zero elements.
*/
template< typename MT     // Type of the adapted matrix
        , bool SO         // Storage order of the adapted matrix
        , bool DF >       // Numeric flag
template< typename Arg >  // Min/max argument type
inline void Rand< LowerMatrix<MT,SO,DF> >::randomize( LowerMatrix<MT,false,DF>& matrix,
                                                      size_t nonzeros, const Arg& min, const Arg& max ) const
{
   BLAZE_CONSTRAINT_MUST_BE_SPARSE_MATRIX_TYPE( MT );

   using ET = ElementType_t<MT>;

   const size_t n( matrix.rows() );

   if( nonzeros > LowerMatrix<MT,SO,DF>::maxNonZeros( n ) ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Invalid number of non-zero elements" );
   }

   if( n == 0UL ) return;

   matrix.reset();
   matrix.reserve( nonzeros );

   std::vector<size_t> dist( n );

   for( size_t nz=0UL; nz<nonzeros; ) {
      const size_t index = rand<size_t>( 0UL, n-1UL );
      if( dist[index] == index+1UL ) continue;
      ++dist[index];
      ++nz;
   }

   for( size_t i=0UL; i<n; ++i ) {
      const Indices indices( 0UL, i, dist[i] );
      for( size_t j : indices ) {
         matrix.append( i, j, rand<ET>( min, max ) );
      }
      matrix.finalize( i );
   }
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Randomization of a column-major sparse LowerMatrix.
//
// \param matrix The matrix to be randomized.
// \param nonzeros The number of non-zero elements of the random matrix.
// \param min The smallest possible value for a matrix element.
// \param max The largest possible value for a matrix element.
// \return void
// \exception std::invalid_argument Invalid number of non-zero elements.
*/
template< typename MT     // Type of the adapted matrix
        , bool SO         // Storage order of the adapted matrix
        , bool DF >       // Numeric flag
template< typename Arg >  // Min/max argument type
inline void Rand< LowerMatrix<MT,SO,DF> >::randomize( LowerMatrix<MT,true,DF>& matrix,
                                                      size_t nonzeros, const Arg& min, const Arg& max ) const
{
   BLAZE_CONSTRAINT_MUST_BE_SPARSE_MATRIX_TYPE( MT );

   using ET = ElementType_t<MT>;

   const size_t n( matrix.rows() );

   if( nonzeros > LowerMatrix<MT,SO,DF>::maxNonZeros( n ) ) {
      BLAZE_THROW_INVALID_ARGUMENT( "Invalid number of non-zero elements" );
   }

   if( n == 0UL ) return;

   matrix.reset();
   matrix.reserve( nonzeros );

   std::vector<size_t> dist( n );

   for( size_t nz=0UL; nz<nonzeros; ) {
      const size_t index = rand<size_t>( 0UL, n-1UL );
      if( dist[index] == n - index ) continue;
      ++dist[index];
      ++nz;
   }

   for( size_t j=0UL; j<n; ++j ) {
      const Indices indices( j, n-1UL, dist[j] );
      for( size_t i : indices ) {
         matrix.append( i, j, rand<ET>( min, max ) );
      }
      matrix.finalize( j );
   }
}
/*! \endcond */
//*************************************************************************************************




//=================================================================================================
//
//  MAKE FUNCTIONS
//
//=================================================================================================

//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Setup of a random symmetric LowerMatrix.
//
// \param matrix The matrix to be randomized.
// \return void
*/
template< typename MT  // Type of the adapted matrix
        , bool SO      // Storage order of the adapted matrix
        , bool DF >    // Density flag
void makeSymmetric( LowerMatrix<MT,SO,DF>& matrix )
{
   const size_t n( matrix.rows() );

   reset( matrix );

   for( size_t i=0UL; i<n; ++i ) {
      matrix(i,i) = rand< ElementType_t<MT> >();
   }

   BLAZE_INTERNAL_ASSERT( isSymmetric( matrix ), "Non-symmetric matrix detected" );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Setup of a random symmetric LowerMatrix.
//
// \param matrix The matrix to be randomized.
// \param min The smallest possible value for a matrix element.
// \param max The largest possible value for a matrix element.
// \return void
*/
template< typename MT     // Type of the adapted matrix
        , bool SO         // Storage order of the adapted matrix
        , bool DF         // Density flag
        , typename Arg >  // Min/max argument type
void makeSymmetric( LowerMatrix<MT,SO,DF>& matrix, const Arg& min, const Arg& max )
{
   using Type = ElementType_t<MT>;

   const size_t n( matrix.rows() );

   reset( matrix );

   for( size_t i=0UL; i<n; ++i ) {
      matrix(i,i) = rand<Type>( min, max );
   }

   BLAZE_INTERNAL_ASSERT( isSymmetric( matrix ), "Non-symmetric matrix detected" );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Setup of a random Hermitian LowerMatrix.
//
// \param matrix The matrix to be randomized.
// \return void
*/
template< typename MT  // Type of the adapted matrix
        , bool SO      // Storage order of the adapted matrix
        , bool DF >    // Density flag
void makeHermitian( LowerMatrix<MT,SO,DF>& matrix )
{
   using Type = UnderlyingBuiltin_t< ElementType_t<MT> >;

   const size_t n( matrix.rows() );

   reset( matrix );

   for( size_t i=0UL; i<n; ++i ) {
      matrix(i,i) = rand<Type>();
   }

   BLAZE_INTERNAL_ASSERT( isHermitian( matrix ), "Non-Hermitian matrix detected" );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Setup of a random Hermitian LowerMatrix.
//
// \param matrix The matrix to be randomized.
// \param min The smallest possible value for a matrix element.
// \param max The largest possible value for a matrix element.
// \return void
*/
template< typename MT     // Type of the adapted matrix
        , bool SO         // Storage order of the adapted matrix
        , bool DF         // Density flag
        , typename Arg >  // Min/max argument type
void makeHermitian( LowerMatrix<MT,SO,DF>& matrix, const Arg& min, const Arg& max )
{
   using Type = UnderlyingBuiltin_t< ElementType_t<MT> >;

   const size_t n( matrix.rows() );

   reset( matrix );

   for( size_t i=0UL; i<n; ++i ) {
      matrix(i,i) = rand<Type>( min, max );
   }

   BLAZE_INTERNAL_ASSERT( isHermitian( matrix ), "Non-Hermitian matrix detected" );
}
/*! \endcond */
//*************************************************************************************************


//*************************************************************************************************
/*! \cond BLAZE_INTERNAL */
/*!\brief Setup of a random (Hermitian) positive definite LowerMatrix.
//
// \param matrix The matrix to be randomized.
// \return void
*/
template< typename MT  // Type of the adapted matrix
        , bool SO      // Storage order of the adapted matrix
        , bool DF >    // Density flag
void makePositiveDefinite( LowerMatrix<MT,SO,DF>& matrix )
{
   makeHermitian( matrix );
}
/*! \endcond */
//*************************************************************************************************

} // namespace blaze

#endif
