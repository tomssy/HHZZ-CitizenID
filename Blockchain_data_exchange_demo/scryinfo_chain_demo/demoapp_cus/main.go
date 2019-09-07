package main

import (
	"fmt"
	"github.com/ethereum/go-ethereum/common"
	"github.com/scryinfo/iscap_demo/src/sdk"
	"github.com/scryinfo/iscap_demo/src/sdk/core/chainoperations"
	"github.com/scryinfo/iscap_demo/src/sdk/core/ethereum/events"
	"github.com/scryinfo/iscap_demo/src/sdk/scryclient"
	cif "github.com/scryinfo/iscap_demo/src/sdk/scryclient/chaininterfacewrapper"
	"github.com/scryinfo/iscap_demo/src/sdk/util/accounts"
    "github.com/widuu/goini"
    "math/big"
    "os"
    "strconv"
    "strings"
    "time"
)


var (
    accountList                                       = ""
    protocolContractAddr                           = ""
    tokenContractAddr                              = ""
    ethNodeAddr                                    = ""
    keyServiceAddr                                 = ""
    ipfsAddr                                       = ""


    appIndex                                       = 0
    appTitle                                       = "demoxrxapp"
    buyerAddr                                      = ""
	publishId                                      = ""
	txId                    *big.Int = big.NewInt(0)
	metaDataIdEncWithSeller []byte
	metaDataIdEncWithBuyer  []byte
    suAddress                                      = "0xd280b60c38bc8db9d309fa5a540ffec499f0a3e8"
    suPassword                                     = "111111"
    clientPassword                                 = "888888"
	buyer                   *scryclient.ScryClient = nil
	sleepTime               time.Duration          = 15000000000
)

func main() {
    readConfig()

    var err error
    wd, _ := os.Getwd()
    if len(os.Args) == 2 {
        appIndex, err = strconv.Atoi(os.Args[1])
        if err != nil {
            fmt.Println("failed to parse args:", err)
            os.Exit(1)
        }
    } else {
        fmt.Println("Invalid args")
    }

    logPath := wd + "/log/scry_app_buyer_test.log"
    appName := appTitle
    fmt.Println("app name:", appName, "; log path:", logPath)

    err = sdk.Init(
        ethNodeAddr,
        keyServiceAddr,
        protocolContractAddr,
        tokenContractAddr,
        0,
        ipfsAddr,
        logPath,
        appName)

	if err != nil {
		fmt.Println("failed to initialize sdk, error:", err)
		return
	}

	initClients()
	testTxWithoutVerify()

	time.Sleep(100000000000000)
}

func readConfig() {
    conf := goini.SetConfig("./config.ini")
    accountList                                    = conf.GetValue("account", "accounts")
    protocolContractAddr                           = conf.GetValue("application", "protocol_contract_addr")
    tokenContractAddr                              = conf.GetValue("application", "token_contract_addr")
    ethNodeAddr                                    = conf.GetValue("application", "eth_node_addr")
    keyServiceAddr                                 = conf.GetValue("application", "key_service_addr")
    ipfsAddr                                       = conf.GetValue("application", "ipfs_addr")
}

func testTxWithoutVerify() {
	fmt.Println("rdemo")
    buyer.SubscribeEvent("DataPublish", onPublish)
}


func initClients() {
	var err error
    accountArr := strings.Split(accountList, ",")
    if len(accountArr) < (appIndex+1) * 2 {
        fmt.Println("no enough accounts")
        os.Exit(1)
    }

    buyerAddr = accountArr[appIndex*2]
    fmt.Println("buyer address:", buyerAddr)

	buyer, err = CreateClient(buyerAddr)
	if err != nil {
		fmt.Println("failed to init clients, error:", err)
		panic(err)
	}

    buyer.SubscribeEvent("Approval", onApprovalBuyerTransfer)
    buyer.SubscribeEvent("TransactionCreate", onTransactionCreate)
    buyer.SubscribeEvent("ReadyForDownload", onReadyForDownload)
    buyer.SubscribeEvent("TransactionClose", onClose)
}

func CreateClient(address string) (*scryclient.ScryClient, error) {
    return scryclient.NewScryClient(address), nil
}


func BuyerApproveTransfer() {
    txParam := chainoperations.TransactParams{
        From: common.HexToAddress(buyer.Account.Address),
        Password: clientPassword,
    }
	err := cif.ApproveTransfer(&txParam,
	    common.HexToAddress(protocolContractAddr),
	    big.NewInt(1001))
	if err != nil {
		fmt.Println("BuyerApproveTransfer:", err)
	}
}

func PrepareToBuy(publishId string) {
    txParam := chainoperations.TransactParams{
        From: common.HexToAddress(buyer.Account.Address),
        Password: clientPassword,
    }
	err := cif.PrepareToBuy(&txParam, publishId)
	if err != nil {
		fmt.Println("failed to prepareToBuy, error:", err)
	}
}

func Buy(txId *big.Int) {
    txParam := chainoperations.TransactParams{
        From: common.HexToAddress(buyer.Account.Address),
        Password: clientPassword,
    }
	err := cif.BuyData(&txParam, txId)
	if err != nil {
		fmt.Println("failed to buyData, error:", err)
	}
}

func ConfirmDataTruth(txId *big.Int) {
    txParam := chainoperations.TransactParams{
        From: common.HexToAddress(buyer.Account.Address),
        Password: clientPassword,
    }
	err := cif.ConfirmDataTruth(
	    &txParam,
	    txId,
	    true)
	fmt.Println("ConfirmDataTruth:", txId)
	if err != nil {
		fmt.Println("failed to ConfirmDataTruth, error:", err)
	}
}

func onReadyForDownload(event events.Event) bool {
	fmt.Println("onReadyForDownload:", event)
	metaDataIdEncWithBuyer = event.Data.Get("metaDataIdEncBuyer").([]byte)

	metaDataId, err := accounts.GetAMInstance().Decrypt(
	    metaDataIdEncWithBuyer,
	    buyer.Account.Address,
	    clientPassword)

    if err != nil {
        fmt.Println("failed to decrypt meta data id with buyer's private key", err)
        return false
    }

	fmt.Println("meta data id:", string(metaDataId))

    ConfirmDataTruth(txId)
	return true
}

func onClose(event events.Event) bool {
	fmt.Println("onClose:", event)
    fmt.Println("Testing end")

	// os.Exit(0)
	return true
}

func onApprovalBuyerTransfer(event events.Event) bool {
	fmt.Println("onApprovalBuyerTransfer:", event)

	PrepareToBuy(publishId)
	return true
}

func onTransactionCreate(event events.Event) bool {
	fmt.Println("onTransactionCreated:", event)

	txId = event.Data.Get("transactionId").(*big.Int)
	proofIDs := event.Data.Get("proofIds").([][32]byte)

    for _, proofId := range proofIDs {
        proofId, err := cif.Bytes32ToIpfsHash(proofId)
        if err != nil {
            panic(err)
        }

        fmt.Println("proof id:", proofId)
    }

    Buy(txId)
	return true
}

func onPublish(event events.Event) bool {
	fmt.Println("onpublish: ", event)

	publishId = event.Data.Get("publishId").(string)
	despDataId := event.Data.Get("despDataId").(string)
    price := event.Data.Get("price").(*big.Int)

	fmt.Println("publishId", publishId,", despDataId:", despDataId, ", price:", price)

    BuyerApproveTransfer()
	return true
}
