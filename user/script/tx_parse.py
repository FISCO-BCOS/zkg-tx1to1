#!/usr/bin/python
# -*- coding: UTF-8 -*-

import sys
import json

if len(sys.argv) != 3:
    print "Usage: python tx_parse.py <file_suffix> <tx_json_string>"
    exit()

suffix = sys.argv[1]
tx = json.loads(sys.argv[2])['result']

print json.dumps(tx, sort_keys=True, indent=2)
print "---------------------------------------------------"
if tx['error_code'] != 0 :
    print "Prove error for: " + tx['description']
    exit()

all_info =  json.dumps(tx, sort_keys=True, indent=2)
to_myself = json.dumps(tx['to_myself'], sort_keys=True, indent=2)
to_payee = json.dumps(tx['to_payee'], sort_keys=True, indent=2)
to_verifier = json.dumps(tx['to_verifier'], sort_keys=True, indent=2)


def write_file(filename, text):
    try:
        file_object = open(filename, 'w')
        file_object.write(text)
        file_object.close()
        print "Generate---> " + filename
    except:
        print "Generate file " + filename + " failed"

write_file("info_" + suffix + ".json", all_info)
write_file("to_myself_" + suffix + ".json", to_myself)
write_file("to_payee_" + suffix + ".json", to_payee)
write_file("to_verifier_" + suffix + ".json", to_verifier)
