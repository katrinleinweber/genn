#pragma once

// Standard C++ includes
#include <functional>
#include <map>
#include <memory>
#include <vector>

// Simulator includes
#include "networkClient.h"

// Forward declarations
namespace pugi
{
    class xml_node;
}

//----------------------------------------------------------------------------
// SpineMLSimulator::InputValue
//----------------------------------------------------------------------------
namespace SpineMLSimulator
{
namespace InputValue
{
class Base
{
public:
    virtual ~Base(){}

    //------------------------------------------------------------------------
    // Declared virtuals
    //------------------------------------------------------------------------
    virtual void update(double dt, unsigned int timestep,
                        std::function<void(unsigned int, double)> applyValueFunc) = 0;

protected:
    Base(unsigned int numNeurons, const pugi::xml_node &node);

    //------------------------------------------------------------------------
    // Protected API
    //------------------------------------------------------------------------
    const std::vector<unsigned int> &getTargetIndices() const{ return m_TargetIndices; }
    unsigned int getNumNeurons() const{ return m_NumNeurons; }

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    unsigned int m_NumNeurons;
    std::vector<unsigned int> m_TargetIndices;
};

//----------------------------------------------------------------------------
// SpineMLSimulator::InputValue::ScalarBase
//----------------------------------------------------------------------------
class ScalarBase : public Base
{
protected:
    ScalarBase(unsigned int numNeurons, const pugi::xml_node &node) : Base(numNeurons, node){}

    //----------------------------------------------------------------------------
    // Protected API
    //----------------------------------------------------------------------------
    void applyScalar(double value,
                     std::function<void(unsigned int, double)> applyValueFunc) const;
};

//----------------------------------------------------------------------------
// SpineMLSimulator::InputValue::Constant
//----------------------------------------------------------------------------
class Constant : public ScalarBase
{
public:
    Constant(double dt, unsigned int numNeurons, const pugi::xml_node &node);

    //------------------------------------------------------------------------
    // InputValue virtuals
    //------------------------------------------------------------------------
    virtual void update(double dt, unsigned int timestep,
                        std::function<void(unsigned int, double)> applyValueFunc) override;
private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    // Value to apply to targetted neurons throughout simulation
    double m_Value;
};

//----------------------------------------------------------------------------
// SpineMLSimulator::InputValue::ConstantArray
//----------------------------------------------------------------------------
class ConstantArray : public Base
{
public:
    ConstantArray(double dt, unsigned int numNeurons, const pugi::xml_node &node);

    //------------------------------------------------------------------------
    // InputValue virtuals
    //------------------------------------------------------------------------
    virtual void update(double dt, unsigned int timestep,
                        std::function<void(unsigned int, double)> applyValueFunc) override;
private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    // Values to apply to targetted neurons throughout simulation
    std::vector<double> m_Values;
};

//----------------------------------------------------------------------------
// SpineMLSimulator::InputValue::TimeVarying
//----------------------------------------------------------------------------
class TimeVarying : public ScalarBase
{
public:
    TimeVarying(double dt, unsigned int numNeurons, const pugi::xml_node &node);

    //------------------------------------------------------------------------
    // InputValue virtuals
    //------------------------------------------------------------------------
    virtual void update(double dt, unsigned int timestep,
                        std::function<void(unsigned int, double)> applyValueFunc) override;
private:
    //----------------------------------------------------------------------------
    // Members
    //----------------------------------------------------------------------------
    // Values to apply to all neurons at certain timesteps
    std::map<unsigned int, double> m_TimeValues;
};

//----------------------------------------------------------------------------
// SpineMLSimulator::InputValue::TimeVaryingArray
//----------------------------------------------------------------------------
class TimeVaryingArray : public Base
{
public:
    TimeVaryingArray(double dt, unsigned int numNeurons, const pugi::xml_node &node);

    //------------------------------------------------------------------------
    // InputValue virtuals
    //------------------------------------------------------------------------
    virtual void update(double dt, unsigned int timestep,
                        std::function<void(unsigned int, double)> applyValueFunc) override;

private:
    //------------------------------------------------------------------------
    // Typedefines
    //------------------------------------------------------------------------
    typedef std::vector<std::pair<unsigned int, double>> NeuronValueVec;

    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    // Vector of neurons and values to apply at certain timesteps
    std::map<unsigned int, NeuronValueVec> m_TimeArrays;
};

//----------------------------------------------------------------------------
// SpineMLSimulator::InputValue::External
//----------------------------------------------------------------------------
class External : public Base
{
public:
    External(double dt, unsigned int numNeurons, const pugi::xml_node &node);

    //------------------------------------------------------------------------
    // InputValue virtuals
    //------------------------------------------------------------------------
    virtual void update(double dt, unsigned int timestep,
                        std::function<void(unsigned int, double)> applyValueFunc) override;

private:
    //------------------------------------------------------------------------
    // Members
    //------------------------------------------------------------------------
    NetworkClient m_Client;
    std::vector<double> m_Buffer;

    // How many GeNN timesteps do we wait before updating
    unsigned int m_IntervalTimesteps;

    // Count down to next time we update
    unsigned int m_CurrentIntervalTimesteps;
};

//----------------------------------------------------------------------------
// Functions
//----------------------------------------------------------------------------
std::unique_ptr<Base> create(double dt, unsigned int numNeurons, const pugi::xml_node &node);

}   // namespace InputValue
}   // namespace SpineMLSimulator