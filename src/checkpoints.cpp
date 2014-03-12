// Copyright (c) 2009-2012 The Bitcoin developers
// Copyright (c) 2011-2012 Litecoin Developers
// Copyright (c) 2013 digitalcoin Developers

// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include <boost/assign/list_of.hpp> // for 'map_list_of()'
#include <boost/foreach.hpp>

#include "checkpoints.h"

#include "main.h"
#include "uint256.h"

namespace Checkpoints
{
    typedef std::map<int, uint256> MapCheckpoints;

    //
    // What makes a good checkpoint block?
    // + Is surrounded by blocks with reasonable timestamps
    //   (no blocks before with a timestamp after, none after with
    //    timestamp before)
    // + Contains no strange transactions
    //
    static MapCheckpoints mapCheckpoints =
            boost::assign::map_list_of
            ( 0, uint256("0x5e039e1ca1dbf128973bf6cff98169e40a1b194c3b91463ab74956f413b2f9c8"))
			( 1, uint256("0x45b2559dbe5e5772498e4170f3f1561448179fa90dd349e60e891766878dea2e"))
			( 20, uint256("0x59436aad777d285d52a3fb61b4176c7ca30a1254b7fc1480b2c7320913953fe3"))
			( 3500, uint256("0x6e92c6cf634c39149d07f022cf13e87b91713d1e7a5d9abc2b5f3646a4027838"))
			( 22222, uint256("0x7a58919a24c189f8c286413381e6ed7224c90a4181a7f7cd098825cc75ddec27"))
			( 480000, uint256("0xa11759fa9ed9c11769dc7ec3c279f523c886ea0ca0b9e1d1a49441c32b701f0d"))
			( 600308, uint256("0x0cd7f68e0e79a4595abb871fb71fd2db803b34b15da182d23c1568f56611af91"))			
            ;


    bool CheckBlock(int nHeight, const uint256& hash)
    {
        if (fTestNet) return true; // Testnet has no checkpoints

        MapCheckpoints::const_iterator i = mapCheckpoints.find(nHeight);
        if (i == mapCheckpoints.end()) return true;
        return hash == i->second;
    }

    int GetTotalBlocksEstimate()
    {
        if (fTestNet) return 0;
        return mapCheckpoints.rbegin()->first;
    }

    CBlockIndex* GetLastCheckpoint(const std::map<uint256, CBlockIndex*>& mapBlockIndex)
    {
        if (fTestNet) return NULL;

        BOOST_REVERSE_FOREACH(const MapCheckpoints::value_type& i, mapCheckpoints)
        {
            const uint256& hash = i.second;
            std::map<uint256, CBlockIndex*>::const_iterator t = mapBlockIndex.find(hash);
            if (t != mapBlockIndex.end())
                return t->second;
        }
        return NULL;
    }
}
