//-----------------------------------------------------
// Objective:   Model a region in an assembly file.
//
// History:     2012/10/7 - B. Fung (Created)
// 
//-----------------------------------------------------

#pragma once

#include "CSFeatureVector.h"

class CCSAssemblyFunction;
class CCSRegionMgr;
class CCSAssemblyFileMgr;

class CCSRegion
{
public:
    CCSRegion(CCSAssemblyFunction* pFcn, int startIdx, int endIdx, int rawStartIdx, int rawEndIdx);
    CCSRegion(int dbRegionID, int dbFcnID, int dbFileID, int startIdx, int endIdx, int rawStartIdx, int rawEndIdx);
    virtual ~CCSRegion();

    inline CCSAssemblyFunction* getFunction() const { return m_pFcn; };
    inline const CCSFeatureVector& getVector() const { return m_vector; };
    inline const CCSBoolArray& getBinaryVector() const { return m_binaryVector; };

    bool countRegionFeatures(int kThreshold, CCSAssemblyFileMgr* pAssemblyFileMgr);
    bool constructVector(CStringArray& globaFeatures);
    bool constructBinaryVector(const CCSIntArray& globalMedians);

    int m_dbRegionID;       // regionID in database
    int m_dbFcnID;          // fcnID in database
    int m_dbFileID;         // fileID in database
    int m_startIdx;         // starting point of a region, inclusive
    int m_endIdx;           // ending point of a region, inclusive        
    int m_rawStartIdx;      // starting point of a region in raw file, inclusive
    int m_rawEndIdx;        // ending point of a region in raw file, inclusive      

    UINT m_hashValue;       // hash value of this funciton.    

private:
    int incFeatureCount(LPCTSTR featureStr);

    CCSAssemblyFunction* m_pFcn;            // function of this region
    CCSMapStringToInt m_featureCounts;      // keep track of the count of each feature. 
    CCSFeatureVector m_vector;              // region-specific feature vector
    CCSBoolArray m_binaryVector;            // binary vector
};

typedef CTypedPtrArray<CPtrArray, CCSRegion*> CCSRegionArray;
class CCSRegions : public CCSRegionArray
{
public:
    CCSRegions() {};
    virtual ~CCSRegions() {};
    int findRegion(const CCSRegion* pRegion) const; // return -1 if not found.
    bool addRegion(CCSRegion* pRegion);
    bool addIfNew(CCSRegion* pRegion);
    bool addIfNewWithSort(CCSRegion* pNewRegion);
    void cleanup();
};

typedef CMap<UINT, UINT, CCSRegions*, CCSRegions*&> CCSMapUINTToRegions;
class CCSRegionsMap : public CCSMapUINTToRegions
{
public:
    CCSRegionsMap();
    virtual ~CCSRegionsMap();
   	bool addRegion(UINT key, CCSRegion* pRegion);
    bool removeSingletons();
    void cleanup();
};

typedef CTypedPtrArray<CPtrArray, CCSRegionsMap*> CCSRegionsMapArray;
class CCSRegionsMaps : public CCSRegionsMapArray
{
public:
    CCSRegionsMaps();
    virtual ~CCSRegionsMaps();
    bool addRegionMap(CCSRegionsMap* pNewRegionMap);
    void cleanup();
};