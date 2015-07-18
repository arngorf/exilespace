
#include "convolution_fftw.hpp"

namespace esTools {

int convolution_plan::dim;
int convolution_plan::width;
int convolution_plan::height;
int convolution_plan::depth;
int convolution_plan::kw;
int convolution_plan::threadMaxCount;

convolution_plan::convolution_plan(int width, int kw, int mode, int threadMaxCount) {

    switch (mode) {
    case 0:
        this->width = width;
        break;
    case 1:
        this->width = width + kw - 1;
        break;
    default:
        throw std::invalid_argument("Warning: 1d convolution plan: Invalid mode");
    }

    if (threadMaxCount > 1) {
        fftw_init_threads(); // This MUST come before all other fftw calls
        fftw_plan_with_nthreads(threadMaxCount);
    }

    this->height = 1;
    this->depth = 1;
    this->dim = 1;
    this->kw = kw;
    this->threadMaxCount = threadMaxCount;
    fftw_complex* benchmarkArray1 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * this->width);
    fftw_complex* benchmarkArray2 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * this->width);

    this->forwardPlan = fftw_plan_dft_1d(this->width, benchmarkArray1, benchmarkArray2, FFTW_FORWARD, FFTW_MEASURE);
    this->backwardPlan = fftw_plan_dft_1d(this->width, benchmarkArray1, benchmarkArray2, FFTW_BACKWARD, FFTW_MEASURE);


    fftw_free(benchmarkArray1);
    fftw_free(benchmarkArray2);

    this->staticKernel = NULL;
}

convolution_plan::convolution_plan(int width, int height, int kw, int mode, int threadMaxCount) {

    switch (mode) {
    case 0:
        this->width = width;
        this->height = height;
        break;
    case 1:
        this->width = width + kw - 1;
        this->height = height + kw - 1;
        break;
    default:
        throw std::invalid_argument("Warning: 2d convolution plan: Invalid mode");
    }

    if (threadMaxCount > 1) {
        fftw_init_threads(); // This MUST come before all other fftw calls
        fftw_plan_with_nthreads(threadMaxCount);
    }

    this->depth = 1;
    this->dim = 2;
    this->kw = kw;
    this->threadMaxCount = threadMaxCount;
    fftw_complex* benchmarkArray1 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * this->width * this->height);
    fftw_complex* benchmarkArray2 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * this->width * this->height);

    this->forwardPlan = fftw_plan_dft_2d(this->height, this->width, benchmarkArray1, benchmarkArray2, FFTW_FORWARD, FFTW_MEASURE);
    this->backwardPlan = fftw_plan_dft_2d(this->height, this->width, benchmarkArray1, benchmarkArray2, FFTW_BACKWARD, FFTW_MEASURE);

    fftw_free(benchmarkArray1);
    fftw_free(benchmarkArray2);

    this->staticKernel = NULL;
}

convolution_plan::convolution_plan(int width, int height, int depth, int kw, int mode, int threadMaxCount) {

    switch (mode) {
    case 0:
        this->width = width;
        this->height = height;
        this->depth = depth;
        break;
    case 1:
        this->width = width + kw - 1;
        this->height = height + kw - 1;
        this->depth = depth + kw - 1;
        break;
    default:
        std::cout << mode << std::endl;
        throw std::invalid_argument("Warning: 3d convolution plan: Invalid mode");
    }

    if (threadMaxCount > 1) {
        fftw_init_threads(); // This MUST come before all other fftw calls
        fftw_plan_with_nthreads(threadMaxCount);
    }

    this->dim = 3;
    this->kw = kw;
    this->threadMaxCount = threadMaxCount;
    fftw_complex* benchmarkArray1 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * this->width * this->height * this->depth);
    fftw_complex* benchmarkArray2 = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * this->width * this->height * this->depth);

    this->forwardPlan = fftw_plan_dft_3d(this->depth, this->height, this->width, benchmarkArray1, benchmarkArray2, FFTW_FORWARD, FFTW_MEASURE);
    this->backwardPlan = fftw_plan_dft_3d(this->depth, this->height, this->width, benchmarkArray1, benchmarkArray2, FFTW_BACKWARD, FFTW_MEASURE);

    fftw_free(benchmarkArray1);
    fftw_free(benchmarkArray2);

    this->staticKernel = NULL;
}

convolution_plan::~convolution_plan() {

    this->unsetStaticKernel();

    fftw_destroy_plan(this->forwardPlan);
    fftw_destroy_plan(this->backwardPlan);

    if (this->threadMaxCount > 1) {
        fftw_cleanup_threads(); // This MUST come after all other fftw calls
    }
}

void convolution_plan::unsetStaticKernel() {
    if (this->staticKernel != NULL) {
        fftw_free(this->staticKernel);
    }
    this->staticKernel = NULL;

}

} // namespace shark