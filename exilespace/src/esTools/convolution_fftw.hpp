//===========================================================================
/*!
 *
 *
 * \brief       Implementation of efficient convolution computation
 *
 *
 *
 *
 * \author      Hans J. T. Stephensen
 * \date        2015
 *
 *
 * \par Copyright 1995-2015 Shark Development Team
 *
 * <BR><HR>
 * This file is part of Shark.
 * <http://image.diku.dk/shark/>
 *
 * Shark is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Shark is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Shark.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
//===========================================================================
#ifndef CONVOLUTION_FFTW_HPP
#define CONVOLUTION_FFTW_HPP

#include <iostream>
#include <stdexcept>
#include <fftw3.h>

namespace esTools {

/// @brief Convolution Plan
///
/// This objects creates a plan to optimize the convolution
/// Instantiation can take several seconds so the plan must be
/// reused for it to have any effect.
class convolution_plan {
public:

    /// Constructor for 1D convolution plans
    /// @param width            input size
    /// @param kw               kernel width
    /// @param mode             Edge mode 0 = reduce image size to fit kernel, 1 = assume zero outside (default)
    /// @param threadMaxCount   Maximum number of allowed threads to use
    convolution_plan(int width, int kw, int mode, int threadMaxCount);

    /// Constructor for 2D convolution plans
    /// @param width            input width
    /// @param height           input height
    /// @param kw               kernel width
    /// @param mode             Edge mode 0 = reduce image size to fit kernel, 1 = assume zero outside (default)
    /// @param threadMaxCount   Maximum number of allowed threads to use
    convolution_plan(int width, int height, int kw, int mode, int threadMaxCount);

    /// Constructor for 3D convolution plans
    /// @param width            input width
    /// @param height           input height
    /// @param depth            input depth
    /// @param kw               kernel width
    /// @param mode             Edge mode 0 = reduce image size to fit kernel, 1 = assume zero outside (default)
    /// @param threadMaxCount   Maximum number of allowed threads to use
    convolution_plan(int width, int height, int depth, int kw, int mode, int threadMaxCount);

    /// Destructor
    ~convolution_plan();

    /// Getter for input dimensions
    int getDim(){return this->dim;};

    /// Getter for input width
    int getWidth(){return this->width;};

    /// Getter for input height
    int getHeight(){return this->height;};

    /// Getter for input depth
    int getDepth(){return this->depth;};

    /// Getter for internal forward fftw plan
    fftw_plan& getForwardPlan(){return this->forwardPlan;};

    /// Getter for internal backward fftw plan
    fftw_plan& getBackwardPlan(){return this->backwardPlan;};

    /// Getter for static kernel
    fftw_complex* getStaticKernel() {return this->staticKernel;};

    /// Check if static kernel is precomputed
    bool availableKernel() {
        return this->staticKernel != NULL;
    }

    /// Set static kernel
    template <class T>
    void setStaticKernel(const T &ker) {
        this->unsetStaticKernel();
        int size, d2, d3;

        switch (this->dim) {
        case 1:
            size = this->width;
            d3 = 0;
            d2 = 0;
            break;
        case 2:
            size = this->width*this->height;
            d3 = 0;
            d2 = 1;
            break;
        case 3:
            size = this->width*this->height*this->depth;
            d3 = 1;
            d2 = 1;
            break;
        }

        fftw_complex* tempArray;
        tempArray = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size);
        this->staticKernel = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * size);

        for (int k = 0; k < this->depth; ++k) {
            for (int j = 0; j < this->height; ++j) {
                for (int i = 0; i < this->width; ++i) {
                    int index = k*this->width*this->height*d3 + j*this->width*d2 + i;
                    //std::cout << index << " < " << size << std::endl;
                    //std::cout << "ZERO: " << k*this->width*this->height*d3 << " ROW: " << j*this->height*d2 << " index: " << i << std::endl;
                    if (i < this->kw && j < this->kw && k < this->kw) {
                        //std::cout << k*this->kw*this->kw*d3 + j*this->kw*d2 + i <<  std::endl;
                        //std::cout << k*this->kw*this->kw*d3 + j*this->kw*d2 + i << std::endl;
                        tempArray[index][0] = ker[k*this->kw*this->kw*d3 + j*this->kw*d2 + i];
                    } else {
                        tempArray[index][0] = 0.0;
                    }
                    tempArray[index][1] = 0.0;
                }
            }
        }

        fftw_execute_dft(this->forwardPlan, tempArray, this->staticKernel);
        fftw_free(tempArray);
    }

    /// Unset static kernel
    void unsetStaticKernel();

private:
    static int dim;
    static int width;
    static int height;
    static int depth;
    static int threadMaxCount;
    static int kw;
    fftw_plan forwardPlan;
    fftw_plan backwardPlan;
    fftw_complex* staticKernel;
};

/// \brief 1D convolution function.
///
/// Performs simple convolution on two input signal and kernel f and g
/// and stores the result f*g in given container.
/// For big input size use the fftw function instead.
///
/// @param  src        Linear object source of data
/// @param  dst        Linear object destination of result
/// @param  ker        Linear object kernel
/// @param  sx         Width of signal
/// @param  kw         kernel width
/// @param  kcx        kernel center (anchor) x
/// @param  mode       boundary mode
template <class T>
void simple_convolution_1d(const T &src, T &dst, const T &ker, int sx, int kw, int kcx, int mode = 1) {
    switch (mode) {
    case 0: {
        int kernelShift = kw - kcx - 1;
        for (int x = kernelShift; x < sx - kcx; ++x) {
            dst[x - kernelShift] = 0.0;
            int t = 0;
            for (int srx = x + kcx; srx > x - kw + kcx; --srx) {
                if (srx >= 0 && srx < sx) {
                    dst[x - kernelShift] += src[srx] * ker[t];
                }
                ++t;
            }
        }
        break;
    }
    case 1:
        for (int x = 0; x < sx; ++x) {
            dst[x] = 0.0;
            int t = 0;
            for (int srx = x + kcx; srx > x - kw + kcx; --srx) {
                if (srx >= 0 && srx < sx) {
                    dst[x] += src[srx] * ker[t];
                }
                ++t;
            }
        }
        break;
    default:
        throw std::invalid_argument("Warning: Invalid mode.");
    }
};

/// \brief 2D convolution function.
///
/// Performs simple convolution on two input signal and kernel f and g
/// and stores the result f*g in given container.
/// For big input size use the fftw function instead.
///
/// @param  src        Linear object source of data
/// @param  dst        Linear object destination of result
/// @param  ker        Linear object kernel
/// @param  sx         Width of signal
/// @param  sy         Height of signal
/// @param  kw         kernel width
/// @param  kcx        kernel center (anchor) x
/// @param  kcy        kernel center (anchor) y
/// @param  mode       boundary mode
template <class T>
void simple_convolution_2d(const T &src, T &dst, const T &ker, int sx, int sy, int kw, int kcx, int kcy, int mode) {
    switch (mode) {
    case 0: {
        int kernelShiftx = kw - kcx - 1;
        int kernelShifty = kw - kcy - 1;
        for (int y = kernelShifty; y < sy - kcy; ++y) {
            for (int x = kernelShiftx; x < sx - kcx; ++x) {
                dst[(y - kernelShifty)*(sx-kw+1) + x - kernelShiftx] = 0.0;
                int s = 0;
                for (int sry = y + kcy; sry > y - kw + kcy; --sry) {
                    int t = 0;
                    for (int srx = x + kcx; srx > x - kw + kcx; --srx) {
                        if (srx >= 0 && srx < sx && sry >= 0 && sry < sy) {
                            dst[(y - kernelShifty) * (sx-kw+1) + x - kernelShiftx] += src[sry*sx + srx] * ker[s*kw + t];
                        }
                        ++t;
                    }
                    ++s;
                }
            }
        }
        break;
    }
    case 1:
        for (int y = 0; y < sy; ++y) {
            for (int x = 0; x < sx; ++x) {
                dst[y*sx + x] = 0.0;
                int s = 0;
                for (int sry = y + kcy; sry > y - kw + kcy; --sry) {
                    int t = 0;
                    for (int srx = x + kcx; srx > x - kw + kcx; --srx) {
                        if (srx >= 0 && srx < sx && sry >= 0 && sry < sy) {
                            dst[y*sx + x] += src[sry*sx + srx] * ker[s*kw + t];
                        }
                        ++t;
                    }
                    ++s;
                }
            }
        }
        break;
    default:
        throw std::invalid_argument("Warning: Invalid mode.");
    }
};

/// \brief 3D convolution function.
///
/// Performs simple convolution on two input signal and kernel f and g
/// and stores the result f*g in given container.
/// For big input size use the fftw function instead.
///
/// @param  src        Linear object source of data
/// @param  dst        Linear object destination of result
/// @param  ker        Linear object kernel
/// @param  sx         Width of signal
/// @param  sy         Height of signal
/// @param  kw         kernel width
/// @param  kcx        kernel center (anchor) x
/// @param  kcy        kernel center (anchor) y
/// @param  mode       boundary mode
template <class T>
void simple_convolution_3d(const T &src, T &dst, const T &ker, int sx, int sy, int sz, int kw, int kcx, int kcy, int kcz, int mode) {
    // This can seem confusing
    // ## 1D -> 3D indexing 101 ##
    // z*sx*sy  sets the current depth level (Shifts an entire 'plane' sx*sy in size)
    // y*sx     sets the current row
    // x        sets which particular index in the current row
    switch (mode) {
    case 0: {
        int kernelShiftx = kw - kcx - 1;
        int kernelShifty = kw - kcy - 1;
        int kernelShiftz = kw - kcz - 1;
        for (int z = kernelShiftz; z < sz - kcz; ++z) {
            for (int y = kernelShifty; y < sy - kcy; ++y) {
                for (int x = kernelShiftx; x < sx - kcx; ++x) {
                    dst[(z - kernelShiftz)*(sx-kw+1)*(sy-kw+1) + (y - kernelShifty)*(sx-kw+1) + x - kernelShiftx] = 0.0;
                    int r = 0;
                    for (int srz = z + kcz; srz > z - kw + kcz; --srz) {
                        int s = 0;
                        for (int sry = y + kcy; sry > y - kw + kcy; --sry) {
                            int t = 0;
                            for (int srx = x + kcx; srx > x - kw + kcx; --srx) {
                                if (srx >= 0 && srx < sx && sry >= 0 && sry < sy && srz >= 0 && srz < sz) {

                                    dst[(z - kernelShiftz)*(sx-kw+1)*(sy-kw+1) + (y - kernelShifty)*(sx-kw+1) + x - kernelShiftx] += src[srz*sx*sy + sry*sx + srx] * ker[r*kw*kw + s*kw + t];
                                }
                                ++t;
                            }
                            ++s;
                        }
                        ++r;
                    }
                }
            }
        }
        break;
    }
    case 1:
        for (int z = 0; z < sz; ++z) {
            for (int y = 0; y < sy; ++y) {
                for (int x = 0; x < sx; ++x) {
                    dst[z*sx*sy + y*sx + x] = 0.0;
                    int r = 0;
                    for (int srz = z + kcz; srz > z - kw + kcz; --srz) {
                        int s = 0;
                        for (int sry = y + kcy; sry > y - kw + kcy; --sry) {
                            int t = 0;
                            for (int srx = x + kcx; srx > x - kw + kcx; --srx) {
                                if (srx >= 0 && srx < sx && sry >= 0 && sry < sy && srz >= 0 && srz < sz) {

                                    dst[z*sx*sy + y*sx + x] += src[srz*sx*sy + sry*sx + srx] * ker[r*kw*kw + s*kw + t];
                                }
                                ++t;
                            }
                            ++s;
                        }
                        ++r;
                    }
                }
            }
        }
        break;
    default:
        throw std::invalid_argument("Warning: Invalid mode.");
    }
};



/// \brief 1D convolution function.
///
/// Performs fast convolution on two input signal and kernel f and g
/// and stores the result f*g in given container.
///
/// The procedure uses the FFTW library making it highly efficient.
///
/// Supply convolution plan for fast computation of multiple instances
/// of input data. Be aware that input signal must have same dimensions
/// and mode.
///
/// @param A the system matrix A
/// @param B the right hand side of the LGS, also stores the result
/// @param  src             Linear object source of data
/// @param  dst             Linear object destination of result
/// @param  ker             Linear object kernel
/// @param  sx              Width of signal
/// @param  sy              Height of signal
/// @param  kw              kernel width
/// @param  kcx             kernel center (anchor) x
/// @param  kcy             kernel center (anchor) y
/// @param  mode            Edge mode 0 = reduce image size to fit kernel, 1 = assume zero outside (default)
/// @param  threadMaxCount  Maximum number of allowed threads (default = 4)
/// @param  convPlan        Plan for optimizing FFT transformation for fast convolutions (default = NULL)
template <class T>
void convolution_fftw_1d(const T &src, T &dst, const T &ker, int sx, int kw, int kcx = 0, int mode = 1, int threadMaxCount = 4, convolution_plan* convPlan = NULL) {

    if (mode == 0 && sx-kw+1 <= 0) {
        throw std::invalid_argument("Warning: Valid region is empty. Change input sizes or mode.");
    }
    if (kw > sx) {
        throw std::invalid_argument("Warning: Kernel is bigger than signal");
    }
    if (false) {
        simple_convolution_1d(src, dst, ker, sx, kw, kcx, mode);
    } else {

        unsigned int padded_width;
        bool staticKernel = false;
        if (convPlan != NULL) {
            staticKernel = convPlan->availableKernel();
        }

        if (convPlan == NULL && threadMaxCount > 1) {
            fftw_init_threads();
            fftw_plan_with_nthreads(threadMaxCount);
        }

        switch (mode) {
        case 0:
            padded_width = sx;
            break;
        case 1:
            padded_width = sx + kw - 1;
            break;
        default:
            throw std::invalid_argument("Warning, convolution_fftw_1d: Invalid mode.");
        }

        if (convPlan != NULL && (convPlan->getDim() != 1 || convPlan->getWidth() != padded_width)) {
            throw std::invalid_argument("Warning: Given convolution plan does not match the input dimensions.");
        }

        fftw_complex *aux1, *aux2, *aux3;
        fftw_plan p;
        aux1 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * padded_width);
        aux2 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * padded_width);
        if (!staticKernel){
            aux3 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * padded_width);
        }

        for (int i = 0; i < padded_width; ++i) {

            if (i < sx) {
                aux1[i][0] = src[i];
            } else {
                aux1[i][0] = 0.0;
            }
            aux1[i][1] = 0.0;

            if (!staticKernel){
                if (i < kw) {
                    aux3[i][0] = ker[i];
                } else {
                    aux3[i][0] = 0.0;
                }
                aux3[i][1] = 0.0;
            }
        }

        if (convPlan == NULL) {
            p = fftw_plan_dft_1d(padded_width, aux1, aux2, FFTW_FORWARD, FFTW_ESTIMATE);
        } else {
            p = convPlan->getForwardPlan();
        }

        fftw_execute_dft(p, aux1, aux2);
        if (!staticKernel) {
            fftw_execute_dft(p, aux3, aux1);
        } else {
            fftw_free(aux1);
            aux1 = convPlan->getStaticKernel();
        }

        if (convPlan == NULL) {
            fftw_destroy_plan(p);
        }

        // Multiply as per convolution theorem
        for (int i = 0; i < padded_width; ++i) {

            double imag = aux2[i][0]*aux1[i][1] + aux2[i][1]*aux1[i][0];

            aux2[i][0] = aux2[i][0]*aux1[i][0] - aux2[i][1]*aux1[i][1];
            aux2[i][1] = imag;
        }


        // Perform inverse fft
        if (convPlan == NULL) {
            p = fftw_plan_dft_1d(padded_width, aux2, aux1, FFTW_BACKWARD, FFTW_ESTIMATE);
        } else {
            p = convPlan->getBackwardPlan();
        }

        fftw_execute_dft(p, aux2, aux1);

        if (convPlan == NULL) {
            fftw_destroy_plan(p);
        }
        // Copy back
        switch (mode) {
        case 0:
            for (int i = 0; i < sx-kw+1; ++i) {
                dst[i] = aux1[i+kw-1][0]/(padded_width);
            }
            break;
        case 1:
            for (int i = 0; i < sx; ++i) {
                dst[i] = aux1[i+kcx][0]/(padded_width);
            }
            break;
        default: {
            // This will never happen!
            }
        }

        fftw_free(aux2);
        if (!staticKernel){
            fftw_free(aux1);
            fftw_free(aux3);
        }

        if (convPlan == NULL && threadMaxCount > 1) {
            fftw_cleanup_threads();
        }
    }
};

/// \brief 2D convolution function.
///
/// Performs fast convolution on two input signal and kernel f and g
/// and stores the result f*g in given container.
///
/// The procedure uses the FFTW library making it highly efficient.
///
/// Supply convolution plan for fast computation of multiple instances
/// of input data. Be aware that input signal must have same dimensions
/// and mode.
///
/// @param  src             Linear object source of data
/// @param  dst             Linear object destination of result
/// @param  ker             Linear object kernel
/// @param  sx              Width of signal
/// @param  sy              Height of signal
/// @param  kw              kernel width
/// @param  kcx             kernel center (anchor) x
/// @param  kcy             kernel center (anchor) y
/// @param  mode            Edge mode 0 = reduce image size to fit kernel, 1 = assume zero outside (default)
/// @param  threadMaxCount  Maximum number of allowed threads (default = 4)
/// @param  convPlan        Plan for optimizing FFT transformation for fast convolutions (default = NULL)
template <class T>
void convolution_fftw_2d(const T &src, T &dst, const T &ker, int sx, int sy, int kw, int kcx = 0, int kcy = 0, int mode = 1, int threadMaxCount = 4, convolution_plan* convPlan = NULL) {

    if (mode == 0 && (sx-kw+1 <= 0 || sy-kw+1 <= 0)) {
        throw std::invalid_argument("Warning: Valid region is empty. Change input sizes or mode.");
    }
    if (kw > sx || kw > sy) {
        throw std::invalid_argument("Warning: A least one kernel dimensions is larger than signals");
    }
    if (false) {
        simple_convolution_2d(src, dst, ker, sx, sy, kw, kcx, kcy, mode);
    } else {
        unsigned int padded_width, padded_height;
        bool staticKernel = false;
        if (convPlan != NULL) {
            staticKernel = convPlan->availableKernel();
        }

        if (convPlan == NULL && threadMaxCount > 1) {
            fftw_init_threads();
            fftw_plan_with_nthreads(threadMaxCount);
        }
        switch (mode) {
        case 0:
            if (sy-kw+1 <= 0 || sx-kw+1 <= 0) {
                throw std::invalid_argument("Warning: Valid region is empty. Change input sizes or mode.");
            }
            padded_width = sx;
            padded_height = sy;
            break;
        case 1:
            padded_width = sx + kw - 1;
            padded_height = sy + kw - 1;
            break;
        default:
            throw std::invalid_argument("Warning, convolution_fftw_2d: Invalid mode.");
        }

        if (convPlan != NULL && (convPlan->getDim() != 2 || convPlan->getHeight() != padded_height || convPlan->getWidth() != padded_width)) {
            throw std::invalid_argument("Warning: Given convolution plan does not match the input dimensions.");
        }

        fftw_complex *aux1, *aux2, *aux3;
        fftw_plan p;
        aux1 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * padded_width * padded_height);
        aux2 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * padded_width * padded_height);
        if (!staticKernel){
            aux3 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * padded_width * padded_height);
        }

        for (int j = 0; j < padded_height; ++j) {
            for (int i = 0; i < padded_width; ++i) {

                if (i < sx && j < sy) {
                    aux1[j*padded_width+i][0] = src[j*sx+i];
                } else {
                    aux1[j*padded_width+i][0] = 0.0;
                }
                aux1[j*padded_width+i][1] = 0.0;

                if (!staticKernel) {
                    if (i < kw && j < kw) {
                        aux3[j*padded_width+i][0] = ker[j*kw+i];
                    } else {
                        aux3[j*padded_width+i][0] = 0.0;
                    }
                    aux3[j*padded_width+i][1] = 0.0;
                }
            }
        }
        if (convPlan == NULL) {
            p = fftw_plan_dft_2d(padded_height, padded_width, aux1, aux2, FFTW_FORWARD, FFTW_ESTIMATE);
        } else {
            p = convPlan->getForwardPlan();
        }
        fftw_execute_dft(p, aux1, aux2);
        if (!staticKernel) {
            fftw_execute_dft(p, aux3, aux1);
        } else {
            fftw_free(aux1);
            aux1 = convPlan->getStaticKernel();
        }

        if (convPlan == NULL) {
            fftw_destroy_plan(p);
        }

        // Multiply as per convolution theorem
        for (int i = 0; i < padded_width*padded_height; ++i) {

            double imag = aux2[i][0]*aux1[i][1] + aux2[i][1]*aux1[i][0];

            aux2[i][0] = aux2[i][0]*aux1[i][0] - aux2[i][1]*aux1[i][1];
            aux2[i][1] = imag;
        }

        // Perform inverse fft
        if (convPlan == NULL) {
            p = fftw_plan_dft_2d(padded_height, padded_width, aux2, aux1, FFTW_BACKWARD, FFTW_ESTIMATE);
        } else {
            p = convPlan->getBackwardPlan();
        }
        fftw_execute_dft(p, aux2, aux1);

        if (convPlan == NULL) {
            fftw_destroy_plan(p);
        }
        // Copy back
        switch (mode) {
        case 0:
            for (int j = 0; j < sy-kw+1; ++j) {
                for (int i = 0; i < sx-kw+1; ++i) {
                    dst[j*(sx-kw+1)+i] = aux1[(j+kw-1)*sx+i+kw-1][0]/(padded_width*padded_height);
                }
            }
            break;
        case 1:
            for (int j = 0; j < sy; ++j) {
                for (int i = 0; i < sx; ++i) {
                    dst[j*sx+i] = aux1[(j+kcy)*padded_width+i+kcx][0]/(padded_width*padded_height);
                }
            }
            break;
        default: {
            // This will never happen!
            }
        }

        fftw_free(aux2);
        if (!staticKernel){
            fftw_free(aux1);
            fftw_free(aux3);
        }

        if (convPlan == NULL && threadMaxCount > 1) {
            fftw_cleanup_threads();
        }
    }
};

/// \brief 3D convolution function.
///
/// Performs fast convolution on two input signal and kernel f and g
/// and stores the result f*g in given container.
///
/// The procedure uses the FFTW library making it highly efficient.
///
/// Supply convolution plan for fast computation of multiple instances
/// of input data. Be aware that input signal must have same dimensions
/// and mode.
///
/// @param  src             Linear object source of data
/// @param  dst             Linear object destination of result
/// @param  ker             Linear object kernel
/// @param  sx              Width of signal
/// @param  sy              Height of signal
/// @param  sz              Height of signal
/// @param  kw              kernel width
/// @param  kcx             kernel center (anchor) x
/// @param  kcy             kernel center (anchor) y
/// @param  kcz             kernel center (anchor) z
/// @param  mode            Edge mode 0 = reduce image size to fit kernel, 1 = assume zero outside (default = 1)
/// @param  threadMaxCount  Maximum number of allowed threads (default = 4)
template <class T>
void convolution_fftw_3d(const T &src, T &dst, const T &ker, int sx, int sy, int sz, int kw, int kcx = 0, int kcy = 0, int kcz = 0, int mode = 1, int threadMaxCount = 4, convolution_plan* convPlan = NULL) {

    if (mode == 0 && (sx-kw+1 <= 0 || sy-kw+1 <= 0 || sz-kw+1 <= 0)) {
        throw std::invalid_argument("Warning: Valid region is empty. Change input sizes or mode.");
    }
    if (kw > sx || kw > sy || kw > sz) {
        throw std::invalid_argument("Warning: A least one kernel dimensions is larger than signals");
    }
    if (false) {
        simple_convolution_3d(src, dst, ker, sx, sy, sz, kw, kcx, kcy, kcz, mode);
    } else {
        unsigned int padded_width, padded_height, padded_depth;
        bool staticKernel = false;
        if (convPlan != NULL) {
            staticKernel = convPlan->availableKernel();
        }

        if (convPlan == NULL && threadMaxCount > 1) {
            fftw_init_threads();
            fftw_plan_with_nthreads(threadMaxCount);
        }

        if (convPlan != NULL && convPlan->getDim() != 3) {
            throw std::invalid_argument("Warning: Given convolution plan has incorrect dimensions.");
        }

        switch (mode)
            {
        case 0:
            if (sy-kw+1 <= 0 || sx-kw+1 <= 0) {
                throw std::invalid_argument("Warning: Valid region is empty. Change input sizes or mode.");
            }
            padded_width = sx;
            padded_height = sy;
            padded_depth = sz;

            break;
        case 1:

            padded_width = sx + kw - 1;
            padded_height = sy + kw - 1;
            padded_depth = sz + kw - 1;

            break;
        default:
            throw std::invalid_argument("Warning: convolution_fftw_3d: Invalid mode.");
        }

        if (convPlan != NULL && (convPlan->getDim() != 3 || convPlan->getHeight() != padded_height || convPlan->getWidth() != padded_width || convPlan->getDepth() != padded_depth)) {
            throw std::invalid_argument("Warning: Given convolution plan does not match the input dimensions.");
        }

        fftw_complex *aux1, *aux2, *aux3;
        fftw_plan p;

        aux1 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * padded_width * padded_height * padded_depth);
        aux2 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * padded_width * padded_height * padded_depth);
        if (!staticKernel){
            aux3 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * padded_width * padded_height * padded_depth);
        }

        for (int k = 0; k < padded_depth; ++k) {
            for (int j = 0; j < padded_height; ++j) {
                for (int i = 0; i < padded_width; ++i) {

                    if (i < sx && j < sy && k < sz) {
                        aux1[k*padded_height*padded_width + j*padded_width + i][0] = src[k*sx*sy+j*sx+i];
                    } else {
                        aux1[k*padded_height*padded_width + j*padded_width + i][0] = 0.0;
                    }
                    aux1[k*padded_height*padded_width + j*padded_width + i][1] = 0.0;

                    if (!staticKernel) {
                        if (i < kw && j < kw && k < kw) {
                            aux3[k*padded_height*padded_width + j*padded_width + i][0] = ker[k*kw*kw+j*kw+i];
                        } else {
                            aux3[k*padded_height*padded_width + j*padded_width + i][0] = 0.0;
                        }
                        aux3[k*padded_height*padded_width + j*padded_width + i][1] = 0.0;
                    }
                }
            }
        }

        if (convPlan == NULL) {
            p = fftw_plan_dft_3d(padded_depth, padded_height, padded_width, NULL, NULL, FFTW_FORWARD, FFTW_ESTIMATE);
        } else {
            p = convPlan->getForwardPlan();
        }

        fftw_execute_dft(p, aux1, aux2);

        if (!staticKernel) {
            fftw_execute_dft(p, aux3, aux1);
        } else {
            fftw_free(aux1);
            aux1 = convPlan->getStaticKernel();
        }

        if (convPlan == NULL) {
            fftw_destroy_plan(p);
        }


        // Multiply as per convolution theorem
        for (int i = 0; i < padded_width*padded_height*padded_depth; ++i) {

            double imag = aux2[i][0]*aux1[i][1] + aux2[i][1]*aux1[i][0];

            aux2[i][0] = aux2[i][0]*aux1[i][0] - aux2[i][1]*aux1[i][1];
            aux2[i][1] = imag;
        }

        // Perform inverse fft
        if (convPlan == NULL) {
            p = fftw_plan_dft_3d(padded_depth, padded_height, padded_width, aux2, aux1, FFTW_BACKWARD, FFTW_ESTIMATE);
        } else {
            p = convPlan->getBackwardPlan();
        }

        fftw_execute_dft(p, aux2, aux1);

        if (convPlan == NULL) {
            fftw_destroy_plan(p);
        }


        // Copy back
        switch (mode) {
        case 0:
            for (int k = 0; k < sz-kw+1; ++k) {
                for (int j = 0; j < sy-kw+1; ++j) {
                    for (int i = 0; i < sx-kw+1; ++i) {
                        dst[k*(sx-kw+1)*(sy-kw+1) + j*(sx-kw+1) + i] = aux1[(k+kw-1)*sx*sy + (j+kw-1)*sx + i+kw-1][0]/(padded_width*padded_height*padded_depth);
                    }
                }
            }
            break;
        case 1:
            for (int k = 0; k < sz; ++k) {
                for (int j = 0; j < sy; ++j) {
                    for (int i = 0; i < sx; ++i) {
                        dst[k*sx*sy + j*sx + i] = aux1[padded_width*padded_height*(k+kcz) + (j+kcy)*padded_width + i+kcx][0]/(padded_width*padded_height*padded_depth);
                    }
                }
            }
            break;
        default: {
            // This will never happen!
            }
        }

        fftw_free(aux2);

        if (!staticKernel){
            fftw_free(aux1);
            fftw_free(aux3);
        }

        if (convPlan == NULL && threadMaxCount > 1) {
            fftw_cleanup_threads();
        }
    }
};

} // namespace esTools

#endif // CONVOLUTION_FFTW_HPP