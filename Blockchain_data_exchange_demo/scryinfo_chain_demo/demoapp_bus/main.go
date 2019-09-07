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

	publishId                                      = ""
    sellerAddr                                     = ""
	txId                    *big.Int = big.NewInt(0)
	metaDataIdEncWithSeller []byte
	metaDataIdEncWithBuyer  []byte
    clientPassword                                 = "888888"
	seller                  *scryclient.ScryClient = nil
	sleepTime               time.Duration          = 15000000000
	appIndex                                       = 0
	appTitle                                       = "demoxrxapp"
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

    logPath := wd + "/log/scry_app_seller_test.log"
    appName := appTitle 
    fmt.Println("app name:", appName, "; for 190324 show demo")

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


    for {
            var str string
            fmt.Scanln(&str)
            fmt.Printf("INPUT OK", str);
            if str!= ""{
                testTxWithoutVerify(str)
            }
            if str== "e"{
                os.Exit(0)
            }
    }
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

func testTxWithoutVerify(sdemo string) {
	fmt.Println("demo business")

	SellerPublishData(sdemo)
}


func initClients() {
	var err error
	accountArr := strings.Split(accountList, ",")
    if len(accountArr) < (appIndex+1) * 2 {
        fmt.Println("no enough accounts")
        os.Exit(1)
    }

    index := appIndex*2 + 1
	sellerAddr = accountArr[index]
	fmt.Println("baddress:", sellerAddr)

	seller, err = CreateClient(sellerAddr)
	if err != nil {
		fmt.Println("failed to init clients, error:", err)
		panic(err)
	}

    seller.SubscribeEvent("DataPublish", onPublish)
    seller.SubscribeEvent("Buy", onPurchase)
    seller.SubscribeEvent("TransactionCreate", onTransactionCreate)
    seller.SubscribeEvent("TransactionClose", onClose)
}

func CreateClient(address string) (*scryclient.ScryClient, error) {
    return scryclient.NewScryClient(address), nil
}

func SellerPublishData(sdemo string) {
	//publish data
	metaData := []byte(sdemo)  //after success
	proofData := [][]byte{{'4', '5', '6', '3'}, {'2', '2', '1'}}     //after 1
	despData := []byte{'7', '8', '9', '3'}      //describe

	txParam := chainoperations.TransactParams{
	    From: common.HexToAddress(seller.Account.Address),
	    Password: clientPassword,
    }

	cif.Publish(
	    &txParam,
	    big.NewInt(1000),
	    metaData,
	    proofData,
	    2,
	    despData,
	)
}


func SubmitMetaDataIdEncWithBuyer(txId *big.Int) {
    txParam := chainoperations.TransactParams{
        From: common.HexToAddress(seller.Account.Address),
        Password: clientPassword,
    }
	err := cif.SubmitMetaDataIdEncWithBuyer(
	    &txParam,
	    txId,
	    metaDataIdEncWithBuyer)
	fmt.Println("SubmitData:", txId, txParam)
	if err != nil {
		fmt.Println("failed to SubmitMetaDataIdEncWithBuyer, error:", err)
	}
}

func onPurchase(event events.Event) bool {
	fmt.Println("onPurchase:", event)

	metaDataIdEncWithSeller = event.Data.Get("metaDataIdEncSeller").([]byte)
	buyerAddr := event.Data.Get("buyer").(common.Address)

	var err error
    metaDataIdEncWithBuyer, err = accounts.GetAMInstance().ReEncrypt(
        metaDataIdEncWithSeller,
        seller.Account.Address,
        buyerAddr.String(),
        clientPassword,
    )

    if err != nil {
        fmt.Println("failed to ReEncrypt meta data id with buyer's public key")
        return false
    }

    SubmitMetaDataIdEncWithBuyer(txId)
	return true
}


func onPublish(event events.Event) bool {
	fmt.Println("onpublish: ", event)

	publishId = event.Data.Get("publishId").(string)
	despDataId := event.Data.Get("despDataId").(string)
    price := event.Data.Get("price").(*big.Int)

	fmt.Println("despDataId:", despDataId, ", price:", price)
	return true
}


func onClose(event events.Event) bool {
    fmt.Println("onClose:", event)
    fmt.Println("Testing end")

    // os.Exit(0)
    return true
}

func onTransactionCreate(event events.Event) bool {
    fmt.Println("onTransactionCreated:", event)

    txId = event.Data.Get("transactionId").(*big.Int)

    return true
}