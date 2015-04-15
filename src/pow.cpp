// Copyright (c) 2009-2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Copyright (c) 2013-2015 The Digitalcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "pow.h"

#include "chainparams.h"
#include "core.h"
#include "main.h"
#include "uint256.h"


static const int64_t nTargetTimespan = 108 * 40; // digitalcoin: 108 blocks (72 mins) [OLD WAS 6*60*3*20]
static const int64_t nTargetSpacing = 1 * 40; // digitalcoin: 40 seconds
static const int64_t nInterval = nTargetTimespan / nTargetSpacing;

//MultiAlgo Target updates
static const int64_t multiAlgoTargetTimespan = 120; // 2 minutes (NUM_ALGOS(3) * 40 seconds)
static const int64_t multiAlgoTargetSpacing = 120; // 2 minutes (NUM_ALGOS * 30 seconds)
static const int64_t multiAlgoInterval = 1; // retargets every blocks

static const int64_t nAveragingInterval = 10; // 10 blocks
static const int64_t nAveragingTargetTimespan = nAveragingInterval * multiAlgoTargetSpacing; // 20 minutes

static const int64_t nMaxAdjustDown = 40; // 40% adjustment down
static const int64_t nMaxAdjustUp = 20; // 20% adjustment up

static const int64_t nTargetTimespanAdjDown = multiAlgoTargetTimespan * (100 + nMaxAdjustDown) / 100;
static const int64_t nLocalDifficultyAdjustment = 40; // 40% down, 20% up

static const int64_t nMinActualTimespan = nAveragingTargetTimespan * (100 - nMaxAdjustUp) / 100;
static const int64_t nMaxActualTimespan = nAveragingTargetTimespan * (100 + nMaxAdjustDown) / 100;

//
// minimum amount of work that could possibly be required nTime after
// minimum work required was nBase
//
unsigned int ComputeMinWork(unsigned int nBase, int64_t nTime)
{
    uint256 bnProofOfWorkLimit = Params().ProofOfWorkLimit(ALGO_SCRYPT);
    // Testnet has min-difficulty blocks
    // after nTargetSpacing*2 time between blocks:
    if (Params().NetworkID() == CChainParams::TESTNET && nTime > nTargetSpacing*2)
        return bnProofOfWorkLimit.GetCompact();

    uint256 bnResult;
    bnResult.SetCompact(nBase);
    while (nTime > 0 && bnResult < bnProofOfWorkLimit)
    {
        // Maximum 400% adjustment...
        bnResult *= 4;
        // ... in best-case exactly 4-times-normal target time
        nTime -= nTargetTimespan*4;
    }
    if (bnResult > bnProofOfWorkLimit)
        bnResult = bnProofOfWorkLimit;
    return bnResult.GetCompact();
}



unsigned int GetNextWorkRequired(const CBlockIndex* pindexLast, const CBlockHeader *pblock, int algo)
{
   int nHeight = pindexLast->nHeight;
   if (Params().NetworkID() == CChainParams::TESTNET)
   {
	return 0x1d13ffec;
   }
   else if (!Params().NetworkID() == CChainParams::TESTNET && nHeight < V3_FORK)
   {
	return GetNextWorkRequiredV1(pindexLast, pblock, algo);
   }
   else if (!Params().NetworkID() == CChainParams::TESTNET && nHeight >= V3_FORK)
   {
        LogPrintf("Switch to DigiShield");
        return GetNextWorkRequiredV2(pindexLast, pblock, algo);
   }

}

unsigned int GetNextWorkRequiredV1(const CBlockIndex* pindexLast, const CBlockHeader *pblock, int algo)
{
   unsigned int nProofOfWorkLimit = Params().ProofOfWorkLimit(algo).GetCompact();
   int nHeight = pindexLast->nHeight + 1;

   bool fNewDifficultyProtocol = (nHeight >= DIFF_SWITCH_HEIGHT);
   bool fInflationFixProtocol = (nHeight >= INFLATION_FIX_HEIGHT);
   bool fDifficultySwitchHeightTwo = (nHeight >= DIFF2_SWITCH_HEIGHT);

   int64_t nTargetTimespanCurrent = fInflationFixProtocol? nTargetTimespan : (nTargetTimespan*5);
   int64_t nInterval = fInflationFixProtocol? (nTargetTimespanCurrent / nTargetSpacing) : (nTargetTimespanCurrent / (nTargetSpacing / 2));

    // Testnet Fixed Diff
    if (Params().NetworkID() == CChainParams::TESTNET)
    {
	return nProofOfWorkLimit;
    }
    // Genesis block
    if (pindexLast == NULL)
        return nProofOfWorkLimit;

    // Only change once per interval
    if ((pindexLast->nHeight+1) % nInterval != 0)
    {
        return pindexLast->nBits;
    }

    // digitalcoin: This fixes an issue where a 51% attack can change difficulty at will.
    // Go back the full period unless it's the first retarget after genesis. Code courtesy of Art Forz
    int blockstogoback = nInterval-1;
    if ((pindexLast->nHeight+1) != nInterval)
        blockstogoback = nInterval;

    // Go back by what we want to be the last intervals worth of blocks
    const CBlockIndex* pindexFirst = pindexLast;
    for (int i = 0; pindexFirst && i < blockstogoback; i++)
        pindexFirst = pindexFirst->pprev;
    assert(pindexFirst);

    // Limit adjustment step
    int64_t nActualTimespan = pindexLast->GetBlockTime() - pindexFirst->GetBlockTime();
    int64_t nActualTimespanMax = fNewDifficultyProtocol? (nTargetTimespanCurrent*2) : (nTargetTimespanCurrent*4);
    int64_t nActualTimespanMin = fNewDifficultyProtocol? (nTargetTimespanCurrent/2) : (nTargetTimespanCurrent/4);

    //new for v1.0.1
    if (fDifficultySwitchHeightTwo){
	nActualTimespanMax = ((nTargetTimespanCurrent*75)/60);
	nActualTimespanMin = ((nTargetTimespanCurrent*55)/73); }

    if (nActualTimespan < nActualTimespanMin)
        nActualTimespan = nActualTimespanMin;
    if (nActualTimespan > nActualTimespanMax)
        nActualTimespan = nActualTimespanMax;

    // Retarget
    uint256 bnNew;
	uint256 bnOld;
    bnNew.SetCompact(pindexLast->nBits);
	bnOld = bnNew;
    bnNew *= nActualTimespan;
    bnNew /= nTargetTimespanCurrent;

    if (bnNew > Params().ProofOfWorkLimit(algo))
        bnNew = Params().ProofOfWorkLimit(algo);

    /// debug print
    LogPrintf("GetNextWorkRequired V1 RETARGET\n");
    LogPrintf("Before: %08x  %s\n", pindexLast->nBits, bnOld.ToString());
    LogPrintf("After:  %08x  %s\n", bnNew.GetCompact(), bnNew.ToString());
    return bnNew.GetCompact();
}

