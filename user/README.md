# 用户端使用说明

包括：

- **apkgen**：用户公钥生成工具
- **server_txgen**：匿名转账生成server
- **script/prove_query.sh**：根据3v3模型，构造请求字段，去请求server_txgen的工具脚本

## apkgen

### **说明**

​	根据用户自定义的私钥生成公钥

### **参数**

``` log
#./apkgen
Usage: ./apkgen <ask hex string>
Eg:    ./apkgen 123ef
Get:   2a4fdf9331f481329f01f21c928b032e1542004e93af96bf3674ab9957a0c826
```

| 序号   | 参数             | 说明   | 是否必须 | 条件            |
| ---- | -------------- | ---- | ---- | ------------- |
| 1    | ask hex string | 用户私钥 | 是    | 16进制数，最长64个数字 |

### **举例**

``` shell
./apkgen 123ef
#Get: 2a4fdf9331f481329f01f21c928b032e1542004e93af96bf3674ab9957a0c826

./apkgen 2222222222222222222222222222222222222222222222222222222222222222
#Get: af5b55a02961a2f81c5f69930e379e06634f6f7ba06ee3ca60815d86ed9dc25d

./apkgen 22222222222222222222222222222222222222222222222222222222222222222
#[libzkg][INFO] Hex string is out of bound(64 bytes)

./apkgen =222222222222222222222222222222222222222222222222222222222222222
#[libzkg][INFO] Illegal hex string of character =
```



## server_txgen

### 启动方法

**说明**

​	user端生成匿名转账信息的server，用户调用server_txgen，将构造好的3v3模型发到server_txgen上，生成匿名转账信息（匿名转账申请单+接收者私密资产+找零私密资产），供后续发到链上验证使用。

**参数**

``` log
#./server_txgen 
Usage: ./server_txgen <this client port> <node rpc url> <pk file>
Eg:    ./server_txgen 9653 127.0.0.1:8545 pk.data
```

| 序号   | 参数               | 说明               | 是否必须 | 条件        |
| ---- | ---------------- | ---------------- | ---- | --------- |
| 1    | this client port | 此server会创建的rpc端口 | 是    | 端口号不能冲突   |
| 2    | node rpc url     | 区块链上某个节点的rpc端口   | 是    | 节点必须启动    |
| 3    | pk file          | pk.data文件        | 是    | 相对路径或绝对路径 |

**举例**

``` shell
#前台启动
./server_txgen 9653 127.0.0.1:8545 pk.data
#退出ctrl-c

#后台启动
nohup ./server_txgen 9653 127.0.0.1:8545 pk.data > txgen.log 2>&1 &
tail -f txgen.log #看日志
#退出直接kill进程
```

### RPC端口调用

**Request**

请求方法：tx1to1_prove

``` json
{
    "jsonrpc": "2.0",
    "method": "tx1to1_prove",
    "id": 1,
    "params": {
        "prove_data": {
            "ask": "111111",
            "vpub_old": 0,
            "s_v0": 30,
            "s_v1": 20,
            "s_r0": "a33117740ab1dc6d65952b515500bf7e26efedf2bf3945a703c09409eb02b52e",
            "s_r1": "a01a8c347499af1800ae6b3b7dc729e2b38f21052bfcc1cd288f3a19036f7b6c",
            "is_zero_cm0": false,
            "is_zero_cm1": false,
            "r_apk": "e23ab62c572cc3a6855f3af75eccaca406ed910b5c507f1f67c478d151b986a0",
            "vpub_new": 50,
            "r_v": 0,
            "g": "39061f1c854fae629b599d29cefe1f12bc4809aa681809bfaaeb1b7087be6fed",
            "Gpk": "9024aeaad0ad8f4cfb58d90ffc76e4c4e129cf3cdea8ec5de2c68c7cefe479d1"
        }
    }
}
```

| 参数          | 说明                         | 条件                                       |
| ----------- | -------------------------- | ---------------------------------------- |
| ask         | 金额发送者私钥                    | 16进制数，最长64个数字                            |
| vpub_old    | 发送者发送的公开金额                 | 整数：0 ~ 18446744073709551615              |
| is_zero_cm0 | 是否为0承诺，若true，则s_v0，s_r0 无效 | true 或 false                             |
| s_v0        | 私密资金0的金额                   | is_zero_cm0 为false有效。整数：0 ~ 18446744073709551615 |
| s_r0        | 私密资金0的spend_key            | is_zero_cm0 为false有效                     |
| is_zero_cm1 | 是否为0承诺，若true，则s_v1，s_r1 无效 | true 或 false                             |
| s_v1        | 私密资金1的金额                   | is_zero_cm1 为false有效。整数：0 ~ 18446744073709551615 |
| s_r1        | 私密资金1的spend_key            | is_zero_cm1 为false有效                     |
| r_apk       | 金额接收者的公钥                   | apkgen生成                                 |
| vpub_new    | 接收者接收的公开金额                 | 整数：0 ~ 18446744073709551615              |
| r_v         | 接收者接收的私密资金                 | 整数：0 ~ 18446744073709551615              |
| g           | 生成gpk的生成元generator         | 从生成元列表中选取即可                              |
| Gpk         | 监管者公钥gpk                   | 必须是从g生成而来                                |

