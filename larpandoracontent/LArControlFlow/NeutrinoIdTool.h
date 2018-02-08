/**
 *  @file   larpandoracontent/LArControlFlow/NeutrinoIdTool.h
 *
 *  @brief  Header file for the neutrino id tool class.
 *
 *  $Log: $
 */
#ifndef LAR_NEUTRINO_ID_TOOL_H
#define LAR_NEUTRINO_ID_TOOL_H 1

#include "larpandoracontent/LArControlFlow/MasterAlgorithm.h"

#include "larpandoracontent/LArObjects/LArSupportVectorMachine.h"

namespace lar_content
{

/**
 *  @brief  NeutrinoIdTool class
 *
 *  Compares the neutrino and cosmic hypotheses of all of the slices in the event. Uses an SVM to calculate the probability of each slice 
 *  containing a neutrino interaction. If the slice with the highest probability is sufficiently probable, then identify it as the neutrino 
 *  slice, otherwise assume all slices are cosmogenic.
 *
 *  If training mode is switched on, then the tool will write SVM training exmples to the specified output file. The events selected for 
 *  training must pass (user congigurable) slicing quality cuts. Users may also select events based on their interaction type (nuance code).
 */
class NeutrinoIdTool : public SliceIdBaseTool
{
public:
    /**
     *  @brief  Default constructor
     */
    NeutrinoIdTool();

    void SelectOutputPfos(const MasterAlgorithm *const pAlgorithm, const SliceHypotheses &nuSliceHypotheses, const SliceHypotheses &crSliceHypotheses, pandora::PfoList &selectedPfos);

private:
    /**
     *  @brief  Slice features class
     */
    class SliceFeatures
    {
    public:
        /**
         *  @brief  Constructor
         */
        SliceFeatures(const pandora::PfoList &nuPfos, const pandora::PfoList &crPfos, const NeutrinoIdTool *const pTool);

        /**
         *  @brief  Check if all features were calculable
         */
        bool IsFeatureVectorAvailable() const;

        /**
         *  @brief  Get the feature vector for the SVM
         */
        SupportVectorMachine::DoubleVector GetFeatureVector() const;
    private:
        /**
         *  @brief  Get the recontructed neutrino the input list of neutrino Pfos
         */
        const pandora::ParticleFlowObject *GetNeutrino(const pandora::PfoList &nuPfos);

        /**
         *  @brief  Get the 3D space points in a given pfo
         */
        void GetSpacePoints(const pandora::ParticleFlowObject *const pPfo, pandora::CartesianPointVector &spacePoints) const;

        /**
         *  @brief  Use a sliding fit to get the direction of a collection of spacepoint near a vertex position
         */
        pandora::CartesianVector GetDirectionFromVertex(const pandora::CartesianPointVector &spacePoints, const pandora::CartesianVector &vertex) const;

        bool                               m_isAvailable;    ///< Is the feature vector available
        SupportVectorMachine::DoubleVector m_featureVector;  ///< The SVM feature vector
        const NeutrinoIdTool *const        m_pTool;          ///< The tool that owns this
    };

    /**
     *  @brief  Get the slice with the most neutrino induced hits
     */
    unsigned int GetBestSliceIndex(const SliceHypotheses &nuSliceHypotheses, const SliceHypotheses &crSliceHypotheses, float &purity, float &completeness) const;

    /**
     *  @brief  Collect all 2D hits in a supplied list of Pfos and push them on to an existing hit list, check so not to double count
     */
    void Collect2DHits(const pandora::PfoList &pfos, pandora::CaloHitList &hitList) const;

    /**
     *  @brief  Count the number of neutrino induced hits in a given list
     */
    unsigned int CountNeutrinoInducedHits(const pandora::CaloHitList &hitList) const;

    /**
     *  @brief  Use the current MCParticle list to get the nuance code of the neutrino in the event
     */
    int GetNuanceCode(const MasterAlgorithm *const pAlgorithm) const;

    pandora::StatusCode ReadSettings(const pandora::TiXmlHandle xmlHandle);

    // Training
    bool         m_useTrainingMode;      ///< Should use training mode. If true, training examples will be written to the output file
    std::string  m_trainingOutputFile;   ///< Output file name for training examples
    bool         m_selectNuanceCode;     ///< Should select training events by nuance code
    int          m_nuance;               ///< Nuance code to select for training
    float        m_minPurity;            ///< Minimum purity of the best slice to use event for training
    float        m_minCompleteness;      ///< Minimum completeness of the best slice to use event for training

    // Classification
    float        m_minProbability;       ///< Minimum probability required to classify a slice as the neutrino
};

} // namespace lar_content

#endif // #ifndef LAR_NEUTRINO_ID_TOOL_H
