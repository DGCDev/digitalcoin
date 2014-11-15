#include "blockbrowser.h"
#include "ui_blockbrowser.h"
#include "main.h"
#include "wallet.h"
#include "base58.h"
#include "clientmodel.h"
#include "rpcserver.h"
#include "transactionrecord.h"

#include <sstream>
#include <string>
double getBlockHardness(int64_t height)
{
    const CBlockIndex* blockindex = getBlockIndex(height);

    int64_t nShift = (blockindex->nBits >> 24) & 0xff;

    double dDiff =
        (double)0x0000ffff / (double)(blockindex->nBits & 0x00ffffff);

    while (nShift < 29)
    {
        dDiff *= 256.0;
        nShift++;
    }
    while (nShift > 29)
    {
        dDiff /= 256.0;
        nShift--;
    }

    return dDiff;
}

const CBlockIndex* getBlockIndex(int64_t height)
{
    std::string hex = getBlockHash(height);
    uint256 hash(hex);
    return mapBlockIndex[hash];
}

std::string getBlockHash(int64_t Height)
{
	
	CBlockIndex* pindexBest = mapBlockIndex[chainActive.Tip()->GetBlockHash()];
    if(Height > pindexBest->nHeight) { return ""; }
    if(Height < 0) { return ""; }
    int64_t desiredheight;
    desiredheight = Height;
    if (desiredheight < 0 || desiredheight > pindexBest->nHeight)
        return 0;

    CBlock block;
    CBlockIndex* pblockindex =  mapBlockIndex[chainActive.Tip()->GetBlockHash()];
    while (pblockindex->nHeight > desiredheight)
        pblockindex = pblockindex->pprev;
    return  pblockindex->GetBlockHash().GetHex(); // pblockindex->phashBlock->GetHex();
}

int64_t getBlockTime(int64_t Height)
{
    std::string strHash = getBlockHash(Height);
    uint256 hash(strHash);

    if (mapBlockIndex.count(hash) == 0)
        return 0;

    CBlock block;
    CBlockIndex* pblockindex = mapBlockIndex[hash];
    return pblockindex->nTime;
}

std::string getBlockMerkle(int64_t Height)
{
    std::string strHash = getBlockHash(Height);
    uint256 hash(strHash);

    if (mapBlockIndex.count(hash) == 0)
        return 0;

    
    CBlockIndex* pblockindex = mapBlockIndex[hash];
    return pblockindex->hashMerkleRoot.ToString();//.substr(0,10).c_str();
}

int64_t getBlocknBits(int64_t Height)
{
    std::string strHash = getBlockHash(Height);
    uint256 hash(strHash);

    if (mapBlockIndex.count(hash) == 0)
        return 0;

    CBlock block;
    CBlockIndex* pblockindex = mapBlockIndex[hash];
    return pblockindex->nBits;
}

int64_t getBlockNonce(int64_t Height)
{
    std::string strHash = getBlockHash(Height);
    uint256 hash(strHash);

    if (mapBlockIndex.count(hash) == 0)
        return 0;

    CBlock block;
    CBlockIndex* pblockindex = mapBlockIndex[hash];
    return pblockindex->nNonce;
}

std::string getBlockDebug(int64_t Height)
{
    std::string strHash = getBlockHash(Height);
    uint256 hash(strHash);

    if (mapBlockIndex.count(hash) == 0)
        return 0;

    CBlock block;
    CBlockIndex* pblockindex = mapBlockIndex[hash];
    return pblockindex->ToString();
}

int64_t blocksInPastHours(int64_t hours)
{
	CBlock block;
	CBlockIndex* pindexBest = mapBlockIndex[chainActive.Tip()->GetBlockHash()];
    int64_t wayback = hours * 3600;
    bool check = true;
    int64_t height = pindexBest->nHeight;
    int64_t heightHour = pindexBest->nHeight;
    int64_t utime = (int64_t)time(NULL);
    int64_t target = utime - wayback;

    while(check)
    {
        if(getBlockTime(heightHour) < target)
        {
            check = false;
            return height - heightHour;
        } else {
            heightHour = heightHour - 1;
        }
    }

    return 0;
}

double getTxTotalValue(std::string txid)
{
    uint256 hash;
    hash.SetHex(txid);

    CTransaction tx;
    uint256 hashBlock = 0;
    if (!GetTransaction(hash, tx, hashBlock))
        return 0;

    CDataStream ssTx(SER_NETWORK, PROTOCOL_VERSION);
    ssTx << tx;

    double value = 0;
    double buffer = 0;
    for (unsigned int i = 0; i < tx.vout.size(); i++)
    {
        const CTxOut& txout = tx.vout[i];

        buffer = value + convertCoins(txout.nValue);
        value = buffer;
    }

    return value;
}

double convertCoins(int64_t amount)
{
    return (double)amount / (double)COIN;
}

