/**
 *  @file   LArContent/include/LArThreeDReco/LArEventBuilding/ShowerParticleBuildingAlgorithm.h
 *
 *  @brief  Header file for the neutrino event creation algorithm class.
 *
 *  $Log: $
 */
#ifndef LAR_SHOWER_PARTICLE_BUILDING_ALGORITHM_H
#define LAR_SHOWER_PARTICLE_BUILDING_ALGORITHM_H 1

#include "LArObjects/LArShowerPfo.h"

#include "LArThreeDReco/LArEventBuilding/CustomParticleCreationAlgorithm.h"

namespace lar_content
{

/**
 *  @brief  ShowerParticleBuildingAlgorithm class
 */
class ShowerParticleBuildingAlgorithm : public CustomParticleCreationAlgorithm
{
public:
    /**
     *  @brief  Factory class for instantiating algorithm
     */
    class Factory : public pandora::AlgorithmFactory
    {
    public:
        pandora::Algorithm *CreateAlgorithm() const;
    };

private:
    pandora::StatusCode ReadSettings(const pandora::TiXmlHandle xmlHandle);

    void CreatePfo(const pandora::ParticleFlowObject *const pInputPfo, const pandora::ParticleFlowObject*& pOutputPfo) const;
};

//------------------------------------------------------------------------------------------------------------------------------------------

inline pandora::Algorithm *ShowerParticleBuildingAlgorithm::Factory::CreateAlgorithm() const
{
    return new ShowerParticleBuildingAlgorithm();
}

} // namespace lar_content

#endif // #ifndef LAR_SHOWER_PARTICLE_BUILDING_ALGORITHM_H