**Response Result**

``` json
{
    "id": 1,
    "jsonrpc": "2.0",
    "description": "Prove success!",
    "error_code": 0,
    "result": {
        "to_myself": {
            "description": "Use my ask to spend",
            "spend_key": "d230937593fb605533b284488d138266853c2513e4b612ae258932e444a1755d",
            "value": 0
        },
        "to_payee": {
            "description": "Use payee's ask to spend",
            "spend_key": "9c98153ec7aed1eb2471c6278d454072586f013691897c4145906fe705dd475e",
            "value": 0
        },
        "to_verifier": {
            "G_data": "ayebZ4rTQ96YnwgcNgLslkaqe+Q3FU4Bxll+aOI+ZnCCshA1wQ3ZnmyKYrCSixhTxqIDBj8SH53UwNzW8UGOmLsUHa7BmgaYZuWOrp6Og/qZwKCZCbW5u+wmB3eT4Y8QEEZx2v/RAbxns7uLBmaqVubom5G6MZg0xS8RxguwwQgYIeu5zcCXzCjB7S0CIDxV2cPBXJafK7FIgTXWb64g0LByzzWrE5lGqn+2jwf8pJKacEt5wbHjtBlfMFCKvy9Qkre2MbSTnptLpSkq8W41Sp45i4LCtCjYyrvWlIZtmmA=",
            "Gpk": "9024aeaad0ad8f4cfb58d90ffc76e4c4e129cf3cdea8ec5de2c68c7cefe479d1",
            "description": "Send me to verifier",
            "g": "39061f1c854fae629b599d29cefe1f12bc4809aa681809bfaaeb1b7087be6fed",
            "proof": "MPLqbCtpc0WooZ76ucdqgCDakJsM6d9DRMo8sWHE75kJMTCp+uO/4uQTLZtQgWssrABkTXWKAMj5bcn9i+ahkUEyIDAw+OwRR2ion99Eje6XEtzrMIBjD+yJo0yVZSrIuIFfaRmLi65QcsBN2XMmLmorUJfpsqX8npQ2QIu1Vjr/HVtrATEwwH1WIHl3Vfae2e5ddqMm3+dtlkNs18pE28m44aFxIwExMHa/iQAvj2Lcaw6U/rfDFxVgtBq9B703KFeorcQ3lUcbMDB8QUbwCP74S/qZwHj/vXv2Rl5slZnmGPEAG398PltsHjEwjbg5dw1dLGW/9+NGyypp4ySGiVDtrvM7P01pS3uGfhsxMOeysi+Bx2+kPqthetFFMD4TKL+g63xFcAIAxMqB75IuMQ==",
            "r_cms0": "77e335d29e4cd43989b20393e04f2384a7c638c8430c7b7ecb189dd64288289b",
            "r_cms1": "fe192729d0cc1aec8993cb3a63d184acbaa3ff6751e8acf9de0334af1d2af4b6",
            "s_rts0": "85a837661e143068e54113c64c8cabd19c111504c05c675ded2fd2b305d60f53",
            "s_rts1": "85a837661e143068e54113c64c8cabd19c111504c05c675ded2fd2b305d60f53",
            "s_sns0": "e6236a5e82bfc42808c0583a1a75ab3dc3a6dd6c1523359c5d1f4178a65cc350",
            "s_sns1": "20b69eab7f4d86e7dd0ba2240ebe9df0b51fc4798e2b562521ef6be97cd98e09",
            "vpub_new": 50,
            "vpub_old": 0
        }
    }
}
```

**返回状态参数**

| 参数          | 说明          |
| ----------- | ----------- |
| error_code  | 错误码：0成功，1失败 |
| description | 错误描述        |

**to_myself 找零私密资金信息**

| 参数          | 说明                                       |
| ----------- | ---------------------------------------- |
| description | 描述                                       |
| spend_key   | 花费这笔私密资金所需的key，需配合资金所有者的私钥ask使用。（自己的私钥ask） |
| value       | 私密金额                                     |

**to_payee 给接收者的私密资金信息**

| 参数          | 说明                                       |
| ----------- | ---------------------------------------- |
| description | 描述                                       |
| spend_key   | 花费这笔私密资金所需的key，需配合资金所有者的私钥ask使用（接收者的私钥ask） |
| value       | 私密金额                                     |

**to_verifier 匿名转账申请单**

| 参数          | 说明       |
| ----------- | -------- |
| description | 描述       |
| vpub_old    | 发送者公开金额  |
| s_rts0,1    | 承诺树的根    |
| s_sns0,1    | 老承诺的序列号  |
| vpub_new    | 接收者公开参数  |
| r_cms0,1    | 新承诺      |
| proof       | 零知识证明字段  |
| g           | 监管指定的生成元 |
| Gpk         | 监管者公钥    |
| G_data      | 转账密文     |



## script/prove_query.sh

### **说明**

​	构造3v3模型的请求字段，请求server_txgen的工具脚本。

