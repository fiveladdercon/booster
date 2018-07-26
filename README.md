Warden
======

A lightweight crypto sign & verify API built on the openssl crypto library to 
manage the features available in a publicly distributed application.

The intent is to embed a function in a publicly distributed application that will
verify that feature authorization comes from the application distributor and 
not a third party.

This is accomplished by the application distributor cryptographically signing 
the feature authorization with a private key and having the application verify 
the feature authorization with the associated public key.


Features:

- a sign API that can be embedded a in private feature authorization utility.
- a verify API that can be embedded in and distibuted in a public application.
- a key generation utility that generates public and private keys as separate
  header files.


Setup
-----

```
$ git clone https://github.com/fiveladdercon/warden.git
$ cd warden
$ make
```

This will:
- compile the key generation utility, keys/warden_keys, 
- generate a private/public key pair, keys/*.h, and
- compile and run a sign & verify demo

If all goes well:
- You may get a compiler warning that the RSA_generate_key function has been 
  depricated.  That's OK.  Everything still works.  If you figure out how to
  call RSA_generate_key_ex, let me know.
- You'll see two line of text.  The one in green has been verified successfully,
  while the one in red has been tampered with and fails the check.

If things didn't go well, it's probably because the openssl header files can't
be found.  In this case, find the headers, update the OPENSSL macro in the
makefile and try again. 


Keys
----

Once compiled, the warden_keys utility will generate a new warden_public_key.h 
and warden_private_key.h pair.

Though it is quick and easy to generate new keys, once your application has been
publicly released, the keys you released with are the ones you need to stick
with for good.  Which means to say that you should check in your keys *before* 
you release so that you don't loose your private key *after* your release.

But of course when you do check in your keys, be aware of where you check them
into.  It's fine to check in the public key into a public repository, but 
don't check the private key into a public repository, as now anyone can sign
with authority.

*If you've forked this repository, you wont't be able to check them in unless 
you change the .gitignore - but then I'm not too sure why you'd do that.*


Sign
----

The warden_sign.c file contains the following API for signing messages:

```c
int WARDEN_sign (const char *imessage, char *osignature);
```

This function will sign the message `imessage` and return a printable signature 
in `osignature`, which must be a 513 element character array to hold the SHA256
digest as printable characters.

The function will return 1 on success or 0 on failure.

Note that both the message and the signature must be combined and transmitted as
a **signed message** to the public application for verification.


Verify
------

The WARDEN_verify.c file contains the following API for verifying signed
messages:

```c
int WARDEN_verify (const char *imessage, const char *isignature);
```

This function will verify that the message `imessage` with the signature 
`isignature` was signed by the application distributor.

The function will return 1 on success or 0 on failure.

Note that public application will receive a **signed message** and that the 
application will have to split it into message and signature for verification.


Notes
-----

The signed & verified message is just a string, so you can serialize what ever 
kind of feature authorization is needed into the message, then deserialize the
message once it has been verified.

One thing to consider is that signed messages are public and thus freely 
distributed, which means that without a mechanism to dissuade this kind of 
distribution, the whole system can be defeated.

The first line of defence is to always serialize an expiry date, in which case 
if the feature set is leaked, at least it won't be forever.

A second line of defence against this is to serialize the contact information of 
the intended recipient into the authorization, in which case if the feature set 
is leaked, we all know darn well who leaked it.  This will deter most *individuals*, 
as it is a clear, public statement that they are ripping you off, but it has
pitfalls when it comes to *organizations*, so use with caution.  As a rule,
the larger the organization, the more trustworthy, as legitimate organizations 
generally don't become large without building public trust.

Another thing to consider is that automated feature authorization is easily
defeated, as it is easy to script the request and unpack the result.


Acknowledgements
----------------

This API is pretty much a verbatim walk through of the [T-rsa.c.tar.gz][1]
sample program on the [OpenSSL Sign & Verify wiki page][2].

All I did was separate the key generation, sign and verify functions into separate
files, inline and optimize the underlying calls and use the RSA get/set macros &
BN conversion macros to write to and include the keys from header files.

But since the OpenSSL documentation is somewhat cryptic *(pun intended)*, I 
spent quite a few hours sifting through the [source code][3] to get the key 
persistence work.

[1]: https://wiki.openssl.org/images/e/eb/T-rsa.c.tar.gz
[2]: https://wiki.openssl.org/index.php/EVP_Signing_and_Verifying
[3]: https://github.com/openssl/openssl