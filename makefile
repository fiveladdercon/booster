OPENSSL = /usr/include

#
# Command Line Targets
#

.PHONY : demo keys

demo : demo/sign demo/verify
	@echo Now is the time for all good men to come to the aide of their country > demo/message.txt
	@cat demo/message.txt  | demo/sign  | tee demo/signed.txt   | demo/verify
	@sed 's/good/dumb/' demo/signed.txt | tee demo/tampered.txt | demo/verify

keys : keys/warden_keys
	cd keys; ./warden_keys

clean :
	rm -rf demo

#
# Compiled Targets
#

keys/warden_keys : src/warden_keys.c
	@mkdir -p keys
	gcc -g3 -O1 -Wall -std=c99 -I/usr/include $^ -o $@ -lcrypto

keys/warden_private_key.h : keys/warden_keys
	cd keys; ./warden_keys

keys/warden_public_key.h : keys/warden_keys
	cd keys; ./warden_keys

demo/sign : keys/warden_private_key.h src/warden_sign.c
	@mkdir -p demo
	gcc -g3 -O1 -Wall -std=c99 -Ikeys -I$(OPENSSL) $^ -D WARDEN_DEMO -o $@ -lcrypto

demo/verify : keys/warden_public_key.h src/warden_verify.c
	@mkdir -p demo
	gcc -g3 -O1 -Wall -std=c99 -Ikeys -I$(OPENSSL) $^ -D WARDEN_DEMO -o $@ -lcrypto

