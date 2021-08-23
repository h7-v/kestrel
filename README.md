# Kestrel
## _A Basic Blockchain Simulator_

A basic blockchain node written in C++ with QT as an introduction for myself into blockchain technology.
All source code is written using [Google's C++ Style Guide](https://google.github.io/styleguide/cppguide.html).

## Features
* Mines blocks filled with transaction data using Proof of Work
* Uses [leveldb](https://github.com/google/leveldb) to store and access blocks on disk (Block data itself may be stored elsewhere eventually)
* Maintains a local wallet balance based on previous transactions (Upcoming)
* Uses private keys to send KES coins to other Kestrel wallets (Upcoming)
* Handles transaction data with Merkle Trees (Upcoming)
* Networking to interact with other nodes (Upcoming)
* Consensus with other nodes on the network to establish which blocks should be added to the chain (Upcoming)
* Cross Platform (Upcoming)

## Downloading Kestrel
### Windows
Windows will be supported soon.

### Unix Systems
A precompiled binary (.app) can be found in the /bin directory above.

If you wish to compile Kestrel yourself, please download QT C++ so that you have the necessary QT libraries on your system.
Make a directory of your choice for Kestrel and use a command line tool to cd to it. Clone the repository by running the following:
```sh
git clone https://github.com/maeteu/kestrel
```

## Additional Notes
* Kestrel stores block and transaction data at /tmp on Unix systems.