std::string getOutputs(std::string txid)
{
    uint256 hash;
    hash.SetHex(txid);

    CTransaction tx;
    uint256 hashBlock = 0;
    if (!GetTransaction(hash, tx, hashBlock))
        return "N/A";

    CDataStream ssTx(SER_NETWORK, PROTOCOL_VERSION);
    ssTx << tx;

    std::string str = "";
    for (unsigned int i = 0; i < tx.vout.size(); i++)
    {
        const CTxOut& txout = tx.vout[i];
        CTxDestination source;
        ExtractDestination(txout.scriptPubKey, source);
        CBitcoinAddress addressSource(source);
        std::string addressStr = addressSource.ToString();
        double buffer = convertCoins(txout.nValue);
        std::string amount = boost::to_string(buffer);
        str.append(addressStr);
        str.append(": ");
        str.append(amount);
        str.append(" DGC");
        str.append("\n");
    }

    return str;
}

std::string getInputs(std::string txid)
{
    uint256 hash;
    hash.SetHex(txid);

    CTransaction tx;
    uint256 hashBlock = 0;
    if (!GetTransaction(hash, tx, hashBlock))
        return "N/A";

    CDataStream ssTx(SER_NETWORK, PROTOCOL_VERSION);
    ssTx << tx;

    std::string str = "";
    for (unsigned int i = 0; i < tx.vin.size(); i++)
    {
        uint256 hash;
        const CTxIn& vin = tx.vin[i];
        hash.SetHex(vin.prevout.hash.ToString());
        CTransaction wtxPrev;
        uint256 hashBlock = 0;
        if (!GetTransaction(hash, wtxPrev, hashBlock))
             return "N/A";

        CDataStream ssTx(SER_NETWORK, PROTOCOL_VERSION);
        ssTx << wtxPrev;

        CTxDestination source;
        ExtractDestination(wtxPrev.vout[vin.prevout.n].scriptPubKey, source);
        CBitcoinAddress addressSource(source);
        std::string inputStr = addressSource.ToString();
        const CScript target = wtxPrev.vout[vin.prevout.n].scriptPubKey;
        double buffer = convertCoins(getInputValue(wtxPrev, target));
        std::string amount = boost::to_string(buffer);
        str.append(inputStr);
        str.append(": ");
        str.append(amount);
        str.append(" DGC");
        str.append("\n");
    }

    return str;
}

int64_t getInputValue(CTransaction tx, CScript target)
{
    for (unsigned int i = 0; i < tx.vin.size(); i++)
    {
        const CTxOut& txout = tx.vout[i];
        if(txout.scriptPubKey == target)
        {
            return txout.nValue;
        }
    }
    return 0;
}

double getTxFees(std::string txid)
{
    uint256 hash;
    hash.SetHex(txid);


    CTransaction tx;
    uint256 hashBlock = 0;
    if (!GetTransaction(hash, tx, hashBlock))
        return 0.0001;

    CDataStream ssTx(SER_NETWORK, PROTOCOL_VERSION);
    ssTx << tx;

    double value = 0;
    double buffer = 0;
    for (unsigned int i = 0; i < tx.vout.size(); i++)
    {
        const CTxOut& txout = tx.vout[i];

        buffer = value + convertCoins(txout.nValue);
        value = buffer;
    }

    double value0 = 0;
    double buffer0 = 0;
    double swp = 0;
    for (unsigned int i = 0; i < tx.vin.size(); i++)
    {
        uint256 hash0;
        const CTxIn& vin = tx.vin[i];
        hash0.SetHex(vin.prevout.hash.ToString());
        CTransaction wtxPrev;
        uint256 hashBlock0 = 0;
        if (!GetTransaction(hash0, wtxPrev, hashBlock0))
             return 0;
        CDataStream ssTx(SER_NETWORK, PROTOCOL_VERSION);
        ssTx << wtxPrev;
        const CScript target = wtxPrev.vout[vin.prevout.n].scriptPubKey;
        swp = convertCoins(getInputValue(wtxPrev, target));
        buffer0 = value0 + convertCoins(getInputValue(wtxPrev, target));
        value0 = buffer0;
    }

    return value0 - value;
}


BlockBrowser::BlockBrowser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BlockBrowser)
{
    ui->setupUi(this);

    setFixedSize(400, 420);

    connect(ui->blockButton, SIGNAL(pressed()), this, SLOT(blockClicked()));
    connect(ui->txButton, SIGNAL(pressed()), this, SLOT(txClicked()));
}

