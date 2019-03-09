#pragma once

// Standard C++ includes
#include <functional>
#include <map>
#include <string>

// GeNN code generator includes
#include "code_generator/backendBase.h"

// Forward declarations
namespace filesystem
{
    class path;
}

//--------------------------------------------------------------------------
// CodeGenerator::SingleThreadedCPU::Backend
//--------------------------------------------------------------------------
namespace CodeGenerator
{
namespace SingleThreadedCPU
{
class Backend : public BackendBase
{
public:
    Backend(int localHostID, const Preferences &preferences)
    :   m_LocalHostID(localHostID), m_Preferences(preferences)
    {
    }

    //--------------------------------------------------------------------------
    // CodeGenerator::BackendBase virtuals
    //--------------------------------------------------------------------------
    virtual void genNeuronUpdate(CodeStream &os, const ModelSpec &model, NeuronGroupSimHandler simHandler, NeuronGroupHandler wuVarUpdateHandler) const override;

    virtual void genSynapseUpdate(CodeStream &os, const ModelSpec &model,
                                  SynapseGroupHandler wumThreshHandler, SynapseGroupHandler wumSimHandler, SynapseGroupHandler wumEventHandler,
                                  SynapseGroupHandler postLearnHandler, SynapseGroupHandler synapseDynamicsHandler) const override;

    virtual void genInit(CodeStream &os, const ModelSpec &model,
                         NeuronGroupHandler localNGHandler, NeuronGroupHandler remoteNGHandler,
                         SynapseGroupHandler sgDenseInitHandler, SynapseGroupHandler sgSparseConnectHandler, 
                         SynapseGroupHandler sgSparseInitHandler) const override;

    virtual void genDefinitionsPreamble(CodeStream &os) const override;
    virtual void genDefinitionsInternalPreamble(CodeStream &os) const override;
    virtual void genRunnerPreamble(CodeStream &os) const override;
    virtual void genAllocateMemPreamble(CodeStream &os, const ModelSpec &model) const override;

    virtual void genVariableDefinition(CodeStream &definitions, CodeStream &definitionsInternal, const std::string &type, const std::string &name, VarLocation loc) const override;
    virtual void genVariableImplementation(CodeStream &os, const std::string &type, const std::string &name, VarLocation loc) const override;
    virtual void genVariableAllocation(CodeStream &os, const std::string &type, const std::string &name, VarLocation loc, size_t count) const override;
    virtual void genVariableFree(CodeStream &os, const std::string &name, VarLocation loc) const override;

    virtual void genPopVariableInit(CodeStream &os, VarLocation loc, const Substitutions &kernelSubs, Handler handler) const override;
    virtual void genVariableInit(CodeStream &os, VarLocation loc, size_t count, const std::string &indexVarName,
                                 const Substitutions &kernelSubs, Handler handler) const override;
    virtual void genSynapseVariableRowInit(CodeStream &os, VarLocation loc, const SynapseGroup &sg,
                                           const Substitutions &kernelSubs, Handler handler) const override;

    virtual void genCurrentTrueSpikePush(CodeStream &os, const NeuronGroup &ng) const override;
    virtual void genCurrentTrueSpikePull(CodeStream &os, const NeuronGroup &ng) const override;
    virtual void genCurrentSpikeLikeEventPush(CodeStream &os, const NeuronGroup &ng) const override;
    virtual void genCurrentSpikeLikeEventPull(CodeStream &os, const NeuronGroup &ng) const override;

    virtual void genVariablePush(CodeStream &os, const std::string &type, const std::string &name, VarLocation loc, bool autoInitialized, size_t count) const override;
    virtual void genVariablePull(CodeStream &os, const std::string &type, const std::string &name, VarLocation loc, size_t count) const override;

    virtual void genGlobalRNG(CodeStream &definitions, CodeStream &definitionsInternal, CodeStream &runner, CodeStream &allocations, CodeStream &free, const ModelSpec &model) const override;
    virtual void genPopulationRNG(CodeStream &definitions, CodeStream &definitionsInternal, CodeStream &runner, CodeStream &allocations, CodeStream &free,
                                  const std::string &name, size_t count) const override;

    virtual void genMakefilePreamble(std::ostream &os) const override;
    virtual void genMakefileLinkRule(std::ostream &os) const override;
    virtual void genMakefileCompileRule(std::ostream &os) const override;

    virtual void genMSBuildConfigProperties(std::ostream &os) const override;
    virtual void genMSBuildImportProps(std::ostream &os) const override;
    virtual void genMSBuildItemDefinitions(std::ostream &os) const override;
    virtual void genMSBuildCompileModule(const std::string &moduleName, std::ostream &os) const override;
    virtual void genMSBuildImportTarget(std::ostream &os) const override;

    virtual std::string getVarPrefix() const override{ return ""; }

    virtual bool isGlobalRNGRequired(const ModelSpec &model) const override;

private:
    //--------------------------------------------------------------------------
    // Private methods
    //--------------------------------------------------------------------------
    void genPresynapticUpdate(CodeStream &os, const SynapseGroup &sg, const Substitutions &popSubs, bool trueSpike,
                              SynapseGroupHandler wumThreshHandler, SynapseGroupHandler wumSimHandler) const;

    void genEmitSpike(CodeStream &os, const NeuronGroup &ng, const Substitutions &subs, bool trueSpike) const;

  
    //--------------------------------------------------------------------------
    // Members
    //--------------------------------------------------------------------------
    const int m_LocalHostID;
    const BackendBase::Preferences m_Preferences;
};
}   // namespace SingleThreadedCPU
}   // namespace CodeGenerator
