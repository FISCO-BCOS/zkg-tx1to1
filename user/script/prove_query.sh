#! /bin/sh

: '
Shielded Overseen Transaction:

Role:   Payer(Sender S)   ->    Payee(Receiver R)  ->   Overseer(Goverment G)
Keys:   $payer_secret_key       $payee_public_key      $overseer_public_key, $generator

MODEL OVERVIEW:       ______
S ---  vpub_old  --->|      |--- vpub_new ---> R
                     |      |
S --- s_v0, s_k0 --->|  Tx  |---  r_v     ---> R
                     |      |
S --- s_v1, s_k1 --->|______|---  change_v---> S (S change)
                        ||
                        \/ 
                      G_data

MODEL INSIGHT: [input params], (generated params)
Payer public input:                   __________        Payee public output:
    [$payer_public_value]       ---->|          |-----> [$payee_public_value]
                                     |          |
                                     |          |
Payer shielded input 0:              |          |       
    [$enable_shielded_input0,        |          |       Payee shielded output:
        $payer_shielded_value0, ---->|    Tx    |-----> [$payee_shielded_value]
        $spend_key0]                 |          |       (spend_key)
                                     |          |
Payer shielded input 1:              |          |       
    [$enable_shielded_input1,        |          |       Payer shielded change:
        $payer_shielded_value1, ---->|          |-----> (change_value, spend_key)
        $spend_key1]                 |__________|
                                          ||
                                          \/ 
                                   (proof, G_data)
'
#server_txgen url
txgen_url=$1
suffix=$2
##################################### Configure Start ##########################################

#Payer(Sender S)
payer_secret_key="111111" #A.ask
payer_public_value=100

    #Payer shielded input 0
    enable_shielded_input0=false
        #if enable
        payer_shielded_value0=0
        spend_key0=""

    #Payer shielded input 1
    enable_shielded_input1=false
        #if enable
        payer_shielded_value1=0
        spend_key1=""

#Payee(Receiver R)
payee_public_key="e23ab62c572cc3a6855f3af75eccaca406ed910b5c507f1f67c478d151b986a0" #A.apk
payee_public_value=0
payee_shielded_value=100

#Overseer(Goverment G)
generator="39061f1c854fae629b599d29cefe1f12bc4809aa681809bfaaeb1b7087be6fed" #generator.key
overseer_public_key="9024aeaad0ad8f4cfb58d90ffc76e4c4e129cf3cdea8ec5de2c68c7cefe479d1" #gov.gpk
##################################### Configure End ############################################

is_zero_cm0=true
if [ "$enable_shielded_input0" == "true" ]; then
is_zero_cm0=false
fi

is_zero_cm1=true
if [ "$enable_shielded_input1" == "true" ]; then
is_zero_cm1=false
fi

if [ "$suffix" == "" ]; then
    suffix="`date +%s`"
fi

#exit
python tx_parse.py $suffix "`
curl --data '{"jsonrpc":"2.0","method":"tx1to1_prove","id":1,"params":{"prove_data":
{
    "ask" : "'$payer_secret_key'",
    "vpub_old" : '$payer_public_value',
    "s_v0" : '$payer_shielded_value0', 
    "s_v1" : '$payer_shielded_value1',
    "s_r0" : "'$spend_key0'", 
    "s_r1" : "'$spend_key1'",
    "is_zero_cm0" : '$is_zero_cm0', 
    "is_zero_cm1" : '$is_zero_cm1',
    "r_apk" : "'$payee_public_key'",
    "vpub_new" : '$payee_public_value', 
    "r_v" : '$payee_shielded_value',
    "g" : "'$generator'",
    "Gpk" : "'$overseer_public_key'"
}
}}' $txgen_url`"

