# sip-softphone

A C++ VoIP platform: SIP server and softphone for cloud telephony.

The goal is a working signaling and media stack (SIP/SDP/RTP) that can serve real clients and grow into HA/clustering.

## What exists today

The repo currently focuses on **`sip-server`** — a UDP SIP server:

- SIP over UDP (`0.0.0.0:5060`)
- SIP message parsing (request-line, headers, body)
- Minimal SDP parsing (`c=` / `m=`)
- In-memory registrar: `REGISTER` handling, AOR storage with expiry, responses to clients

## Roadmap

| Area | Direction |
|------|-----------|
| Signaling | `INVITE` / dialog / `ACK` / `BYE`, routing to registered peers |
| Media | RTP/RTCP, media exchange between call legs |
| Softphone | Client app alongside the server |
| Reliability | Event loop, capacity limits, Active/Standby, state replication |
| Infra | SIP over TCP, observability, deployment |

## Stack

- **C++20**
- **CMake** ≥ 3.10
- POSIX sockets (UDP), Linux/macOS

## Layout

```
sip-softphone/
├── sip-server/          # SIP server
│   ├── include/         # net/, sip/, util/
│   └── src/
└── README.md
```

## Build and run

```bash
cd sip-server
cmake -S . -B build
cmake --build build
./build/sip-server
```

The server listens for SIP on port **5060**. Any SIP client can register against this host/port.