​	需要根据3v3模型，先手动修改脚本内的参数，再运行此脚本。

### **参数**

脚本内参数说明（只需配置Configure Start 到Configure End之间的参数）

| 参数                     | 说明                                       | 条件                                       |
| ---------------------- | ---------------------------------------- | ---------------------------------------- |
| payer_secret_key       | 金额发送者私钥                                  | 16进制数，最长64个数字                            |
| payer_public_value     | 发送者发送的公开金额                               | 整数：0 ~ 18446744073709551615              |
| enable_shielded_input0 | 私密资金输入0的使能flag，若为false，则payer_shielded_value0，spend_key0无效 | true 或 false                             |
| payer_shielded_value0  | 私密资金0的金额                                 | enable_shielded_input0为true有效。整数：0 ~ 18446744073709551615 |
| spend_key0             | 私密资金0的spend_key                          | enable_shielded_input0为true有效            |
| enable_shielded_input1 | 私密资金输入1的使能flag，若为false，则payer_shielded_value1，spend_key1无效 | true 或 false                             |
| payer_shielded_value1  | 私密资金1的金额                                 | enable_shielded_input1为true有效。整数：0 ~ 18446744073709551615 |
| spend_key1             | 私密资金1的spend_key                          | enable_shielded_input1为true有效            |
| payee_public_key       | 金额接收者的公钥                                 | apkgen生成                                 |
| payee_public_value     | 接收者接收的公开金额                               | 整数：0 ~ 18446744073709551615              |
| payee_shielded_value   | 接收者接收的私密资金                               | 整数：0 ~ 18446744073709551615              |
| generator              | 生成gpk的生成元generator                       | 从生成元列表中选取即可                              |
| overseer_public_key    | 监管者公钥gpk                                 | 必须是从g生成而来                                |

调用脚本参数说明

``` shell
sh prove_query.sh <server_txgen_url> <(opetional) suffix>
```

| 序号   | 参数               | 说明                               | 是否必须 | 条件               |
| ---- | ---------------- | -------------------------------- | ---- | ---------------- |
| 1    | server_txgen_url | server_txgen的url(ip:port)        | 是    | server_txgen必须启动 |
| 2    | suffix           | 可指定生成的匿名转账文件的后缀，便于区分每一笔生成的匿名转账信息 | 否    |                  |

### **举例**

3v3模型：A[50] -> B(20) + B[10] + A[20]模型如下。

```log
    A.ask ----> B.apk 
          ____
 A(0)  --|    |-- B(20)
 A[50] --|    |-- B[10]
     0 --|____|-- A[20]
     
```

脚本内参数

``` shell
#Payer(Sender S)
payer_secret_key="111111" #A.ask
payer_public_value=0

    #Payer shielded input 0
    enable_shielded_input0=true
        #if enable
        payer_shielded_value0=50
        spend_key0="7f917ca21ea263273f4c6b59ea6966310ddd4ef723acf154940b5d5cea84bbdd" #场景5合账后得到的50元，此私密50元的spend_key

    #Payer shielded input 1
    enable_shielded_input1=false
        #if enable
        payer_shielded_value1=0
        spend_key1=""

#Payee(Receiver R)
payee_public_key="c3c58c87f0a6a4ffea20f0e9b5a75b611ac27d9d9813a9b0bf4d9d606a34fbb8" #B.apk
payee_public_value=20 #提款20，转公开
payee_shielded_value=10 #给B 10元私密，剩下20元找零给A，会自动计算找零

#Overseer(Goverment G)
generator="39061f1c854fae629b599d29cefe1f12bc4809aa681809bfaaeb1b7087be6fed" #generator.key
overseer_public_key="9024aeaad0ad8f4cfb58d90ffc76e4c4e129cf3cdea8ec5de2c68c7cefe479d1" #gov.gpk
```

脚本执行

``` shell
sh prove_query.sh 127.0.0.1:9653 4#参数为server_txgen的ip:port suffix
#Generate---> info_4.json
#Generate---> to_myself_4.json
#Generate---> to_payee_4.json
#Generate---> to_verifier_4.json

sh prove_query.sh 127.0.0.1:9653 #参数为server_txgen的ip:port 自动生成按时间戳的suffix
#Generate---> info_1524135774.json
#Generate---> to_myself_1524135774.json
#Generate---> to_payee_1524135774.json
#Generate---> to_verifier_1524135774.json
```

### 返回字段

​	即为参考server_txgen的返回字段。参考server_txgen的Response Result部分。

### 生成文件

| 文件                    | 描述                                       |
| --------------------- | ---------------------------------------- |
| info_xxxx.json        | server_txgen返回的所有信息                      |
| to_myself_xxxx.json   | 找零私密资金信息                                 |
| to_payee_xxxx.json    | 给到接收者的字谜资金信息                             |
| to_verifier_xxxx.json | 匿名转账申请单，需发到链上进行验证，验证通过后，to_myself_xxxx.json和to_payee_xxxx.json对应的资金变为有效。<br>发送命令: ```babel-node sendZkgTx.js to_verifier_xxxx.json``` |

​	