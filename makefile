#
# Command Line Targets
#

test : demo/sign demo/verify
	@echo Now is the time for all good men to come to the aide of their country > demo/message.txt
	@cat demo/message.txt  | demo/sign  | tee demo/signed.txt   | demo/verify
	@sed 's/good/dumb/' demo/signed.txt | tee demo/tampered.txt | demo/verify

keys : bin/keys
	./bin/keys

clean :
	rm -rf bin demo

#
# Compiled Targets
#

bin/keys : src/warden_keys.c
	mkdir -p bin keys
	gcc -g3 -O1 -Wall -std=c99 -I/usr/include $^ -o $@ -lcrypto

keys/warden_private_key.h : bin/keys
	./bin/keys

keys/warden_public_key.h : bin/keys
	./bin/keys

demo/sign : keys/warden_private_key.h src/warden_sign.c | demo
	gcc -g3 -O1 -Wall -std=c99 -Ikeys -I/usr/include $^ -D WARDEN_DEMO -o $@ -lcrypto

demo/verify : keys/warden_public_key.h src/warden_verify.c
	gcc -g3 -O1 -Wall -std=c99 -Ikeys -I/usr/include $^ -D WARDEN_DEMO -o $@ -lcrypto

