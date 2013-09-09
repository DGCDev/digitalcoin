digitalcoin 0.1 BETA

Copyright (c) 2009-2012 Bitcoin Developers
Copyright (c) 2011-2012 Litecoin Developers
Copyright (c) 2011-2012 Digitalcoin Developers
Distributed under the MIT/X11 software license, see the accompanying
file COPYING or http://www.opensource.org/licenses/mit-license.php.
This product includes software developed by the OpenSSL Project for use in
the OpenSSL Toolkit (http://www.openssl.org/).  This product includes
cryptographic software written by Eric Young (eay@cryptsoft.com).


Intro
-----
What It Is:
    A digital currency
    A means to store value
    A fast method to send and receive money
    Transparent and open source
    Fair

Transaction Specifications:
-Irreversible value transfer
-Transactions are sent instantly and are confirmed in minutes
-Double spending prevented by the blockchain
-Miners process transactions and create new currency through mining
-Transactions can be received at any time

Technical Specifications:
    Algorithm: Scrypt
    Block Time: 20 seconds  - Enough time to minimize orphans but still provide almost instant transactions.
    Difficulty: Starts at 0.00024414 – Retargets every 6 hours – It will take 6-8 difficulty adjustments (or ~13000 blocks) to reach desired difficulty of 1. It retargets quickly to adjust to the environment, so stability should not be much of an issue. Miners should also find the system adjusts fast enough to be up to date, but not so fast as to make the environment unpredictable and unstable.
    Reward: 20 coins per block, halved every 3 years – Consistent rewards encourage miners to support the network long term. Loyalty is rewarded. This also reduces volatility by setting the foundation on a stable path. Due to the fair launch policy, block rewards will grow in value until they reach the 20 coins per block reward. More details below.
    Maximum coins: 200,000,000

bitcointalk thread: https://bitcointalk.org/index.php?topic=209508.0


Setup
-----
Unpack the files into a directory and run digitalcoin-qt.exe.

If you have Microsoft Security Essentials, you need to add digitalcoin-qt.exe to its
"Excluded processes" list.  Microsoft Security Essentials->Settings tab,
select Excluded processes, press Add, select digitalcoin-qt.exe, OK, Save changes.

The software automatically finds other nodes to connect to.  You can
enable Universal Plug and Play using a menu entry or set your firewall
to forward port 9333 (TCP) to your computer so you can receive
incoming connections.  digitalcoin works without incoming connections,
but allowing incoming connections helps the digitalcoin network thrive.

See the bitcoin wiki at:
  https://en.bitcoin.it/wiki/Main_Page
for more help and information.
