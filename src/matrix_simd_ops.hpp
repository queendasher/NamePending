#include "matrix.hpp"
#include "../NamePending-HPC/src/simd.hpp"

namespace Mathlib {

using ASC_HPC::SIMD;
using ASC_HPC::fma;
using std::size_t;

// Compute C_block(HxW) += A_block(HxK) * B_block(KxW)
template <size_t H, size_t W>
inline void AddMatMatKernel(size_t K,
                            const double* a, size_t lda,
                            const double* b, size_t ldb,
                            double*       c, size_t ldc)
{
    // Load current C-block into registers: one SIMD<double, H> per column of the block
    SIMD<double, H> acc[W];
    for (size_t j = 0; j < W; ++j) {
        acc[j] = SIMD<double, H>(const_cast<double*>(c + j * ldc));  // C(i..i+H-1, j0+j)
    }

    // Main multiply-accumulate loop over k
    for (size_t k = 0; k < K; ++k) {
        // A column k, rows i..i+H-1 is contiguous in col-major
        const double* a_col = a + k * lda;
        SIMD<double, H> a_vec(const_cast<double*>(a_col));           // [A(i+0,k),...,A(i+H-1,k)]

        // For each column j in the W-block
        for (size_t j = 0; j < W; ++j) {
            // B(k, j0+j) = b[ k + j*ldb ]
            double bkj = b[k + j * ldb];
            SIMD<double, H> b_vec(bkj);                              // broadcast scalar to all lanes

            acc[j] = fma(a_vec, b_vec, acc[j]);          // acc_j += a_vec * bkj
        }
    }

    // Store back the updated C-block
    for (size_t j = 0; j < W; ++j) {
        acc[j].store(c + j * ldc);
    }
}


void AddMatMat2(MatrixView<double> A,
                MatrixView<double> B,
                MatrixView<double> C)
{
    constexpr size_t H = 4;
    constexpr size_t W = 12;

    const size_t m = C.Rows();     // #rows of C (same as A.Rows())
    const size_t n = C.Cols();     // #cols of C (same as B.Cols())
    const size_t k = A.Cols();     // shared dimension

    // Main blocked region: multiples of HxW
    size_t j = 0;
    for (; j + W <= n; j += W) {
        size_t i = 0;
        for (; i + H <= m; i += H) {
            AddMatMatKernel<H, W>(
                k,
                &A(i, 0), A.Dist(),          // A block: HxK starting at row i, col 0
                &B(0, j), B.Dist(),          // B block: KxW starting at row 0, col j
                &C(i, j), C.Dist());         // C block: HxW starting at (i, j)
        }

        // Remaining rows for this W-wide stripe
        for (; i < m; ++i) {
            for (size_t jj = j; jj < j + W; ++jj) {
                double sum = 0.0;
                for (size_t kk = 0; kk < k; ++kk) {
                    sum += A(i, kk) * B(kk, jj);
                }
                C(i, jj) += sum;
            }
        }
    }

    // Remaining columns (n % W) — do full scalar matmul for this tail
    if (j < n) {
        for (size_t i = 0; i < m; ++i) {
            for (size_t jj = j; jj < n; ++jj) {
                double sum = 0.0;
                for (size_t kk = 0; kk < k; ++kk) {
                    sum += A(i, kk) * B(kk, jj);
                }
                C(i, jj) += sum;
            }
        }
    }
}



void AddMatMat (MatrixView<double> A, MatrixView<double> B, MatrixView<double> C) {
  constexpr size_t BH=96;
  constexpr size_t BW=96;
  alignas (64) double memBA[BH*BW];
  for (size_t i1 = 0; i1 < A.Rows(); i1 += BH)
    for (size_t j1 = 0; j1 < A.Cols(); j1 += BW) {
      size_t i2 = std::min(A.Rows(), i1+BH);
      size_t j2 = std::min(A.Cols(), j1+BW);

      MatrixView<double> Ablock(i2-i1, j2-j1, BW, memBA);
      Ablock = A.RowRange(i1,i2).ColRange(j1,j2);
      AddMatMat2 (Ablock, B.RowRange(j1,j2), C.RowRange(i1,i2));
    }
}



}
