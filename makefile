#
# Command Line Targets
#

all : test

keys : bin/keys
	./bin/keys

sign : bin/sign

verify : bin/verify

test : bin/sign bin/verify
	@echo Now is the time for all good men to come to the aide of their country > message.txt
	@cat message.txt | ./bin/sign | tee signed.txt | ./bin/verify
	@sed 's/good men/ugly women/' signed.txt | tee tampered.txt | ./bin/verify

# RE "ugly women": I'm not the bad guy; I'm only acting like one for the purpose of demonstration.

clean :
	rm -rf bin *.txt

#
# Compiled Targets
#

bin/keys : setup/keys.c | bin
	gcc -g3 -O1 -Wall -std=c99 -I/usr/include $^ -o $@ -lcrypto

private/key.h : bin/keys
	./bin/keys

public/key.h : bin/keys
	./bin/keys

bin/sign : private/key.h private/sign.c | bin
	gcc -g3 -O1 -Wall -std=c99 -Iprivate -I/usr/include $^ -D DEMO -o $@ -lcrypto

bin/verify : public/key.h public/verify.c | bin
	gcc -g3 -O1 -Wall -std=c99 -Ipublic -I/usr/include $^ -D DEMO -o $@ -lcrypto

#
# Directories
#

bin :
	mkdir -p $@
