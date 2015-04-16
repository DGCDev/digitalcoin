// Copyright (c) 2009-2014 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "checkpoints.h"

#include "main.h"
#include "uint256.h"

#include <stdint.h>

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

    // How many times we expect transactions after the last checkpoint to
    // be slower. This number is a compromise, as it can't be accurate for
    // every system. When reindexing from a fast disk with a slow CPU, it
    // can be up to 20, while when downloading from a slow network with a
    // fast multicore CPU, it won't be much higher than 1.
    static const double SIGCHECK_VERIFICATION_FACTOR = 5.0;

    struct CCheckpointData {
        const MapCheckpoints *mapCheckpoints;
        int64_t nTimeLastCheckpoint;
        int64_t nTransactionsLastCheckpoint;
        double fTransactionsPerDay;
    };

    bool fEnabled = true;

    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    static MapCheckpoints mapCheckpoints =
        boost::assign::map_list_of
 		( 0,       uint256("0x5e039e1ca1dbf128973bf6cff98169e40a1b194c3b91463ab74956f413b2f9c8"))
		( 1,       uint256("0x45b2559dbe5e5772498e4170f3f1561448179fa90dd349e60e891766878dea2e"))
		( 20,      uint256("0x59436aad777d285d52a3fb61b4176c7ca30a1254b7fc1480b2c7320913953fe3"))
		( 500,     uint256("0xad48f7f697107bc2235779cd3efb56d522ac557c5f7824b81cba610ed2bb90d4"))
		( 1000,    uint256("0x6970173f5fe0686a168e67e37d858c2fb090e67dfd26d9bf44c356aa2cda100c"))
		( 3500,    uint256("0x6e92c6cf634c39149d07f022cf13e87b91713d1e7a5d9abc2b5f3646a4027838"))
		( 5000,    uint256("0x45c1a2e86b6fbeb0e0ee3d4662e62cba0677571b8956c1846b19ee7f10648028"))
		( 10000,   uint256("0x73e21da5458d9e3b321228f1b319e217fd4c82da629bc293981a79077e28b869"))
		( 15000,   uint256("0x5c456fa7873afec904d965194d27fc7127295d08a6910d9ac255bacfc19ab013"))
		( 22222,   uint256("0x7a58919a24c189f8c286413381e6ed7224c90a4181a7f7cd098825cc75ddec27"))
		( 30000,   uint256("0xcf2f900a91d869974e309451c28ae368690f42b773618f6500bac04588f3bb44"))
		( 50000,   uint256("0xa691f38abcea1434b10dd64714b12d9654aae718d2a5d6d8b45720108edc3d5d"))
		( 75000,   uint256("0x461190bfda4470466ad456291de9094664ef56f85a73b2f3811d06eed444bbd8"))
		( 100000,  uint256("0x06b2abf25bd4d7d1d9866e166e90c389480c58aaacf6545d2277c14c8cbb94d3"))
		( 200000,  uint256("0x79cb7dba6456048b9af2ffaae309f9c4d80b436ad92e01e8ae11ec1127ee5b18"))
		( 300000,  uint256("0x6b77bcab02c162b9a7a4424db80bd4241564d2c9d783e3c9f0e02cd0f25b0025"))
		( 480000,  uint256("0xa11759fa9ed9c11769dc7ec3c279f523c886ea0ca0b9e1d1a49441c32b701f0d"))
		( 600308,  uint256("0x0cd7f68e0e79a4595abb871fb71fd2db803b34b15da182d23c1568f56611af91"))
		( 778389,  uint256("0x5d1c20eda68cf4221885f2e9e0ad47817cca44da24f770f1334ca8cc2b07eb49"))
		( 800000,  uint256("0x609f6259e199a0a60705bf4c02d6aee84dc38d7d741f8044b1eec0c636567ebb"))
		( 850000,  uint256("0xa52a30db508a3bc3ab71f9bd83573e4ab4289ed8e5b66dc92f862baf6eb80eba"))
		( 900000,  uint256("0x254c90e2a47d0f28292bc7d6f5cd8f856eb782d8692a1d923abd9c43749935bc"))
		( 950000,  uint256("0x706c8ce10f2c9ffbf90fc10fe683ccabeeafa4e6cf5c8ec320f50319c9ba40a9"))
		( 1000000, uint256("0x564e27dc7bf17488c4bf66ba0955cbf075c975f8386ede157be9578da0476356"))
		( 1010000, uint256("0xbbe9adc561be01cb37acd71abf42a17e566d05b9cdfed95793db931540805bcf"))
		( 1020000, uint256("0xdc51bcc193a2e84bcbdd0448e6e0f5396b4e57c2f43e239d110d1145b147d4c9"))
		( 1023000, uint256("0xeaae71b7dae28ab3abfcfa959ae3db50eb4ed93204e36731a54063a4ea8e7218"))
		( 1023013, uint256("0xc328d2a8f8b976769a6b0488cbf6dc641902b6eb7db0995befd58e69679af4f8"))
		;
    static const CCheckpointData data = {
        &mapCheckpoints,
        1417684495, // * UNIX timestamp of last checkpoint block
        1755145,   // * total number of transactions between genesis and last checkpoint
                    //   (the tx=... number in the SetBestChain debug.log lines)
        3840.0     // * estimated number of transactions per day after checkpoint
    };

    static MapCheckpoints mapCheckpointsTestnet =
        boost::assign::map_list_of
	(     0, uint256("0x7497ea1b465eb39f1c8f507bc877078fe016d6fcb6dfad3a64c98dcc6e1e8496"))
        ;
    static const CCheckpointData dataTestnet = {
        &mapCheckpointsTestnet,
	1393183580, // * UNIX timestamp of last checkpoint block
        30549816,   // * total number of transactions between genesis and last checkpoint
                    //   (the tx=... number in the SetBestChain debug.log lines)
        1000.0     // * estimated number of transactions per day after checkpoint

    };

    static MapCheckpoints mapCheckpointsRegtest =
        boost::assign::map_list_of
        ( 0, uint256("0f9188f13cb7b2c71f2a335e3a4fc328bf5beb436012afca590b1a11466e2206"))
        ;
    static const CCheckpointData dataRegtest = {
        &mapCheckpointsRegtest,
        0,
        0,
        0
    };

    const CCheckpointData &Checkpoints() {
        if (Params().NetworkID() == CBaseChainParams::TESTNET)
            return dataTestnet;
        else if (Params().NetworkID() == CBaseChainParams::MAIN)
            return data;
        else
            return dataRegtest;
    }

    bool CheckBlock(int nHeight, const uint256& hash)
    {
        if (!fEnabled)
            return true;

        const MapCheckpoints& checkpoints = *Checkpoints().mapCheckpoints;

        MapCheckpoints::const_iterator i = checkpoints.find(nHeight);
        if (i == checkpoints.end()) return true;
        return hash == i->second;
    }

    // Guess how far we are in the verification process at the given block index
    double GuessVerificationProgress(CBlockIndex *pindex, bool fSigchecks) {
        if (pindex==NULL)
            return 0.0;

        int64_t nNow = time(NULL);

        double fSigcheckVerificationFactor = fSigchecks ? SIGCHECK_VERIFICATION_FACTOR : 1.0;
        double fWorkBefore = 0.0; // Amount of work done before pindex
        double fWorkAfter = 0.0;  // Amount of work left after pindex (estimated)
        // Work is defined as: 1.0 per transaction before the last checkpoint, and
        // fSigcheckVerificationFactor per transaction after.

        const CCheckpointData &data = Checkpoints();

        if (pindex->nChainTx <= data.nTransactionsLastCheckpoint) {
            double nCheapBefore = pindex->nChainTx;
            double nCheapAfter = data.nTransactionsLastCheckpoint - pindex->nChainTx;
            double nExpensiveAfter = (nNow - data.nTimeLastCheckpoint)/86400.0*data.fTransactionsPerDay;
            fWorkBefore = nCheapBefore;
            fWorkAfter = nCheapAfter + nExpensiveAfter*fSigcheckVerificationFactor;
        } else {
            double nCheapBefore = data.nTransactionsLastCheckpoint;
            double nExpensiveBefore = pindex->nChainTx - data.nTransactionsLastCheckpoint;
            double nExpensiveAfter = (nNow - pindex->GetBlockTime())/86400.0*data.fTransactionsPerDay;
            fWorkBefore = nCheapBefore + nExpensiveBefore*fSigcheckVerificationFactor;
            fWorkAfter = nExpensiveAfter*fSigcheckVerificationFactor;
        }

        return fWorkBefore / (fWorkBefore + fWorkAfter);
    }

    int GetTotalBlocksEstimate()
    {
        if (!fEnabled)
            return 0;

        const MapCheckpoints& checkpoints = *Checkpoints().mapCheckpoints;

        return checkpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        if (!fEnabled)
            return NULL;

        const MapCheckpoints& checkpoints = *Checkpoints().mapCheckpoints;

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, checkpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }
}
