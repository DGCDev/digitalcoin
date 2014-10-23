=======
DigitalCoin v3 Ubuntu Build
=====================================

sudo apt-get install build-essential libtool autotools-dev autoconf pkg-config libssl-dev git

sudo apt-get install libboost-all-dev

sudo apt-get install libdb4.8-dev

git clone git://github.com/DGCFoundation/DGCV3.0.git

cd DGCV3.0

./autogen.sh

./configure

make

cd src

sudo cp digitalcoind /usr/bin


=======
Digitalcoin integration/staging tree
=====================================

http://digitalcoin.co

Copyright (c) 2009-2014 Bitcoin Developers
Copyright (c) 2011-2014 Litecoin Developers 
Copyright (c) 2011-2014 Digitalcoin Developers

What is DigitalCoin?
--------------------
DigitalCoin is a decentralized peer-to-peer cryptographic currency that is not controlled by any central authority. DigitalCoin is designed for transaction speed, security, stability, and ease of use. This regard for stability is inherent in the design of the economy and in the spirit of the community.

- 40 second block targets
- subsidy halves every ~3 years
- ~145 million total coins
- 5 coins per block
- 3 algorithms
- 108 blocks to retarget difficulty

For more information, see www.digitalcoin.co

Links
------------------------
Website: http://www.digitalcoin.co

Forums: http://www.digitalcoin.co/forums/

BitcoinTalk: https://bitcointalk.org/index.php?topic=209508.0

Facebook: https://www.facebook.com/DigitalCoinDGC

Twitter: https://twitter.com/DigitalcoinDGC

Reddit: http://www.reddit.com/r/digitalcoin/

IRC Channel: http://webchat.freenode.net/?channels=#digitalcoin

License
-------

DigitalCoin is released under the terms of the MIT license. See COPYING for more information or see http://opensource.org/licenses/MIT.



Development process
-------------------

Developers work in their own trees, then submit pull requests when they think
their feature or bug fix is ready.

If it is a simple/trivial/non-controversial change, then one of the DigitalCoin
development team members simply pulls it.

The patch will be accepted if there is broad consensus that it is a good thing.
Developers should expect to rework and resubmit patches if the code doesn't
match the project's coding conventions (see [doc/coding.md](doc/coding.md)) or are
controversial.

The `master` branch is regularly built and tested, but is not guaranteed to be
completely stable. [Tags](https://github.com/bitcoin/bitcoin/tags) are created
regularly to indicate new official, stable release versions of DigitalCoin.

Testing
-------

Testing and code review is the bottleneck for development; we get more pull
requests than we can review and test. Please be patient and help out, and
remember this is a security-critical project where any mistake might cost people
lots of money.

### Automated Testing

Developers are strongly encouraged to write unit tests for new code, and to
submit new unit tests for old code. Unit tests can be compiled and run (assuming they weren't disabled in configure) with: `make check`

### Proof Of Developer
POD-CryptoAsian
