// Copyright (c) 2010 Satoshi Nakamoto
// Copyright (c) 2009-2014 The Bitcoin developers
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or http://www.opensource.org/licenses/mit-license.php.

#include "chainparams.h"

#include "assert.h"
#include "random.h"
#include "util.h"

#include <boost/assign/list_of.hpp>

using namespace std;
using namespace boost::assign;

struct SeedSpec6 {
    uint8_t addr[16];
    uint16_t port;
};

#include "chainparamsseeds.h"

//
// Main network
//

// Convert the pnSeeds6 array into usable address objects.
static void convertSeed6(std::vector<CAddress> &vSeedsOut, const SeedSpec6 *data, unsigned int count)
{
	// It'll only connect to one or two seed nodes because once it connects,
    // it'll get a pile of addresses with newer timestamps.
    // Seed nodes are given a random 'last seen time' of between one and two
    // weeks ago.
    const int64_t nOneWeek = 7*24*60*60;
    for (unsigned int i = 0; i < count; i++)
    {
        struct in6_addr ip;
        memcpy(&ip, data[i].addr, sizeof(ip));
        CAddress addr(CService(ip, data[i].port));
        addr.nTime = GetTime() - GetRand(nOneWeek) - nOneWeek;
        vSeedsOut.push_back(addr);
    }
}

class CMainParams : public CChainParams {
public:
    CMainParams() {
		networkID = CBaseChainParams::MAIN;
		strNetworkID = "main";
		// The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        pchMessageStart[0] = 0xfb;
        pchMessageStart[1] = 0xc0;
        pchMessageStart[2] = 0xb6;
        pchMessageStart[3] = 0xdb;
        vAlertPubKey = ParseHex("04016c44069c3152982413d3ba3bf262a3a4d3ddad859ba78e0d744f5c67c2205d2aa2122e6c62b6310dad2d1e2f7e39028455ff1dbb26511c60fc96c8b4560c43");
        nDefaultPort = 7999;        
        bnProofOfWorkLimit[ALGO_SHA256D] = ~uint256(0) >> 20;
        bnProofOfWorkLimit[ALGO_SCRYPT]  = ~uint256(0) >> 20;
        bnProofOfWorkLimit[ALGO_X11] = ~uint256(0) >> 20;
		nMinerThreads = 0;
		nTargetTimespan = 108 * 40; // digitalcoin: 108 blocks (72 mins) [OLD WAS 6*60*3*20]
		nTargetSpacing = 1 * 40; // digitalcoin: 40 seconds		
		
	// Build the genesis block. Note that the output of the genesis coinbase cannot
        // be spent as it did not originally exist in the database.
        const char* pszTimestamp = "Digitalcoin, A Currency for a Digital Age";
        CMutableTransaction txNew;
        txNew.vin.resize(1);
        txNew.vout.resize(1);
        txNew.vin[0].scriptSig = CScript() << 486604799 << CScriptNum(4) << vector<unsigned char>((const unsigned char*)pszTimestamp, (const unsigned char*)pszTimestamp + strlen(pszTimestamp));
        txNew.vout[0].nValue = 50 * COIN;
        txNew.vout[0].scriptPubKey = CScript() << ParseHex("04a5814813115273a109cff99907ba4a05d951873dae7acb6c973d0c9e7c88911a3dbc9aa600deac241b91707e7b4ffb30ad91c8e56e695a1ddf318592988afe0a") << OP_CHECKSIG;

        genesis.vtx.push_back(txNew);
        genesis.hashPrevBlock = 0;
        genesis.hashMerkleRoot = genesis.BuildMerkleTree();
        genesis.nVersion = 1;
        genesis.nTime    = 1367867384;
        genesis.nBits    = 0x1e0ffff0;
        genesis.nNonce   = 672176;

        hashGenesisBlock = genesis.GetHash();

        assert(hashGenesisBlock == uint256("0x5e039e1ca1dbf128973bf6cff98169e40a1b194c3b91463ab74956f413b2f9c8"));
        assert(genesis.hashMerkleRoot == uint256("0xecb2c595fff9f2364152c32027737007c5a4c60ec960cf93754b0211bc2a1501"));

        vSeeds.push_back(CDNSSeedData("digitalcoin.co seed #1", "digitalcoin.co"));        
		vSeeds.push_back(CDNSSeedData("digitalcoin.co seed #2", "game.digitalcoin.co"));
		vSeeds.push_back(CDNSSeedData("digitalcoin.co seed #3", "dev.digitalcoin.co"));
		vSeeds.push_back(CDNSSeedData("digitalcoin.co seed #3", "wiki.digitalcoin.co"));
		vSeeds.push_back(CDNSSeedData("ahmed seed #1", "178.62.28.81"));
        //vSeeds.push_back(CDNSSeedData("digihash.co seed #3", "seed3.digihash.co"));
        //vSeeds.push_back(CDNSSeedData("love2hash.com seed #4", "seed4.love2hash.com"));
        //vSeeds.push_back(CDNSSeedData("digiexplorer.info seed #5", "seed5.digiexplorer.info"));

        base58Prefixes[PUBKEY_ADDRESS] = list_of(30);
        base58Prefixes[SCRIPT_ADDRESS] = list_of(5);
        base58Prefixes[SECRET_KEY] =     list_of(0x9e);
        base58Prefixes[EXT_PUBLIC_KEY] = list_of(0x04)(0x88)(0xB2)(0x1E);
        base58Prefixes[EXT_SECRET_KEY] = list_of(0x04)(0x88)(0xAD)(0xE4);

        convertSeed6(vFixedSeeds, pnSeed6_main, ARRAYLEN(pnSeed6_main));
    
		fRequireRPCPassword = true;
		fMiningRequiresPeers = true;
		fDefaultCheckMemPool = false;
		fAllowMinDifficultyBlocks = false;
		fRequireStandard = true;		
		fMineBlocksOnDemand = false;
    }
};
static CMainParams mainParams;

