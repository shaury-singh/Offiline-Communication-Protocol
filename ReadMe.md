# Offline Peer-to-Peer Communication Protocol

This project is a **custom peer-to-peer communication protocol** designed to enable **secure offline communication between nearby devices**.

The protocol simulates how real-world transport protocols establish connections, maintain state, and exchange data. It is currently implemented as a **protocol simulator**, with plans to extend it into a **real communication system using audio signal transmission via Android's AudioTrack API**.

---

# How To Run The Project:

### NOTE: This project depends on OpenSSL libraries.

Clone the repo using the git command:
`git clone https://github.com/shaury-singh/Offiline-Communication-Protocol.git`

In the terminal, run the command:
`g++ encryption.cpp fsk.cpp generatingFunctions.cpp keyDerivation.cpp merchant.cpp payer.cpp merchant.cpp sessionKey.cpp -o p2pProtocol.exe -lssl -lcrypto`

--- 

# Project Motivation

Most modern communication protocols rely on internet connectivity or centralized infrastructure.

This project explores an alternative approach by designing a **self-contained communication protocol** capable of operating **offline between nearby devices**, using **sound signals as the physical transmission medium**.

The goal is to understand and implement the **core principles behind network protocol design**, including:

* Connection establishment
* Packet sequencing
* Acknowledgements
* State machines
* Secure session establishment

---

# Architecture Overview

The system currently simulates communication between two peers through a controller that mimics network packet exchange.

### Entities

**Payer**

* Initiates the communication request.

**Merchant**

* Responds to connection requests and participates in the session.

**Controller**

* Simulates packet routing and protocol behavior during development.

```
Payer  →  Controller  →  Merchant
```

In the future implementation, the controller will be replaced by **direct signal transmission between devices**.

---

# Connection Establishment

The protocol implements a **three-step handshake mechanism inspired by TCP** to establish reliable communication.

### Step 1 – Connection Request

```
Payer → Merchant
SYN
seq = x
```

---

### Step 2 – Acknowledgement

```
Merchant → Payer
SYN + ACK
seq = y
ack = x + 1
```

---

### Step 3 – Connection Confirmation

```
Payer → Merchant
ACK
ack = y + 1
```

Once this process completes, the connection state transitions to:

```
ESTABLISHED
```

---

# Packet Structure

Packets are currently simulated as structured data containing protocol metadata.

Example representation:

```
{ sequence_number, acknowledgement_number, SYN_flag, ACK_flag }
```

These packets are processed through the protocol controller during simulation.

---

# Protocol State Machine

Each peer maintains an internal **connection state machine** to ensure that packets are processed only in valid states.

Example states include:

```
CLOSED
SYN_SENT
SYN_RECEIVED
ESTABLISHED
```

This prevents invalid packet sequences and enforces correct protocol behavior.

---

# Cryptographic Components

The protocol integrates cryptographic primitives to enable secure communication.

### Implemented Components

* **AES-128 encryption**
* **HMAC-SHA256 based key derivation**

These components will be used to derive **session keys after the handshake process**, enabling encrypted message exchange between peers.

---

# Future Communication Layer

The final version of the protocol will replace simulated packet exchange with **real-world signal transmission**.

Communication will occur through **audio frequencies in real time**.

### Transmission Pipeline

```
Protocol Packet
      ↓
Signal Encoding
      ↓
Sound Transmission
      ↓
Nearby Device Reception

```

This enables **device-to-device communication without internet connectivity**.

---

# Current Status

### Implemented

* Protocol handshake simulation
* Packet sequencing
* Acknowledgement tracking
* Initial protocol controller
* AES-128 encryption module
* HMAC-SHA256 key derivation
* Message transmission after handshake
* Packet encoding into audio signals
* Complete protocol state machine
* Packet validation
* Secure session establishment

### In Progress

* Key Ratcheting For Forward Secrecy
* Audio Packet Demodulation into serialized bits
* Real device-to-device communication

---

# Technologies Used

* **C++**
* Protocol simulation
* Cryptographic primitives
* Networking concepts
* Android AudioTrack API (planned)

---