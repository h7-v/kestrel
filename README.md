# Kestrel
## _A Basic Blockchain Simulator_

A basic blockchain node written in C++ with QT as an introduction for myself into blockchain technology. This is my first real programming project - go easy on me!
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
* A precompiled Mac binary can be found on the [releases](https://github.com/maeteu/Kestrel/releases/) page.

If you wish to compile Kestrel yourself, please download QT C++ so that you have the necessary QT libraries on your system.
Make a directory of your choice for Kestrel and use a command line tool to clone the repository:
```sh
mkdir kestrel-node
cd kestrel-node
git clone https://github.com/maeteu/kestrel
```
Open the .pro file in QT. From the build dropdown, select "Run qmake", then select either run, debug, or build at the bottom left area of the window.

## Additional Notes
* Kestrel stores block and transaction data at /tmp on Unix systems.