unsigned int GetNextWorkRequiredV2(const CBlockIndex* pindexLast, const CBlockHeader *pblock, int algo)
{
    unsigned int nProofOfWorkLimit = Params().ProofOfWorkLimit(algo).GetCompact();
    LogPrintf("Proof Of Work Limit For Algo %i, is % i", algo, nProofOfWorkLimit);

    // Genesis block
    if (pindexLast == NULL)
        return nProofOfWorkLimit;

    // find first block in averaging interval
    // Go back by what we want to be nAveragingInterval blocks per algo
    const CBlockIndex* pindexFirst = pindexLast;
    for (int i = 0; pindexFirst && i < NUM_ALGOS * nAveragingInterval; i++)
    {
        pindexFirst = pindexFirst->pprev;
    }
    const CBlockIndex* pindexPrevAlgo = GetLastBlockIndexForAlgo(pindexLast, algo);
    if (pindexPrevAlgo == NULL || pindexFirst == NULL)
        return nProofOfWorkLimit; // not enough blocks available

    // Limit adjustment step
    // Use medians to prevent time-warp attacks
    int64_t nActualTimespan = pindexLast->GetMedianTimePast() - pindexFirst->GetMedianTimePast();
    nActualTimespan = nAveragingTargetTimespan + (nActualTimespan - nAveragingTargetTimespan)/6;
    LogPrintf("  nActualTimespan = %d before bounds\n", nActualTimespan);
    if (nActualTimespan < nMinActualTimespan)
        nActualTimespan = nMinActualTimespan;
    if (nActualTimespan > nMaxActualTimespan)
        nActualTimespan = nMaxActualTimespan;

    // Global retarget
    uint256 bnNew;
	uint256 bnOld;
    bnNew.SetCompact(pindexPrevAlgo->nBits);
	bnOld = bnNew;   
    bnNew *= nActualTimespan;
    bnNew /= nAveragingTargetTimespan;

    // Per-algo retarget
    int nAdjustments = pindexPrevAlgo->nHeight - pindexLast->nHeight + NUM_ALGOS - 1;
    if (nAdjustments > 0)
    {
        for (int i = 0; i < nAdjustments; i++)
        {
            bnNew /= 100 + nLocalDifficultyAdjustment;
            bnNew *= 100;
        }
    }
    if (nAdjustments < 0)
    {
        for (int i = 0; i < -nAdjustments; i++)
        {
            bnNew *= 100 + nLocalDifficultyAdjustment;
            bnNew /= 100;
        }
    }

    if (bnNew > Params().ProofOfWorkLimit(algo))
        bnNew = Params().ProofOfWorkLimit(algo);

    /// debug print
    LogPrintf("GetNextWorkRequired RETARGET\n");
    LogPrintf("nTargetTimespan = %d    nActualTimespan = %d\n", nTargetTimespan, nActualTimespan);
    LogPrintf("Before: %08x  %s\n", pindexLast->nBits, bnOld.ToString());
    LogPrintf("After:  %08x  %s\n", bnNew.GetCompact(), bnNew.ToString());

    return bnNew.GetCompact();
}

bool CheckProofOfWork(uint256 hash, unsigned int nBits, int algo)
{
	bool fNegative;
	bool fOverflow;
    uint256 bnTarget;
	bnTarget.SetCompact(nBits, &fNegative, &fOverflow);
	
    // Check range
    if (fNegative || bnTarget == 0 || fOverflow || bnTarget > Params().ProofOfWorkLimit(algo))
        return error("CheckProofOfWork(algo=%d) : nBits below minimum work", algo);

    // Check proof of work matches claimed amount
    if (hash > bnTarget)
        return error("CheckProofOfWork(algo=%d) : hash doesn't match nBits", algo);

    return true;
}