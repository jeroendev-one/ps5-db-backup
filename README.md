# PS5 Backup payload
A payload to be sent to the [ELF loader](https://github.com/john-tornblom/ps5-payload-elfldr) on port 9021. 

This payload backups up several databases from `/system_data/priv/mms` to your USB. At the time of writing FPKG's from external drives are still on the system but lost from the homescreen after a forced rebuild of the database, in case you have a backup using this payload you can restore that. Internally installed FPKG's will be gone after a rebuild.

## Requirements
- [PS5-Payload-SDK and the requirements for the SDK](https://github.com/john-tornblom/ps5-payload-sdk/tree/master)
- Exploitable PS5 [3.xx-4.xx]
- USB formatted in exFAT or FAT32

## Build
Only build:
```bash
export PS5_PAYLOAD_SDK=/opt/ps5-payload-sdk
make
```

Build & inject:
```bash
export PS5_PAYLOAD_SDK=/opt/ps5-payload-sdk
export PS5_HOST=192.168.1.175
export PS5_PORT=9021
make test
```

Or inject manually:
```bash
nc -q0 192.168.1.175 < backup.elf
```