//
// Testnet (v3)
//
class CTestNetParams : public CMainParams {
public:
    CTestNetParams() {
		networkID = CBaseChainParams::TESTNET;
		strNetworkID = "test";
        // The message start string is designed to be unlikely to occur in normal data.
        // The characters are rarely used upper ASCII, not valid as UTF-8, and produce
        // a large 4-byte int at any alignment.
        nDefaultPort = 12025;        
		nMinerThreads = 0;
		nTargetTimespan = 108 * 40; // digitalcoin: 108 blocks (72 mins) [OLD WAS 6*60*3*20]
		nTargetSpacing = 1 * 40; // digitalcoin: 40 seconds		

		vFixedSeeds.clear();
        vSeeds.clear();
		
		convertSeed6(vFixedSeeds, pnSeed6_test, ARRAYLEN(pnSeed6_test));
		
		fRequireRPCPassword = true;
		fMiningRequiresPeers = true;
		fDefaultCheckMemPool = false;
		fAllowMinDifficultyBlocks = true;
		fRequireStandard = false;		
		fMineBlocksOnDemand = false;
    }
};
static CTestNetParams testNetParams;

//
// Regression test
//
class CRegTestParams : public CTestNetParams {
public:
    CRegTestParams() {
		networkID = CBaseChainParams::REGTEST;
		strNetworkID = "regtest";
        pchMessageStart[0] = 0xfa;
        pchMessageStart[1] = 0xb3;
        pchMessageStart[2] = 0xb2;
        pchMessageStart[3] = 0xdb;
        //nSubsidyHalvingInterval = 150;
       // bnProofOfWorkLimit = CBigNum();
		nMinerThreads = 1;
        genesis.nTime = 1392796564;
        genesis.nBits = 0x207fffff;
        genesis.nNonce = 961533;
        hashGenesisBlock = genesis.GetHash();
        nDefaultPort = 18444;        
		nTargetTimespan = 108 * 40; // digitalcoin: 108 blocks (72 mins) [OLD WAS 6*60*3*20]
		nTargetSpacing = 1 * 40; // digitalcoin: 40 seconds		
        //assert(hashGenesisBlock == uint256("0x0f9188f13cb7b2c71f2a335e3a4fc328bf5beb436012afca590b1a11466e2206"));

        vSeeds.clear();  // Regtest mode doesn't have any DNS seeds.
    

		fRequireRPCPassword = false;
		fMiningRequiresPeers = false;
		fDefaultCheckMemPool = true;
		fAllowMinDifficultyBlocks = true;
		fRequireStandard = false;		
		fMineBlocksOnDemand = true;
	}
};
static CRegTestParams regTestParams;

static CChainParams *pCurrentParams = 0;

const CChainParams &Params() {
	assert(pCurrentParams);
    return *pCurrentParams;
}

void SelectParams(CBaseChainParams::Network network) {
	SelectBaseParams(network);
    switch (network) {
        case CBaseChainParams::MAIN:
            pCurrentParams = &mainParams;
            break;
        case CBaseChainParams::TESTNET:
            pCurrentParams = &testNetParams;
            break;
        case CBaseChainParams::REGTEST:
            pCurrentParams = &regTestParams;
            break;
        default:
            assert(false && "Unimplemented network");
            return;
    }
}

bool SelectParamsFromCommandLine() {
    if (!SelectBaseParamsFromCommandLine())
        return false;
    

	SelectParams(BaseParams().NetworkID());
	
    return true;
}
