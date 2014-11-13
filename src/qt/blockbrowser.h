#ifndef BLOCKBROWSER_H
#define BLOCKBROWSER_H

#include "clientmodel.h"
#include "main.h"
#include "wallet.h"
#include "base58.h"
#include <QWidget>

#include <QDir>
#include <QFile>
#include <QProcess>
#include <QTime>
#include <QTimer>
#include <QStringList>
#include <QMap>
#include <QSettings>
#include <QSlider>

double getBlockHardness(int64_t);
double getTxTotalValue(std::string);
double convertCoins(int64_t);
double getTxFees(std::string);
int64_t getBlockTime(int64_t);
int64_t getBlocknBits(int64_t);
int64_t getBlockNonce(int64_t);
int64_t blocksInPastHours(int64_t);
int64_t getBlockHashrate(int64_t);
std::string getInputs(std::string);
std::string getOutputs(std::string);
std::string getBlockHash(int64_t);
std::string getBlockMerkle(int64_t);
bool addnode(std::string);
const CBlockIndex* getBlockIndex(int64_t);
int64_t getInputValue(CTransaction, CScript);


namespace Ui {
class BlockBrowser;
}
class ClientModel;

class BlockBrowser : public QWidget
{
    Q_OBJECT

public:
    explicit BlockBrowser(QWidget *parent = 0);
    ~BlockBrowser();

    void setModel(ClientModel *model);

public slots:

    void blockClicked();
    void txClicked();
    void updateExplorer(bool);

private slots:

private:
    Ui::BlockBrowser *ui;
    ClientModel *model;

};

#endif // BLOCKBROWSER_H
