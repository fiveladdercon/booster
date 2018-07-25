Booster
=======

A lightweight crypto sign & verify API built on the openssl crypto library to 
"boost" the features available in a publicly distributed application.

The intent is to embed a function in a publicly distributed application that will
verify that "feature authorization" comes from the application distributor and 
not a third party.

This is accomplished by the application distributor signing the "feature 
authorization" with a private key and having the application verify the "feature
authorization" with it's embedded public key.


Features:

- a sign API that can be embedded a in private "feature authorization" utility.
- a verify API that can be embedded in and distibuted in a public application.
- a key generation utility that generates public and private keys as separate
  header files.


Setup
-----

```
$ git clone https://github.com/fiveladdercon/booster.git
$ cd booster
$ make
```

This will:
- compile the key generation utility (bin/keys); 
- generate a private/public key pair (private/key.h & public/key.h); 
- compile sign & verify demos (bin/sign, bin/verify);
- run a test

If all goes well:
- You'll get a warning about a depricated function (RSA_generate_key) when 
  compiling.  That's OK.  Everything still works.
- You'll see two line of text.  The one in green has been verified successfully,
  while the one in red has been tampered with and fails the check.



Sign
----

```c
extern int sign (const char *imessage, char *osignature);
```

This function will sign the message `*imessage` and return a "printable" signature 
in `*osignature`, which must be a 513 element character array to hold the SHA256
digest as printable characters.

The function will return 1 on success or 0 on failure.

Note that both the message and the signature must be combined and transmitted as
a "signed message" to the public application for verification.


Verify
------

```c
extern int verify (const char *imessage, const char *isignature);
```

This function will verify that the message `*imessage` with the signature 
`*isignature` was signed by the application distributor.

The function will return 1 on success or 0 on failure.

Note that public application will receive a "signed message" and that the 
application will have to split it into the message and signature for verification.


Notes
-----

The signed & verified message is just a string, so an application developper can
serialize what ever kind of "feature authorization" is needed into the message,
then deserialize the message once it has been verified.

One thing to consider is that the signed messages are public, so they can be
freely distributed, which means that without a mechanism to dissuade this kind
of distribution, the whole system can be defeated.

The first line against this is to alwasy serialize an expiry date, in which case
if the feature set is leaked, at least it won't be forever.

The second line against this is to serialize the personal contact information of
the recipient into the authorization, in which case if the feature set is leaked, 
you know who leaked it.  This will deter most people, as it is a clear, public 
statement that they are ripping you off.

Another thing to consider is that the granting of "trial" feature authorization
should not be automated, as it is easy to script issuing the request and unpacking
the result, again defeating the system.