void BlockBrowser::updateExplorer(bool block)
{
    if(block)
    {
        ui->heightLabelBE1->show();
        ui->heightLabelBE1->setTextInteractionFlags(Qt::TextSelectableByMouse);
        ui->heightLabelBE2->show();
        ui->heightLabelBE1->setTextInteractionFlags(Qt::TextSelectableByMouse);
        ui->hashLabel->show();
        ui->hashLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
        ui->hashBox->show();
        ui->hashBox->setTextInteractionFlags(Qt::TextSelectableByMouse);
        ui->merkleLabel->show();
        ui->merkleLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
        ui->merkleBox->show();
        ui->merkleBox->setTextInteractionFlags(Qt::TextSelectableByMouse);
        ui->nonceLabel->show();
        ui->nonceLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
        ui->nonceBox->show();
        ui->nonceBox->setTextInteractionFlags(Qt::TextSelectableByMouse);
        ui->bitsLabel->show();
        ui->bitsLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
        ui->bitsBox->show();
        ui->bitsBox->setTextInteractionFlags(Qt::TextSelectableByMouse);
        ui->timeLabel->show();
        ui->timeLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
        ui->timeBox->show();
        ui->timeBox->setTextInteractionFlags(Qt::TextSelectableByMouse);
        ui->hardLabel->show();
        ui->hardLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
        ui->hardBox->show();;
        ui->hardBox->setTextInteractionFlags(Qt::TextSelectableByMouse);
        ui->pawLabel->show();
        ui->pawLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
        ui->pawBox->show();
        ui->pawBox->setTextInteractionFlags(Qt::TextSelectableByMouse);
        int64_t height = ui->heightBox->value();
		CBlock block;
		CBlockIndex* pindexBest = mapBlockIndex[chainActive.Tip()->GetBlockHash()];
        if (height > pindexBest->nHeight)
        {
            ui->heightBox->setValue(pindexBest->nHeight);
            height = pindexBest->nHeight;
        }
        //int64_t Pawrate = getBlockHashrate(height); Wtf
        //double Pawrate2 = 0.000;
        //Pawrate2 = ((double)Pawrate / 1000000);
        std::string hash = getBlockHash(height);
        std::string merkle = getBlockMerkle(height);
        int64_t nBits = getBlocknBits(height);
        int64_t nNonce = getBlockNonce(height);
        int64_t atime = getBlockTime(height);
        double hardness = getBlockHardness(height);
        QString QHeight = QString::number(height);
        QString QHash = QString::fromUtf8(hash.c_str());
        QString QMerkle = QString::fromUtf8(merkle.c_str());
        QString QBits = QString::number(nBits);
        QString QNonce = QString::number(nNonce);
        QString QTime = QString::number(atime);
        QString QHardness = QString::number(hardness, 'f', 6);
        QString QPawrate = QString::number(0);
        ui->heightLabelBE1->setText(QHeight);
        ui->hashBox->setText(QHash);
        ui->merkleBox->setText(QMerkle);
        ui->bitsBox->setText(QBits);
        ui->nonceBox->setText(QNonce);
        ui->timeBox->setText(QTime);
        ui->hardBox->setText(QHardness);
        ui->pawBox->setText(QPawrate + " MH/s");
    }

    if(block == false) {
        ui->txID->show();
        ui->txID->setTextInteractionFlags(Qt::TextSelectableByMouse);
        ui->txLabel->show();
        ui->txLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
        ui->valueLabel->show();
        ui->valueLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
        ui->valueBox->show();
        ui->valueBox->setTextInteractionFlags(Qt::TextSelectableByMouse);
        ui->inputLabel->show();
        ui->inputLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
        ui->inputBox->show();
        ui->inputBox->setTextInteractionFlags(Qt::TextSelectableByMouse);
        ui->outputLabel->show();
        ui->outputLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
        ui->outputBox->show();
        ui->outputBox->setTextInteractionFlags(Qt::TextSelectableByMouse);
        ui->feesLabel->show();
        ui->feesLabel->setTextInteractionFlags(Qt::TextSelectableByMouse);
        ui->feesBox->show();
        ui->feesBox->setTextInteractionFlags(Qt::TextSelectableByMouse);
        std::string txid = ui->txBox->text().toUtf8().constData();
        double value = getTxTotalValue(txid);
        double fees = getTxFees(txid);
        std::string outputs = getOutputs(txid);
        std::string inputs = getInputs(txid);
        QString QValue = QString::number(value, 'f', 6);
        QString QID = QString::fromUtf8(txid.c_str());
        QString QOutputs = QString::fromUtf8(outputs.c_str());
        QString QInputs = QString::fromUtf8(inputs.c_str());
        QString QFees = QString::number(fees, 'f', 6);
        ui->valueBox->setText(QValue + " DGC");
        ui->txID->setText(QID);
        ui->outputBox->setText(QOutputs);
        ui->inputBox->setText(QInputs);
        ui->feesBox->setText(QFees + " DGC");
    }
}


void BlockBrowser::txClicked()
{
    updateExplorer(false);
}

void BlockBrowser::blockClicked()
{
    updateExplorer(true);
}

void BlockBrowser::setModel(ClientModel *model)
{
    this->model = model;
}

BlockBrowser::~BlockBrowser()
{
    delete ui;
}
