DigitalCoin v3 Ubuntu Build
=====================================

sudo apt-get install build-essential libtool autotools-dev autoconf pkg-config libssl-dev git

sudo apt-get install libboost-all-dev

sudo apt-get install libdb4.8-dev

git clone git://github.com/DGCFoundation/digitalcoin.git

cd digitalcoin

./autogen.sh

./configure

make

cd src

sudo cp digitalcoind /usr/bin


Digitalcoin integration/staging tree
=====================================

http://digitalcoin.co

Copyright (c) 2009-2015 Bitcoin Developers
Copyright (c) 2011-2014 Litecoin Developers 
Copyright (c) 2013-2015 Digitalcoin Developers

What is DigitalCoin?
--------------------
DigitalCoin is a decentralized peer-to-peer cryptographic currency that is not controlled by any central authority. DigitalCoin is designed for transaction speed, security, stability, and ease of use. This regard for stability is inherent in the design of the economy and in the spirit of the community.

- 40 second block targets
- subsidy halves every ~3 years
- 48,166,000 total coins
- 5 coins per block
- 3 algorithms
- 1 block to retarget difficulty

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

Development process
-------------------

Developers work in their own trees, then submit pull requests when they think
their feature or bug fix is ready.

If it is a simple/trivial/non-controversial change, then one of the DigitalCoin
development team members simply pulls it.

If it is a *more complicated or potentially controversial* change, then the patch
submitter will be asked to start a discussion (if they haven't already) on the
[mailing list](http://sourceforge.net/mailarchive/forum.php?forum_name=bitcoin-development).

The patch will be accepted if there is broad consensus that it is a good thing.
Developers should expect to rework and resubmit patches if the code doesn't
match the project's coding conventions (see [doc/coding.md](doc/coding.md)) or are
controversial.

The `master` branch is regularly built and tested, but is not guaranteed to be
completely stable. [Tags](https://github.com/digitalcoin/digitalcoin/tags) are created
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

Every pull request is built for both Windows and Linux on a dedicated server,
and unit and sanity tests are automatically run. The binaries produced may be
used for manual QA testing — a link to them will appear in a comment on the
pull request posted by [BitcoinPullTester](https://github.com/BitcoinPullTester). See https://github.com/TheBlueMatt/test-scripts
for the build/test scripts.

### Manual Quality Assurance (QA) Testing

Large changes should have a test plan, and should be tested by somebody other
than the developer who wrote the code.
See https://github.com/bitcoin/QA/ for how to create a test plan.

Translations
------------

Changes to translations as well as new translations can be submitted to
[Bitcoin Core's Transifex page](https://www.transifex.com/projects/p/bitcoin/).

Periodically the translations are pulled from Transifex and merged into the git repository. See the
[translation process](doc/translation_process.md) for details on how this works.

**Important**: We do not accept translation changes as github pull request because the next
pull from Transifex would automatically overwrite them again.

Development tips and tricks
---------------------------

**compiling for debugging**

Run configure with the --enable-debug option, then make. Or run configure with
CXXFLAGS="-g -ggdb -O0" or whatever debug flags you need.

**debug.log**

If the code is behaving strangely, take a look in the debug.log file in the data directory;
error and debugging message are written there.

The -debug=... command-line option controls debugging; running with just -debug will turn
on all categories (and give you a very large debug.log file).

The Qt code routes qDebug() output to debug.log under category "qt": run with -debug=qt
to see it.

**testnet and regtest modes**

Run with the -testnet option to run with "play bitcoins" on the test network, if you
are testing multi-machine code that needs to operate across the internet.

If you are testing something that can run on one machine, run with the -regtest option.
In regression test mode blocks can be created on-demand; see qa/rpc-tests/ for tests
that run in -regest mode.

**DEBUG_LOCKORDER**

Bitcoin Core is a multithreaded application, and deadlocks or other multithreading bugs
can be very difficult to track down. Compiling with -DDEBUG_LOCKORDER (configure
CXXFLAGS="-DDEBUG_LOCKORDER -g") inserts run-time checks to keep track of what locks
are held, and adds warning to the debug.log file if inconsistencies are detected.
