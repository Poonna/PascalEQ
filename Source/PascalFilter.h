/*
  ==============================================================================

    PascalFilter.h
    Created: 2 Apr 2020 1:29:06pm
    Author:  Poonna

    PascalFilter base class with a couple of sample derived filter classes.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

#include "PascalMatrix.h"

template <typename FloatT>
class PascalFilter
{
public:
    PascalFilter(int order, FloatT fs, FloatT fc = 1000.0f, FloatT gain_dB = 0.0f, FloatT q = defaultQ);

    void setParameters(FloatT fc, FloatT gain_dB, FloatT q = defaultQ);
    void setSampleRate(FloatT fs);

    void resetBuffers();
    void processBlock(FloatT* buffer, size_t length);

    static constexpr FloatT defaultQ = FloatT(1.0) / MathConstants<FloatT>::sqrt2;

protected:
    virtual void computeCoefficients(FloatT omega, FloatT gain_lin, FloatT q, dsp::Matrix<FloatT>& b_, dsp::Matrix<FloatT>& a_) = 0;

    const dsp::Matrix<FloatT> pascalMatrix;

    const int filterOrder;

    FloatT sampleRate;

    // Filter coefficients
    dsp::Matrix<FloatT> a;
    dsp::Matrix<FloatT> b;

private:
    // Back buffers
    std::unique_ptr<FloatT[]> xi_;
    std::unique_ptr<FloatT[]> yi_;

    // Last parameters
    FloatT fc;
    FloatT gain_dB;
    FloatT q;

    bool parametersChanged;
};

template <typename FloatT>
PascalFilter<FloatT>::PascalFilter(int order, FloatT fs, FloatT fc, FloatT gain_dB, FloatT q) :
    a(order + 1, 1),
    b(order + 1, 1),
    filterOrder{ order },
    xi_{ std::make_unique<FloatT[]>(order + 1) },
    yi_{ std::make_unique<FloatT[]>(order + 1) },
    pascalMatrix{ PascalMatrix<FloatT>::create(order + 1) }
{
    sampleRate = fs;
    setParameters(fc, gain_dB, q);
    resetBuffers();
}

template <typename FloatT>
void PascalFilter<FloatT>::setParameters(FloatT fc, FloatT gain_dB, FloatT q)
{
    this->fc = fc;
    this->gain_dB = gain_dB;
    this->q = q;

    parametersChanged = true;
}

template <typename FloatT>
void PascalFilter<FloatT>::setSampleRate(FloatT fs)
{
    if (fs != sampleRate)
    {
        sampleRate = fs;
        parametersChanged = true;
        resetBuffers();
    }
}

template <typename FloatT>
void PascalFilter<FloatT>::resetBuffers()
{
    for (auto i = 0; i <= filterOrder; ++i)
    {
        xi_[i] = 0.0f;
        yi_[i] = 0.0f;
    }
}

template <typename FloatT>
void PascalFilter<FloatT>::processBlock(FloatT* buffer, size_t numSamples)
{
    if (parametersChanged)
    {
        // Analog-domain coefficients
        dsp::Matrix<FloatT> a_(filterOrder + 1, 1);
        dsp::Matrix<FloatT> b_(filterOrder + 1, 1);

        const FloatT gain_lin = Decibels::decibelsToGain(gain_dB); // linear gain
        const FloatT omega = tan(MathConstants<FloatT>::pi * fc / sampleRate); // normalized frequency parameter

        computeCoefficients(omega, gain_lin, q, b_, a_);

        b = pascalMatrix * b_;
        a = pascalMatrix * a_;

        b *= 1.0f / a(0, 0);
        a *= 1.0f / a(0, 0);

        parametersChanged = false;
    }

    for (auto i = 0; i < numSamples; ++i)
    {
        xi_[0] = buffer[i];
        yi_[0] = b(0, 0) * xi_[0];
        for (auto j = 1; j <= filterOrder; ++j)
        {
            yi_[0] += b(j, 0) * xi_[j];
            yi_[0] -= a(j, 0) * yi_[j];
        }
        buffer[i] = yi_[0];

        for (auto j = filterOrder; j > 0; --j)
        {
            xi_[j] = xi_[j - 1];
            yi_[j] = yi_[j - 1];
        }
    }
}

//==============================================================================
// Sample filter classes
//==============================================================================

template <typename FloatT>
class PeakFilter : public PascalFilter<FloatT>
{
public:
    PeakFilter(FloatT fs) : PeakFilter(fs, 1000.0f, 0.0f, defaultQ) {}
    PeakFilter(FloatT fs, FloatT fc, FloatT gain_dB, FloatT q) : PascalFilter(2, fs, fc, gain_dB, q) {}

    void computeCoefficients(FloatT omega, FloatT gain_lin, FloatT q, dsp::Matrix<FloatT>& b_, dsp::Matrix<FloatT>& a_) override
    {
        if (gain_lin > 1.0f)
        {
            // Boost
            b_(0, 0) = omega * omega;
            b_(1, 0) = gain_lin * omega / q;
            b_(2, 0) = 1.0f;

            a_(0, 0) = omega * omega;
            a_(1, 0) = omega / q;
            a_(2, 0) = 1.0f;
        }
        else
        {
            // Cut
            b_(0, 0) = omega * omega;
            b_(1, 0) = omega / q;
            b_(2, 0) = 1.0f;

            a_(0, 0) = omega * omega;
            a_(1, 0) = omega / (gain_lin * q);
            a_(2, 0) = 1.0f;
        }
    }
};

template <typename FloatT>
class LowShelfFilter : public PascalFilter<FloatT>
{
public:
    LowShelfFilter(FloatT fs) : LowShelfFilter(fs, 80.0f, 0.0f) {}
    LowShelfFilter(FloatT fs, FloatT fc, FloatT gain_dB) : PascalFilter(2, fs, fc, gain_dB) {}

    void computeCoefficients(FloatT omega, FloatT gain_lin, FloatT q, dsp::Matrix<FloatT>& b_, dsp::Matrix<FloatT>& a_) override
    {
        if (gain_lin > 1.0f)
        {
            // Boost
            b_(0, 0) = gain_lin * omega * omega;
            b_(1, 0) = sqrt(2.0f * gain_lin) * omega;
            b_(2, 0) = 1.0f;

            a_(0, 0) = omega * omega;
            a_(1, 0) = MathConstants<FloatT>::sqrt2 * omega;
            a_(2, 0) = 1.0f;
        }
        else
        {
            // Cut
            b_(0, 0) = omega * omega;
            b_(1, 0) = MathConstants<FloatT>::sqrt2 * omega;
            b_(2, 0) = 1.0f;

            a_(0, 0) = omega * omega / gain_lin;
            a_(1, 0) = sqrt(2.0f / gain_lin) * omega;
            a_(2, 0) = 1.0f;
        }
    }
};

template <typename FloatT>
class HighShelfFilter : public PascalFilter<FloatT>
{
public:
    HighShelfFilter(FloatT fs) : HighShelfFilter(fs, 16000.0f, 0.0f) {}
    HighShelfFilter(FloatT fs, FloatT fc, FloatT gain_dB) : PascalFilter(2, fs, fc, gain_dB) {}

    void computeCoefficients(FloatT omega, FloatT gain_lin, FloatT q, dsp::Matrix<FloatT>& b_, dsp::Matrix<FloatT>& a_) override
    {
        if (gain_lin > 1.0f)
        {
            // Boost
            b_(0, 0) = omega * omega;
            b_(1, 0) = sqrt(2.0f * gain_lin) * omega;
            b_(2, 0) = gain_lin;

            a_(0, 0) = omega * omega;
            a_(1, 0) = MathConstants<FloatT>::sqrt2 * omega;
            a_(2, 0) = 1.0f;
        }
        else
        {
            // Cut
            b_(0, 0) = omega * omega;
            b_(1, 0) = MathConstants<FloatT>::sqrt2 * omega;
            b_(2, 0) = 1.0f;

            a_(0, 0) = omega * omega;
            a_(1, 0) = sqrt(2.0f / gain_lin) * omega;
            a_(2, 0) = 1.0f / gain_lin;
        }
    }
};