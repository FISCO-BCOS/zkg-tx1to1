# 监管端使用说明

包括：

- gpkgen：生成监管者公钥的工具


- pkvkgen：生成pk，vk的工具
- govdata：拉取链上数据解密，得到匿名转账信息

## gpkgen

**说明**

​	根据监管者自定义的私钥和选取的生成元，生成监管者公钥

**参数**

``` log
#./gpkgen 
Usage: ./gpkgen <gsk hex string> <(optional) generator>
Eg:    ./gpkgen 123ef 333338d52b05df563667c6c87821f1fb54571e83925617a23f2541254e512c43
Get:   c4a0213fcea718a2f4ea19ae723c033f50b4bfdd8728836f10efdca0e1e647bc
```

| 序号   | 参数             | 说明        | 是否必须 | 条件                                       |
| ---- | -------------- | --------- | ---- | ---------------------------------------- |
| 1    | gsk hex string | 监管者私钥     | 是    | 16进制数，最长64个数字                            |
| 2    | generator      | 监管者选取的生成元 | 否    | 从生成元列表中选，默认： 39061f1c854fae629b599d29cefe1f12bc4809aa681809bfaaeb1b7087be6fed |

**举例**

``` shell
./gpkgen 123ef 333338d52b05df563667c6c87821f1fb54571e83925617a23f2541254e512c43
#Get: c4a0213fcea718a2f4ea19ae723c033f50b4bfdd8728836f10efdca0e1e647bc

./gpkgen 123ef 
#Get: 408600def5bc95ee17e967f05cbcfbcaac88a14b4ecff339269f726e4e356ab4

./gpkgen 123ef 39061f1c854fae629b599d29cefe1f12bc4809aa681809bfaaeb1b7087be6fed
#Get: 408600def5bc95ee17e967f05cbcfbcaac88a14b4ecff339269f726e4e356ab4
```



## pkvkgen

**说明**

​	生成libzkg所需的pk，vk。pk需配置给user端的server_txgen，vk需配置给FISCO-BCOS节点。

**参数**

``` log
#./pkvkgen 
Usage: ./pkvkgen <pk name> <vk name>
Eg:    ./pkvkgen pk.data vk.data
```

| 序号   | 参数      | 说明                 | 是否必须 | 条件        |
| ---- | ------- | ------------------ | ---- | --------- |
| 1    | pk name | 要生成的prove key的文件名  | 是    | 252M的存储空间 |
| 2    | vk name | 要生成的verify key的文件名 | 是    | 无         |

**举例**

```shell
./pkvkgen pk.data vk.data
```



## govdata

**说明**

​	拉取链上匿名转账数据，用监管者私钥进行解密，获取链上所有的匿名转账信息。

**参数**

```log
#./govdata 
Usage: ./govdata <gsk hex string> <node rpc url>
Eg:    ./govdata 12312432432 127.0.0.1:8545
```

| 序号   | 参数             | 说明           | 是否必须 | 条件                               |
| ---- | -------------- | ------------ | ---- | -------------------------------- |
| 1    | gsk hex string | 监管者的私钥       | 是    | 必须与公开的监管者公钥和生成元相匹配。16进制数，最长64个数字 |
| 2    | node rpc url   | 节点的RPC端口的URL | 是    | 无                                |

**举例**

```shell
./govdata 123456 127.0.0.1:8545
./govdata 123456 http://127.0.0.1:8545
```

